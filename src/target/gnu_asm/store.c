/* This file is part of libmcb.
   SPDX-License-Identifier: LGPL-3.0-or-later
*/
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
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

static int store_imm(struct mcb_store_inst *inst);
static int store_normal_value(struct mcb_store_inst *inst, struct gnu_asm *ctx);
static int store_string(struct mcb_store_inst *inst, struct gnu_asm *ctx);
static int store_to_array_elem(
		struct mcb_inst *inst_outer,
		struct gnu_asm *ctx);
static int store_to_struct_elem(
		struct mcb_inst *inst_outer,
		struct gnu_asm *ctx);
static int store_to_value(
		struct gnu_asm_value *val,
		struct mcb_inst *inst_outer,
		struct gnu_asm *ctx);
static int store_to_var(struct mcb_inst *inst_outer, struct gnu_asm *ctx);

int
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
	return 0;
}

int
store_normal_value(struct mcb_store_inst *inst, struct gnu_asm *ctx)
{
	assert(inst);
	switch (inst->kind) {
	case MCB_STORE_INT:
	case MCB_STORE_UINT:
		return store_imm(inst);
	case MCB_STORE_STRING:
		return store_string(inst, ctx);
	case MCB_STORE_VALUE:
		inst->container->data = inst->operand.value->data;
		return 0;
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
store_to_array_elem(
		struct mcb_inst *inst_outer,
		struct gnu_asm *ctx)
{
	struct mcb_store_inst *inst;
	struct gnu_asm_value *val;
	assert(inst_outer && ctx);
	inst = &inst_outer->inner.store;
	assert(inst);
	assert(inst->container);

	val = inst->container->data;
	assert(val);

	return store_to_value(val, inst_outer, ctx);
}

/* shits 💩 */
int
store_to_struct_elem(
		struct mcb_inst *inst_outer,
		struct gnu_asm *ctx)
{
	struct mcb_store_inst *inst = &inst_outer->inner.store;
	struct mcb_struct_elem_value *struct_elem;
	struct gnu_asm_struct_value *struct_val;
	struct gnu_asm_value *val;

	struct_elem = &inst->container->inner.structure_elem;
	assert(struct_elem->structure_container);
	struct_val = struct_elem->structure_container->data;
	val = struct_val->values[struct_elem->idx];
	
	return store_to_value(val, inst_outer, ctx);
}

int
store_to_value(struct gnu_asm_value *val,
		struct mcb_inst *inst_outer,
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
	if (gen_mov(&ctx->buf, val, src_ptr))
		eabort("gen_mov()");
	blk = text_block_from_str(&ctx->buf);
	append_text_block(&ctx->text, blk);

	return 0;
}

int
store_to_var(struct mcb_inst *inst_outer, struct gnu_asm *ctx)
{
	struct mcb_store_inst *inst = &inst_outer->inner.store;
	struct gnu_asm_value *val = inst->container->data;
	assert(val);
	return store_to_value(val, inst_outer, ctx);
}

int
build_store_inst(struct mcb_inst *inst_outer,
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
		return store_to_var(inst_outer, ctx);
	}
	return 0;
}
