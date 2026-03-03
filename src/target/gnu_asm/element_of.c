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

#include "../utils.h"
#include "../../ealloc.h"
#include "../../err.h"
#include "../../str.h"
#include "../../text_block.h"

int
build_element_of_inst(struct mcb_inst *inst_outer,
		struct mcb_func *fn,
		struct gnu_asm *ctx)
{
	struct mcb_element_of_inst *inst;
	assert(inst_outer && fn && ctx);
	inst = &inst_outer->inner.element_of;
	assert(inst);
	
	if (mcb_is_inst_unwanted(inst->result, inst_outer))
		return 0;

	return 0;
}
