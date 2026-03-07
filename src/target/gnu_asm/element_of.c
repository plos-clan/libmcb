/* This file is part of libmcb.
   SPDX-License-Identifier: LGPL-3.0-or-later
*/
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "mcb/func.h"
#include "mcb/inst.h"
#include "mcb/inst/element_of.h"
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

int
build_element_of_inst(struct mcb_inst *inst_outer,
		struct mcb_func *fn,
		struct gnu_asm *ctx)
{
	struct gnu_asm_mem_obj *mem;
	struct gnu_asm_value *result, *container;
	struct mcb_element_of_inst *inst;
	assert(inst_outer && fn && ctx);
	inst = &inst_outer->inner.element_of;
	assert(inst);

	assert(inst->result->data == NULL);
	assert(inst->container->data);

	container = inst->container->data;

	result = ecalloc(1, sizeof(*result));
	result->container = inst->result;
	result->kind = map_type_to_value_kind(
			I8_MEM_VALUE,
			inst->result->type);

	assert(IS_MEM(container->kind));
	mem = ecalloc(1, sizeof(*mem));
	mem->base = container->inner.mem->base;
	mem->kind = VAR_MEM;
	mem->offset = container->inner.mem->offset;
	mem->user = result;

	result->inner.mem = mem;
	inst->result->data = result;
	
	return 0;
}
