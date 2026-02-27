/* This file is part of libmcb.
   SPDX-License-Identifier: LGPL-3.0-or-later
*/
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "mcb/func.h"
#include "mcb/inst/call.h"

#define LIBMCB_STRIP
#include "func.h"
#include "gen_mov.h"
#include "gnu_asm.h"
#include "inst.h"
#include "label.h"
#include "reg.h"
#include "utils.h"
#include "value.h"
#include "value_kind.h"

#include "../../darr.h"
#include "../../ealloc.h"
#include "../../err.h"
#include "../../macros.h"
#include "../../str.h"
#include "../../text_block.h"

struct func_call_context {
	/* drop them after calling by drop_arg() */
	int argc;
	struct gnu_asm_value **args;

	struct mcb_func *callee;

	struct mcb_func *fn;
	struct mcb_call_inst *inst;
	struct mcb_inst *inst_outer;
	struct gnu_asm *ctx;
};

static void align_stack(struct gnu_asm_func *fn, struct gnu_asm *ctx);
static void build_call(const struct mcb_func *callee, struct gnu_asm *ctx);
static bool can_define_label(
		const struct mcb_func *fn,
		size_t label_idx,
		size_t inst_idx);
static struct text_block *define_func_beg(struct mcb_func *fn, struct gnu_asm *ctx);
static void define_func_end(struct mcb_func *fn, struct gnu_asm *ctx);
static void drop_arg(int idx, struct func_call_context *ctx);
static void init_func_arg_value(int idx, struct mcb_func *fn);
static bool is_arg_reg(
		enum GNU_ASM_REG reg,
		int argc,
		struct gnu_asm_value **args);
static bool is_callee_save_reg(enum GNU_ASM_REG reg);
static bool is_scope_end_at_call(enum GNU_ASM_REG reg, struct func_call_context *ctx);
static void push_arg(int idx, struct func_call_context *ctx);
static void save_regs_before_call(struct func_call_context *ctx);

static const enum GNU_ASM_REG callee_save_regs[] = {
	RBX, R12, R13, R14, R15
};

static const enum GNU_ASM_REG arg_alloc_arr[] = {
	RDI, RSI, RDX, RCX, R8, R9
};

void
align_stack(struct gnu_asm_func *fn, struct gnu_asm *ctx)
{
	int aligned_stack, bytes;
	struct text_block *blk;
	struct gnu_asm_mem_obj *last_mem;
	int last_mem_bytes;
	int len;

	assert(fn && ctx);

	if (!fn->allocated_mem) {
		assert(fn->allocated_mem_count == 0);
		return;
	}

	last_mem = fn->allocated_mem[fn->allocated_mem_count - 1];
	last_mem_bytes = map_value_kind_to_bytes(last_mem->user->kind);
	bytes = -(last_mem->offset) + last_mem_bytes;
	aligned_stack = (bytes + 15) & -16;

	estr_clean(&ctx->buf);
	len = snprintf(ctx->buf.s, ctx->buf.siz, "subq $%d, %%rsp\n", aligned_stack);
	if (len < 0)
		eabort("snprintf()");
	blk = text_block_from_str(&ctx->buf);

	insert_text_block(&ctx->text,
			fn->beg_blk,
			fn->beg_blk->nex,
			blk);
}

void
build_call(const struct mcb_func *callee, struct gnu_asm *ctx)
{
	struct text_block *blk;
	int len;
	assert(callee && ctx);
	assert(callee->name);
	estr_clean(&ctx->buf);
	len = snprintf(ctx->buf.s, ctx->buf.siz,
			"call %s\n",
			callee->name);
	if (len < 0)
		eabort("snprintf()");
	ctx->buf.len = len;
	blk = text_block_from_str(&ctx->buf);
	append_text_block(&ctx->text, blk);
}

bool
can_define_label(
		const struct mcb_func *fn,
		size_t label_idx,
		size_t inst_idx)
{
	if (!fn->label_arr)
		return false;
	if (label_idx >= fn->label_arr_count)
		return false;
	if (fn->label_arr[label_idx]->beg == fn->inst_arr[inst_idx])
		return true;
	return false;
}

