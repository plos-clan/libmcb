/* This file is part of libmcb.
   SPDX-License-Identifier: LGPL-3.0-or-later
*/
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "mcb/func.h"
#include "mcb/inst.h"
#include "mcb/inst/address_of.h"
#include "mcb/value.h"

#define LIBMCB_STRIP
#include "gen_mov.h"
#include "gnu_asm.h"
#include "inst.h"
#include "mem.h"
#include "reg.h"
#include "struct.h"
#include "value.h"

#include "../utils.h"
#include "../../ealloc.h"
#include "../../err.h"
#include "../../str.h"
#include "../../text_block.h"

static int address_of_mem(
		struct gnu_asm_value *val,
		struct mcb_address_of_inst *inst,
		struct mcb_func *fn,
		struct gnu_asm *ctx);
static int address_of_struct(
		struct gnu_asm_struct_value *struct_val,
		struct mcb_address_of_inst *inst,
		struct mcb_func *fn,
		struct gnu_asm *ctx);
static struct gnu_asm_value *alloc_result(
		struct mcb_value *container,
		struct mcb_func *fn);
static void gen_lea(
		const char *dst,
		const char *src,
		enum GNU_ASM_VALUE_KIND kind,
		struct gnu_asm *ctx);

int
address_of_mem(struct gnu_asm_value *val,
		struct mcb_address_of_inst *inst,
		struct mcb_func *fn,
		struct gnu_asm *ctx)
{
	struct str dst, src;
	struct gnu_asm_mem_obj *mem;
	struct gnu_asm_value *result;
	assert(val && inst && fn && ctx);
	assert(inst->result);

	result = inst->result->data = alloc_result(inst->result, fn);

	str_from_value(&dst, result);
	str_from_mem(&src, val);

	mem = ecalloc(1, sizeof(*mem));
	mem->base = result->inner.reg;
	mem->kind = VAR_MEM;
	mem->offset = 0;
	mem->user = result;
	result->kind = map_type_to_value_kind(I8_MEM_VALUE, inst->val->type);
	result->inner.mem = mem;

	gen_lea(dst.s, src.s, I64_REG_VALUE, ctx);

	str_free(&dst);
	str_free(&src);

	return 0;
}

int
address_of_struct(
		struct gnu_asm_struct_value *struct_val,
		struct mcb_address_of_inst *inst,
		struct mcb_func *fn,
		struct gnu_asm *ctx)
{
	struct str dst, src;
	struct gnu_asm_mem_obj *mem;
	struct gnu_asm_value *result;
	struct gnu_asm_value *val;

	assert(struct_val && inst && fn && ctx);
	assert(inst->val->type->builtin == MCB_STRUCT);
	val = struct_val->values[0];
	assert(val);

	result = inst->result->data = alloc_result(inst->result, fn);

	str_from_value(&dst, result);
	str_from_mem(&src, val);

	mem = ecalloc(1, sizeof(*mem));
	mem->base = result->inner.reg;
	mem->kind = VAR_MEM;
	mem->offset = 0;
	mem->user = result;
	result->kind = STRUCT_VALUE;
	result->inner.mem = mem;

	gen_lea(dst.s, src.s, I64_REG_VALUE, ctx);

	str_free(&dst);
	str_free(&src);

	return 0;
}

struct gnu_asm_value *
alloc_result(struct mcb_value *container, struct mcb_func *fn)
{
	struct gnu_asm_value *result;
	assert(container && fn);
	result = ecalloc(1, sizeof(*result));
	result->container = container;
	result->kind = I64_REG_VALUE;
	result->inner.reg = alloc_reg(AUTO_ALLOC_REG, result, fn);
	if (result->inner.reg == REG_COUNT)
		eabort("alloc_reg()");
	return result;
}

void
gen_lea(const char *dst,
		const char *src,
		enum GNU_ASM_VALUE_KIND kind,
		struct gnu_asm *ctx)
{
	struct text_block *blk;
	int len;
	estr_clean(&ctx->buf);
	len = snprintf(ctx->buf.s, ctx->buf.siz,
			"lea%c %s, %s\n",
			get_inst_suffix(kind),
			src, dst);
	if (len < 0)
		eabort("snprintf()");
	ctx->buf.len = len;
	blk = text_block_from_str(&ctx->buf);
	append_text_block(&ctx->text, blk);
}

int
build_address_of_inst(struct mcb_inst *inst_outer,
		struct mcb_func *fn,
		struct gnu_asm *ctx)
{
	struct mcb_address_of_inst *inst;
	struct gnu_asm_value *val;
	assert(inst_outer && fn && ctx);
	inst = &inst_outer->inner.address_of;
	assert(inst);
	
	if (mcb_is_inst_unwanted(inst->result, inst_outer))
		return 0;

	assert(inst->val);
	if (inst->val->type->builtin == MCB_STRUCT)
		return address_of_struct(inst->val->data, inst, fn, ctx);

	val = inst->val->data;
	assert(val);

	switch (val->kind) {
	CASE_IMM_VALUE: ereturn(1, "get address of immediate");
	CASE_MEM_VALUE: return address_of_mem(val, inst, fn, ctx);
	CASE_REG_VALUE: ereturn(1, "get address of register");
	default: eabort("get address of 'unknown value'");
	}

	return 0;
}
