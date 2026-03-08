/* SPDX-License-Identifier: LGPL-3.0-or-later */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "mcb/func.h"
#include "mcb/inst.h"
#include "mcb/inst/sub.h"
#include "mcb/value.h"

#define LIBMCB_STRIP
#include "gen_mov.h"
#include "gnu_asm.h"
#include "inst.h"
#include "reg.h"
#include "value.h"

#include "../../ealloc.h"
#include "../../err.h"
#include "../../str.h"
#include "../../text_block.h"

static void mov_to_result(
		struct gnu_asm_value *result,
		struct gnu_asm_value *dst,
		struct mcb_func *fn,
		struct gnu_asm *ctx);

void
mov_to_result(struct gnu_asm_value *result,
		struct gnu_asm_value *dst,
		struct mcb_func *fn,
		struct gnu_asm *ctx)
{
	struct text_block *blk;
	assert(result && dst && ctx);
	estr_clean(&ctx->buf);
	if (gen_mov(&ctx->buf, result, dst, fn, ctx))
		eabort("gen_mov()");
	blk = text_block_from_str(&ctx->buf);
	append_text_block(&ctx->text, blk);
}

int
build_sub_inst(struct mcb_inst *inst_outer,
		struct mcb_func *fn,
		struct gnu_asm *ctx)
{
	struct text_block *blk;
	struct str dst, src;
	struct mcb_sub_inst *inst;
	struct gnu_asm_value *lhs_val, *rhs_val, *result;
	int len;

	assert(inst_outer && fn && ctx);
	inst = &inst_outer->inner.sub;

	assert(inst->lhs && inst->rhs);
	lhs_val = inst->lhs->data;
	rhs_val = inst->rhs->data;
	assert(lhs_val && rhs_val);

	assert(inst->result->data == NULL);
	result = ecalloc(1, sizeof(*result));
	result->container = inst->result;
	result->kind = map_type_to_value_kind(
			I8_REG_VALUE,
			inst->result->type);
	result->inner.reg = alloc_reg(AUTO_ALLOC_REG, result, fn);
	if (result->inner.reg == REG_COUNT)
		eabort("alloc_reg()");
	inst->result->data = result;

	if (IS_REG(lhs_val->kind) && inst->lhs->scope_end == inst_outer) {
		drop_reg(result->inner.reg, fn);
		free(result);
		result = lhs_val;
		inst->lhs->data = NULL;
		inst->result->data = result;
	}
	result->container = inst->result;

	mov_to_result(result, lhs_val, fn, ctx);
	str_from_value(&dst, result);
	str_from_value(&src, rhs_val);

	estr_clean(&ctx->buf);
	len = snprintf(ctx->buf.s, ctx->buf.siz,
			"sub%c %s, %s\n",
			get_inst_suffix(result->kind),
			src.s, dst.s);
	if (len < 0)
		eabort("snprintf()");
	ctx->buf.len = len;
	blk = text_block_from_str(&ctx->buf);
	append_text_block(&ctx->text, blk);

	str_free(&dst);
	str_free(&src);

	if (inst->lhs->scope_end == inst_outer)
		drop_value(inst->lhs, fn);
	if (inst->rhs->scope_end == inst_outer)
		drop_value(inst->rhs, fn);

	return 0;
}