struct text_block *
define_func_beg(struct mcb_func *fn, struct gnu_asm *ctx)
{
	struct text_block *blk;
	struct gnu_asm_func *f;
	assert(fn && ctx);

	f = ecalloc(1, sizeof(*f));
	fn->data = f;

	estr_clean(&ctx->buf);

	if (fn->export_type == MCB_EXPORT_FUNC)
		ctx->buf.len = snprintf(
				ctx->buf.s,
				ctx->buf.siz,
				".globl %s\n", fn->name);

	ctx->buf.len += snprintf(
			&ctx->buf.s[ctx->buf.len],
			ctx->buf.siz - ctx->buf.len,
			"%s:\n"
			"pushq %%rbp\n"
			"movq %%rsp, %%rbp\n", fn->name);

	blk = text_block_from_str(&ctx->buf);
	append_text_block(&ctx->text, blk);

	/* alloc func->args[*]->val_link->data */
	if (fn->argc && !fn->args)
		eabort("fn->argc > 0 && fn->args == NULL");
	for (int i = 0; i < fn->argc; i++)
		init_func_arg_value(i, fn);

	return blk;
}

void
define_func_end(struct mcb_func *fn,
		struct gnu_asm *ctx)
{
	struct gnu_asm_func *f;

	assert(fn && ctx);
	f = fn->data;
	assert(f);

	align_stack(f, ctx);
}

void
drop_arg(int idx, struct func_call_context *ctx)
{
	struct mcb_value val;
	assert(ctx);
	assert(ctx->fn);

	if (ctx->argc == 0 && ctx->args == NULL) {
		assert(ctx->callee->argc == 0);
		return;
	}

	val.data = ctx->args[idx];
	assert(val.data);

	/* drop_value() ignore content in mcb_value other than val.data */
	drop_value(&val, ctx->fn);

	ctx->args[idx] = NULL;
	free(val.data);
}

void
init_func_arg_value(int idx, struct mcb_func *fn)
{
	struct gnu_asm_value *gval;
	struct mcb_func_arg *arg;

	assert(fn && fn->args);
	arg = fn->args[idx];
	assert(arg);

	if (!arg->val_link)
		return;

	if (idx > (int)LENGTH(arg_alloc_arr))
		eabort("unsupport, idx > LENGTH(arg_alloc_arr)");

	gval = arg->val_link->data = ecalloc(1, sizeof(*gval));
	gval->container = arg->val_link;
	gval->kind = map_type_to_value_kind(I8_REG_VALUE, arg->type);
	gval->inner.reg = alloc_reg(arg_alloc_arr[idx], gval, fn);
	if (gval->inner.reg == REG_COUNT)
		eabort("alloc_reg()");
}

bool
is_arg_reg(enum GNU_ASM_REG reg, int argc, struct gnu_asm_value **args)
{
	assert(args);
	for (int i = 0; i < argc; i++) {
		assert(IS_REG(args[i]->kind));
		if (args[i]->inner.reg == reg)
			return true;
	}
	return false;
}

bool
is_callee_save_reg(enum GNU_ASM_REG reg)
{
	for (int i = 0; i < (int)LENGTH(callee_save_regs); i++) {
		if (reg == callee_save_regs[i])
			return true;
	}
	return false;
}

bool
is_scope_end_at_call(enum GNU_ASM_REG reg, struct func_call_context *ctx)
{
	struct gnu_asm_func *f;
	assert(ctx);
	assert(ctx->fn);
	assert(ctx->fn->args);
	f = ctx->fn->data;
	assert(f);
	if (!f->allocated_reg[reg])
		return true;
	if (f->allocated_reg[reg]->container->scope_end == ctx->inst_outer)
		return true;
	return false;
}

void
push_arg(int idx, struct func_call_context *ctx)
{
	struct gnu_asm_value *arg;
	struct text_block *blk;
	const struct gnu_asm_value *val;
	assert(ctx);
	assert(ctx->fn && ctx->inst && ctx->ctx);
	assert(ctx->inst->args);
	assert(ctx->inst->args[idx]);
	val = ctx->inst->args[idx]->data;
	assert(val);

	if (idx > (int)LENGTH(arg_alloc_arr))
		eabort("unsupport: idx > LENGTH(arg_alloc_arr)");

	if (IS_REG(val->kind) && val->inner.reg == arg_alloc_arr[idx])
		return;

	arg = ecalloc(1, sizeof(*arg));
	arg->container = NULL;
	arg->kind = remap_value_kind(I8_REG_VALUE, val->kind);
	arg->inner.reg = alloc_reg(arg_alloc_arr[idx], arg, ctx->fn);
	if (arg->inner.reg == REG_COUNT) {
		if (mov_reg_user_to_mem(arg_alloc_arr[idx], ctx->fn, ctx->ctx))
			eabort("mov_reg_user_to_mem()");
		drop_reg(arg_alloc_arr[idx], ctx->fn);
		arg->inner.reg = alloc_reg(arg_alloc_arr[idx], arg, ctx->fn);
		if (arg->inner.reg == REG_COUNT)
			eabort("alloc_reg()");
	}

	estr_clean(&ctx->ctx->buf);
	if (gen_mov(&ctx->ctx->buf, arg, val))
		return;
	blk = text_block_from_str(&ctx->ctx->buf);
	append_text_block(&ctx->ctx->text, blk);

	darr_append(ctx->args, ctx->argc, arg);
}

