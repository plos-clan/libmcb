/* This file is part of libmcb.
   SPDX-License-Identifier: LGPL-3.0-or-later
*/
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "mcb/func.h"
#include "mcb/inst.h"
#include "mcb/value.h"

#define LIBMCB_STRIP
#include "func.h"
#include "gen_mov.h"
#include "gnu_asm.h"
#include "inst.h"
#include "utils.h"
#include "value.h"

#include "../utils.h"
#include "../../ealloc.h"
#include "../../err.h"
#include "../../str.h"
#include "../../text_block.h"

static void build_lhs(struct gnu_asm_value *result,
		const struct gnu_asm_value *val,
		struct mcb_func *fn,
		struct gnu_asm *ctx);
static void build_rhs(struct str *src,
		const struct gnu_asm_value *val,
		struct mcb_func *fn,
		struct gnu_asm *ctx);
static int gen_imul(enum GNU_ASM_VALUE_KIND result_kind,
		struct str *src,
		struct gnu_asm *ctx);
static int gen_mul(enum GNU_ASM_VALUE_KIND result_kind,
		struct str *src,
		struct gnu_asm *ctx);
static void get_lhs_and_rhs(
		struct gnu_asm_value **lhs,
		struct gnu_asm_value **rhs,
		const struct mcb_mul_inst *inst);
static struct gnu_asm_value *get_result(
		const struct mcb_value *res,
		struct mcb_func *fn);
static bool need_mov_to_rax(const struct gnu_asm_value *val);

void
build_lhs(struct gnu_asm_value *result,
		const struct gnu_asm_value *val,
		struct mcb_func *fn,
		struct gnu_asm *ctx)
{
	struct text_block *blk;
	assert(result && val && fn && ctx);

	if (!need_mov_to_rax(val))
		return;

	estr_clean(&ctx->buf);
	if (gen_mov(&ctx->buf, result, val))
		eabort("gen_mov()");
	blk = text_block_from_str(&ctx->buf);
	append_text_block(&ctx->text, blk);
}

void
build_rhs(struct str *src,
		const struct gnu_asm_value *val,
		struct mcb_func *fn,
		struct gnu_asm *ctx)
{
	struct text_block *blk;
	struct gnu_asm_value tmp;
	assert(src && val && fn && ctx);

	if (IS_IMM(val->kind)) {
		tmp.kind = remap_value_kind(I8_REG_VALUE, val->kind);
		tmp.inner.reg = alloc_reg(AUTO_ALLOC_REG, &tmp, fn);
		if (tmp.inner.reg == REG_COUNT)
			eabort("alloc_reg()");
		estr_clean(&ctx->buf);
		if (gen_mov(&ctx->buf, &tmp, val))
			eabort("gen_mov()");
		blk = text_block_from_str(&ctx->buf);
		append_text_block(&ctx->text, blk);
		str_from_value(src, &tmp);
		drop_reg(tmp.inner.reg, fn);
	} else if (IS_REG(val->kind)) {
		str_from_value(src, val);
	}
}

int
gen_imul(enum GNU_ASM_VALUE_KIND result_kind,
		struct str *src,
		struct gnu_asm *ctx)
{
	return snprintf(ctx->buf.s, ctx->buf.siz,
			"imul%c %s\n",
			get_inst_suffix(result_kind),
			src->s);
}

int
gen_mul(enum GNU_ASM_VALUE_KIND result_kind,
		struct str *src,
		struct gnu_asm *ctx)
{
	return snprintf(ctx->buf.s, ctx->buf.siz,
			"mul%c %s\n",
			get_inst_suffix(result_kind),
			src->s);
}

void
get_lhs_and_rhs(struct gnu_asm_value **lhs,
		struct gnu_asm_value **rhs,
		const struct mcb_mul_inst *inst)
{
	assert(inst);
	assert(inst->lhs && inst->rhs);
	assert(lhs && rhs);
	*lhs = inst->lhs->data;
	*rhs = inst->rhs->data;
	assert(*lhs && *rhs);
}

struct gnu_asm_value *
get_result(const struct mcb_value *res, struct mcb_func *fn)
{
	struct gnu_asm_value *result;
	assert(res && fn);
	assert(res->data == NULL);
	result = ecalloc(1, sizeof(*result));
	result->container = res;
	result->kind = map_type_to_value_kind(
			I8_REG_VALUE,
			res->type);
	result->inner.reg = alloc_reg(RAX, result, fn);
	if (result->inner.reg == REG_COUNT)
		eabort("alloc_reg()");
	return result;
}

bool
need_mov_to_rax(const struct gnu_asm_value *val)
{
	if (IS_IMM(val->kind))
		return true;
	if (IS_REG(val->kind) && val->inner.reg != RAX)
		return true;
	return false;
}

int
build_mul_inst(struct mcb_inst *inst_outer,
		struct mcb_func *fn,
		struct gnu_asm *ctx)
{
	struct text_block *blk;
	struct mcb_mul_inst *inst;
	int len;
	struct gnu_asm_value *lhs_val, *rhs_val, *result;
	struct str src;

	assert(inst_outer && fn && ctx);
	inst = &inst_outer->inner.mul;

	if (mcb_is_inst_unwanted(inst->result, inst_outer))
		return 0;

	get_lhs_and_rhs(&lhs_val, &rhs_val, inst);
	if (((struct gnu_asm_func*)fn->data)->allocated_reg[RAX]) {
		if (mov_reg_user(RAX, fn, ctx))
			eabort("mov_reg_user()");
		drop_reg(RAX, fn);
	}
	result = inst->result->data = get_result(inst->result, fn);
	result->container = inst->result;

	build_lhs(result, lhs_val, fn, ctx);
	build_rhs(&src, rhs_val, fn, ctx);

	estr_clean(&ctx->buf);

	if (IS_INT(result->kind)) {
		len = gen_imul(result->kind, &src, ctx);
	} else {
		len = gen_mul(result->kind, &src, ctx);
	}

	if (len < 0)
		return 1;
	ctx->buf.len = len;
	blk = text_block_from_str(&ctx->buf);
	append_text_block(&ctx->text, blk);

	if (inst->lhs->scope_end == inst_outer)
		drop_value(inst->lhs, fn);
	if (inst->rhs->scope_end == inst_outer)
		drop_value(inst->rhs, fn);

	return 0;
}
