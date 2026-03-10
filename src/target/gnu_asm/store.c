/* SPDX-License-Identifier: LGPL-3.0-or-later */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mcb/func.h"
#include "mcb/inst.h"
#include "mcb/value.h"

#define LIBMCB_STRIP
#include "data.h"
#include "gen_mov.h"
#include "gnu_asm.h"
#include "inst.h"
#include "struct.h"
#include "value.h"

#include "../../ealloc.h"
#include "../../err.h"
#include "../../str.h"

static void store_imm(struct mcb_store_inst *inst);
static int store_normal_value(struct mcb_store_inst *inst, struct gnu_asm *ctx);
static int store_string(struct mcb_store_inst *inst, struct gnu_asm *ctx);
static int store_to_value(
		struct gnu_asm_value *val,
		struct mcb_inst *inst_outer,
		struct mcb_func *fn,
		struct gnu_asm *ctx);
static int store_to_var(
		struct mcb_inst *inst_outer,
		struct mcb_func *fn,
		struct gnu_asm *ctx);

void
store_imm(struct mcb_store_inst *inst)
{
	struct gnu_asm_value *v = ecalloc(1, sizeof(*v));
	v->container = inst->container;
	v->kind = map_type_to_value_kind(
			I8_IMM_VALUE,
			inst->container->type);
	if (v->kind == UNKOWN_VALUE)
		eabort("map_type_to_value_kind()");
	v->inner.imm.i = inst->operand.i;
	inst->container->data = v;
}

int
store_normal_value(struct mcb_store_inst *inst, struct gnu_asm *ctx)
{
	assert(inst);
	switch (inst->kind) {
	case MCB_STORE_INT:
	case MCB_STORE_UINT:
		store_imm(inst);
		break;
	case MCB_STORE_STRING:
		return store_string(inst, ctx);
	case MCB_STORE_VALUE:
		inst->container->data = ecalloc(1, sizeof(struct gnu_asm_value));
		memcpy(inst->container->data,
				inst->operand.value->data,
				sizeof(struct gnu_asm_value));
		break;
	}
	return 0;
}

int
store_string(struct mcb_store_inst *inst, struct gnu_asm *ctx)
{
	struct gnu_asm_value *v;
	assert(inst);
	v = ecalloc(1, sizeof(*v));
	v->container = inst->container;
	v->kind = map_type_to_value_kind(
			I8_DATA_VALUE,
			inst->container->type);
	if (v->kind == UNKOWN_VALUE)
		eabort("map_type_to_value_kind()");
	v->inner.data = alloc_str_data(inst->operand.str.str, v, ctx);
	inst->container->data = v;
	return 0;
}

int
store_to_value(struct gnu_asm_value *val,
		struct mcb_inst *inst_outer,
		struct mcb_func *fn,
		struct gnu_asm *ctx)
{
	struct text_block *blk;
	struct mcb_store_inst *inst = &inst_outer->inner.store;
	struct gnu_asm_value src, *src_ptr;
	assert(val);

	if (inst->kind == MCB_STORE_VALUE) {
		src_ptr = inst->operand.value->data;
	} else {
		src.kind = remap_value_kind(I8_IMM_VALUE, val->kind);
		src.inner.imm.i = inst->operand.i;
		src_ptr = &src;
	}

	estr_clean(&ctx->buf);
	if (gen_mov(&ctx->buf, val, src_ptr, fn, ctx))
		eabort("gen_mov()");
	blk = text_block_from_str(&ctx->buf);
	append_text_block(&ctx->text, blk);

	if (inst->kind == MCB_STORE_VALUE) {
		if (inst->operand.value->scope_end == inst_outer)
			drop_value(inst->operand.value, fn);
	}

	return 0;
}

int
store_to_var(
		struct mcb_inst *inst_outer,
		struct mcb_func *fn,
		struct gnu_asm *ctx)
{
	struct mcb_store_inst *inst = &inst_outer->inner.store;
	struct gnu_asm_value *val = inst->container->data;
	assert(val);
	return store_to_value(val, inst_outer, fn, ctx);
}

int
build_store_inst(struct mcb_inst *inst_outer,
		struct mcb_func *fn,
		struct gnu_asm *ctx)
{
	struct mcb_store_inst *inst;
	assert(inst_outer && ctx);
	inst = &inst_outer->inner.store;
	assert(inst);
	assert(inst->container);
	switch (inst->container->kind) {
	case MCB_NORMAL_VALUE:
		return store_normal_value(inst, ctx);
	case MCB_FUNC_ARG_VALUE:
		ereturn(1, "store to value of function argument");
	case MCB_VAR_VALUE:
		return store_to_var(inst_outer, fn, ctx);
	}
	return 0;
}
