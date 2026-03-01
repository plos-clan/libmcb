/* This file is part of libmcb.
   SPDX-License-Identifier: LGPL-3.0-or-later
*/
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "mcb/func.h"
#include "mcb/inst.h"
#include "mcb/inst/load.h"
#include "mcb/value.h"

#define LIBMCB_STRIP
#include "gen_mov.h"
#include "gnu_asm.h"
#include "inst.h"
#include "mem.h"
#include "reg.h"
#include "struct.h"
#include "value.h"

#include "../../ealloc.h"
#include "../../err.h"
#include "../../str.h"
#include "../../text_block.h"

static void alloc_struct_elem(int idx,
		struct mcb_struct_value *structure,
		struct gnu_asm_struct_value *structure_value,
		struct gnu_asm_value *address,
		struct mcb_func *fn);
static int load_struct(
		struct gnu_asm_value *address,
		struct mcb_load_inst *inst,
		struct mcb_func *fn,
		struct gnu_asm *ctx);

void
alloc_struct_elem(int idx,
		struct mcb_struct_value *structure,
		struct gnu_asm_struct_value *structure_value,
		struct gnu_asm_value *address,
		struct mcb_func *fn)
{
	struct gnu_asm_mem_obj *mem, *prv_mem;
	struct gnu_asm_value *val, *prv_val;
	struct mcb_value *val_container;
	assert(idx >= 0);
	assert(structure_value && structure && fn);
	assert(structure->values);
	assert(address->kind == STRUCT_VALUE);

	val_container = structure->values[idx];

	val = ecalloc(1, sizeof(*val));
	val->container = val_container;
	val->kind = map_type_to_value_kind(
			I8_MEM_VALUE,
			structure->structure->elems[idx]->type);
	val->inner.mem = mem = ecalloc(1, sizeof(*mem));
	mem->base = address->inner.mem->base;
	mem->offset = 0;
	mem->user = val;
	if (idx > 0) {
		prv_val = structure_value->values[idx - 1];
		assert(prv_val);
		assert(IS_MEM(prv_val->kind));
		prv_mem = prv_val->inner.mem;
		mem->offset = prv_mem->offset +
			map_value_kind_to_bytes(prv_val->kind);
	}
	structure_value->values[idx] = val;
	if (val_container)
		val_container->data = val;
}

int
load_struct(struct gnu_asm_value *address,
		struct mcb_load_inst *inst,
		struct mcb_func *fn,
		struct gnu_asm *ctx)
{
	struct mcb_struct *structure;
	struct gnu_asm_struct_value *structure_value;
	assert(address && inst && fn && ctx);

	assert(inst->result);
	assert(inst->result->type->builtin == MCB_STRUCT);
	structure = inst->result->inner.structure.structure;
	structure_value = ecalloc(1, sizeof(*structure_value));
	structure_value->values = ecalloc(
			structure->elems_count,
			sizeof(*structure_value->values));
	for (int i = 0; i < structure->elems_count; i++)
		alloc_struct_elem(i,
				&inst->result->inner.structure,
				structure_value,
				address,
				fn);
	inst->result->data = structure_value;
	return 0;
}

int
build_load_inst(struct mcb_inst *inst_outer,
		struct mcb_func *fn,
		struct gnu_asm *ctx)
{
	struct gnu_asm_value *address, *result;
	struct text_block *blk;
	struct mcb_load_inst *inst;
	assert(inst_outer && fn && ctx);
	inst = &inst_outer->inner.load;
	assert(inst);
	
	if (inst->result->scope_end == inst_outer)
		return 0;

	assert(inst->address);
	address = inst->address->data;
	assert(address);

	if (address->kind == STRUCT_VALUE)
		return load_struct(address, inst, fn, ctx);

	assert(inst->result);
	result = inst->result->data = ecalloc(1, sizeof(*result));
	result->container = inst->result;
	result->kind = map_type_to_value_kind(
			I8_REG_VALUE,
			inst->result->type);
	result->inner.reg = alloc_reg(AUTO_ALLOC_REG, result, fn);
	if (result->inner.reg == REG_COUNT)
		eabort("alloc_reg()");

	estr_clean(&ctx->buf);
	if (gen_mov(&ctx->buf, result, address))
		eabort("gen_mov()");
	blk = text_block_from_str(&ctx->buf);
	append_text_block(&ctx->text, blk);

	return 0;
}
