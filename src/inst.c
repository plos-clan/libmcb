/* SPDX-License-Identifier: LGPL-3.0-or-later */
#include <assert.h>
#include <stdlib.h>
#include "mcb/blk.h"
#include "mcb/inst.h"
#include "mcb/val.h"

#include "darr.h"
#include "ealloc.h"
#include "err.h"

static void
append_inst(struct mcb_blk *blk, struct mcb_inst *inst)
{
	assert(blk && inst);
	darr_append(blk->insts, blk->ninst, inst);
	mcb_use_val(inst->arg[0], inst);
	mcb_use_val(inst->arg[1], inst);
	mcb_use_val(inst->dst, inst);
}

void
mcb_free_inst(struct mcb_inst *inst)
{
	if (!inst)
		return;
	free(inst);
}

int
mcb_inst(struct mcb_blk *blk,
		enum MCB_INST_OP op,
		uint64_t arg0_typ, struct mcb_val *arg0,
		uint64_t arg1_typ, struct mcb_val *arg1,
		uint64_t dst_typ, struct mcb_val *dst)
{
	struct mcb_inst *inst;
	assert(blk && arg0 && arg1 && dst);
	inst = ecalloc(1, sizeof(*inst));
	inst->op = op;
	inst->typ[0] = arg0_typ;
	inst->typ[1] = arg1_typ;
	inst->typ[2] = dst_typ;
	inst->arg[0] = arg0;
	inst->arg[1] = arg1;
	inst->dst = dst;
	append_inst(blk, inst);
	return 0;
}
