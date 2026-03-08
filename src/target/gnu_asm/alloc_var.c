/* SPDX-License-Identifier: LGPL-3.0-or-later */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "mcb/func.h"
#include "mcb/inst.h"
#include "mcb/inst/alloc_var.h"
#include "mcb/value.h"

#define LIBMCB_STRIP
#include "gnu_asm.h"
#include "inst.h"
#include "mem.h"
#include "value.h"

#include "../../ealloc.h"
#include "../../err.h"
#include "../../str.h"
#include "../../text_block.h"

int
build_alloc_var_inst(struct mcb_inst *inst_outer,
		struct mcb_func *fn,
		struct gnu_asm *ctx)
{
	int bytes;
	struct mcb_alloc_var_inst *inst;
	struct gnu_asm_value *val;

	assert(inst_outer && fn && ctx);
	inst = &inst_outer->inner.alloc_var;
	assert(inst);
	assert(inst->container);

	val = ecalloc(1, sizeof(*val));
	val->container = inst->container;
	val->kind = map_type_to_value_kind(I8_MEM_VALUE, inst->container->type);
	bytes = map_value_kind_to_bytes(val->kind);
	val->inner.mem = alloc_stack_mem(bytes, val, fn);
	if (!val->inner.mem)
		eabort("alloc_stack_mem()");
	val->inner.mem->kind = VAR_MEM;
	inst->container->data = val;

	return 0;
}