void
save_regs_before_call(struct func_call_context *ctx)
{
	struct gnu_asm_func *f;

	assert(ctx);
	assert(ctx->fn);
	if (!ctx->fn->argc && !ctx->fn->args)
		return;
	if (!ctx->argc && !ctx->args)
		return;
	f = ctx->fn->data;
	assert(f);

	for (int i = 0; i < REG_COUNT; i++) {
		if (i == RAX || i == RBP || i == RSP)
			continue;
		if (!f->allocated_reg[i])
			continue;
		if (is_callee_save_reg(i))
			continue;
		/* In push_arg(), the function argument registers of caller
		 * are saved to memory. And the function argument registers
		 * are just for callee only, don't need save. */
		if (is_arg_reg(i, ctx->argc, ctx->args))
			continue;
		if (is_scope_end_at_call(i, ctx))
			continue;
		if (mov_reg_user_to_mem(i, ctx->fn, ctx->ctx))
			eabort("mov_reg_user_to_mem()");
		drop_reg(i, ctx->fn);
	}
}

int
define_func(struct mcb_func *fn, struct gnu_asm *ctx)
{
	struct text_block *beg_blk;
	struct gnu_asm_func *f;
	assert(fn && ctx);

	beg_blk = define_func_beg(fn, ctx);
	f = fn->data;
	assert(f);
	f->beg_blk = beg_blk;
	for (size_t i = 0, label_i = 0; i < fn->inst_arr_count; i++) {
		if (can_define_label(fn, label_i, i)) {
			define_label(fn->label_arr[label_i], fn, ctx);
			label_i++;
		}

		if (build_inst(fn->inst_arr[i], fn, ctx))
			return 1;
	}
	define_func_end(fn, ctx);

	/* destroy allocated_mem */

	return 0;
}

int
build_call_inst(struct mcb_inst *inst_outer,
		struct mcb_func *fn,
		struct gnu_asm *ctx)
{
	struct func_call_context call_ctx = {0};
	struct mcb_call_inst *inst;
	struct gnu_asm_value *result;
	assert(inst_outer && fn && ctx);
	inst = &inst_outer->inner.call;
	assert(inst);
	if (inst->result->scope_end == inst_outer)
		return 0;

	assert(inst->callee);

	call_ctx.fn = fn;
	call_ctx.inst = inst;
	call_ctx.inst_outer = inst_outer;
	call_ctx.ctx = ctx;

	result = inst->result->data = ecalloc(1, sizeof(*result));
	result->container = inst->result;
	result->kind = map_type_to_value_kind(I8_REG_VALUE, inst->result->type);
	result->inner.reg = alloc_reg(RAX, result, fn);
	if (result->inner.reg == REG_COUNT) {
		if (mov_reg_user(RAX, fn, ctx))
			eabort("mov_reg_user()");
		drop_reg(RAX, fn);
		result->inner.reg = alloc_reg(RAX, result, fn);
		if (result->inner.reg == REG_COUNT)
			eabort("alloc_reg()");
	}

	for (int i = 0; i < inst->callee->argc; i++)
		push_arg(i, &call_ctx);

	save_regs_before_call(&call_ctx);
	build_call(inst->callee, ctx);

	for (int i = 0; i < call_ctx.argc; i++)
		drop_arg(i, &call_ctx);
	free(call_ctx.args);

	return 0;
}

void
destroy_func(struct mcb_func *container)
{
	struct gnu_asm_func *fn = container->data;
	for (size_t i = 0; i < container->value_arr_count; i++)
		destroy_value(container->value_arr[i]);
	free(fn->allocated_mem);
	free(fn);
	container->data = NULL;
}
