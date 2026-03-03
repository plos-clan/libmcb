/* This file is part of libmcb.
   SPDX-License-Identifier: LGPL-3.0-or-later
*/
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "mcb/func.h"
#include "mcb/inst.h"
#include "mcb/inst/alloc_struct.h"
#include "mcb/value.h"

#define LIBMCB_STRIP
#include "gnu_asm.h"
#include "inst.h"
#include "mem.h"
#include "struct.h"
#include "value.h"

#include "../utils.h"
#include "../../ealloc.h"
#include "../../err.h"
#include "../../str.h"
#include "../../text_block.h"

static void alloc_elem(
		int idx,
		struct gnu_asm_struct_value *structure,
		struct mcb_alloc_struct_inst *inst,
		struct mcb_func *fn,
		struct gnu_asm *ctx);

void
alloc_elem(int idx,
		struct gnu_asm_struct_value *structure,
		struct mcb_alloc_struct_inst *inst,
		struct mcb_func *fn,
		struct gnu_asm *ctx)
{
	int bytes;
	struct gnu_asm_value *val;
	struct mcb_value *val_container;
	assert(structure && inst && fn && ctx);
	assert(structure->values);
	val_container = inst->container->inner.structure.values[idx];

	val = ecalloc(1, sizeof(*val));
	val->container = val_container;
	val->kind = map_type_to_value_kind(
			I8_MEM_VALUE,
			inst->structure->elems[idx]->type);
	bytes = map_value_kind_to_bytes(val->kind);
	val->inner.mem = alloc_stack_mem(bytes, val, fn);
	if (!val->inner.mem)
		eabort("alloc_stack_mem()");
	if (val_container)
		val_container->data = val;
	structure->values[idx] = val;
}

int
build_alloc_struct_inst(
		struct mcb_inst *inst_outer,
		struct mcb_func *fn,
		struct gnu_asm *ctx)
{
	struct mcb_alloc_struct_inst *inst;
	struct gnu_asm_struct_value *structure;

	assert(inst_outer && fn && ctx);
	inst = &inst_outer->inner.alloc_struct;
	assert(inst);
	assert(inst->container);

	if (mcb_is_inst_unwanted(inst->container, inst_outer))
		return 0;

	structure = ecalloc(1, sizeof(*structure));
	structure->values = ecalloc(
			inst->structure->elems_count,
			sizeof(*structure->values));
	inst->container->data = structure;

	for (int i = inst->structure->elems_count - 1; i >= 0; i--)
		alloc_elem(i, structure, inst, fn, ctx);

	return 0;
}
