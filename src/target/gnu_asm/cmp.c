/* This file is part of libmcb.
   SPDX-License-Identifier: LGPL-3.0-or-later
*/
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "mcb/func.h"
#include "mcb/inst.h"
#include "mcb/inst/cmp.h"
#include "mcb/value.h"

#define LIBMCB_STRIP
#include "cmp.h"
#include "gen_mov.h"
#include "gnu_asm.h"
#include "inst.h"
#include "value.h"
#include "value_kind.h"

#include "../../ealloc.h"
#include "../../err.h"
#include "../../str.h"

static void build_lhs(struct str *str,
		struct gnu_asm_value *val,
		struct mcb_func *fn,
		struct gnu_asm *ctx);
static void build_rhs(struct str *str,
		struct gnu_asm_value *val,
		struct mcb_func *fn,
		struct gnu_asm *ctx);

void
build_lhs(struct str *str,
		struct gnu_asm_value *val,
		struct mcb_func *fn,
		struct gnu_asm *ctx)
{
	assert(str && val && fn && ctx);
	/* TODO: handle different value kind */
	str_from_value(str, val);
}

void
build_rhs(struct str *str,
		struct gnu_asm_value *val,
		struct mcb_func *fn,
		struct gnu_asm *ctx)
{
	struct text_block *blk;
	struct gnu_asm_value tmp;
	assert(str && val && fn && ctx);

	if (IS_IMM(val->kind)) {
		estr_clean(&ctx->buf);
		tmp = *val;
		val->kind = remap_value_kind(I8_REG_VALUE, val->kind);
		val->inner.reg = alloc_reg(AUTO_ALLOC_REG, val, fn);
		if (gen_mov(&ctx->buf, val, &tmp))
			eabort("gen_mov()");
		blk = text_block_from_str(&ctx->buf);
		append_text_block(&ctx->text, blk);
	}
	str_from_value(str, val);
}

int
build_cmp_inst(struct mcb_inst *inst_outer,
		struct mcb_func *fn,
		struct gnu_asm *ctx)
{
	struct text_block *blk;
	struct mcb_cmp_inst *inst;
	int len;
	struct str lhs_str, rhs_str;
	struct gnu_asm_value *lhs, *rhs, *result;

	assert(inst_outer && fn && ctx);
	inst = &inst_outer->inner.cmp;
	assert(inst->result && inst->result->scope_end);

	assert(inst->lhs && inst->rhs);
	lhs = inst->lhs->data;
	rhs = inst->rhs->data;
	assert(lhs && rhs);

	result = inst->result->data = ecalloc(1, sizeof(*result));
	result->kind = CMP_RESULT_VALUE;
	result->inner.operator = inst->operator;

	/* Operation direction is reversed in gnu assembly,
	 * So, [lhs_str] and [rhs_str] also need reversed.
	 * In fact, I don't fucking know wtf gnu asm
	 * and amd64 architecture. */
	build_lhs(&rhs_str, lhs, fn, ctx);
	build_rhs(&lhs_str, rhs, fn, ctx);

	estr_clean(&ctx->buf);
	len = snprintf(ctx->buf.s, ctx->buf.siz, "cmp%c %s, %s\n",
			get_inst_suffix(rhs->kind),
			lhs_str.s, rhs_str.s);
	if (len < 0)
		ereturn(1, "snprintf()");
	ctx->buf.len = len;
	blk = text_block_from_str(&ctx->buf);
	append_text_block(&ctx->text, blk);

	if (inst->lhs->scope_end == inst_outer)
		drop_value(inst->lhs, fn);
	if (inst->rhs->scope_end == inst_outer)
		drop_value(inst->rhs, fn);

	str_free(&lhs_str);
	str_free(&rhs_str);

	return 0;
}

enum MCB_CMP_OPERATOR
reverse_cmp_op(enum MCB_CMP_OPERATOR op)
{
	switch (op) {
	case MCB_GT: return MCB_LE;
	case MCB_LE: return MCB_GT;
	}
	eabort("reverse_cmp_op(): location that must not be reached");
	return -1;
}
