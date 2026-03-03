/* This file is part of libmcb.
   SPDX-License-Identifier: LGPL-3.0-or-later
*/
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "mcb/array.h"
#include "mcb/func.h"
#include "mcb/inst.h"
#include "mcb/inst/alloc_array.h"
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

static void alloc_elem(size_t idx,
		struct mcb_alloc_array_inst *inst,
		struct mcb_func *fn,
		struct gnu_asm *ctx);

void
alloc_elem(size_t idx,
		struct mcb_alloc_array_inst *inst,
		struct mcb_func *fn,
		struct gnu_asm *ctx)
{
	struct mcb_array_value *array_value;
	int bytes;
	struct gnu_asm_value *val;
	struct mcb_value *val_container;
	assert(inst && fn && ctx);
	array_value = &inst->container->inner.array;
	val_container = array_value->elems[idx];

	val = ecalloc(1, sizeof(*val));
	val->container = val_container;
	val->kind = map_type_to_value_kind(
			I8_MEM_VALUE,
			array_value->elems[idx]->type);
	bytes = map_value_kind_to_bytes(val->kind);
	val->inner.mem = alloc_stack_mem(bytes, val, fn);
	if (!val->inner.mem)
		eabort("alloc_stack_mem()");
	if (val_container)
		val_container->data = val;
}

int
build_alloc_array_inst(
		struct mcb_inst *inst_outer,
		struct mcb_func *fn,
		struct gnu_asm *ctx)
{
	struct mcb_alloc_array_inst *inst;

	assert(inst_outer && fn && ctx);
	inst = &inst_outer->inner.alloc_array;
	assert(inst);
	assert(inst->container);
	assert(inst->container->type->builtin == MCB_ARRAY);

	if (mcb_is_inst_unwanted(inst->container, inst_outer))
		return 0;

	for (size_t i = inst->container->inner.array.size - 1;
			i > 0; i--)
		alloc_elem(i, inst, fn, ctx);
	alloc_elem(0, inst, fn, ctx);

	return 0;
}
