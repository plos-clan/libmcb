/* SPDX-License-Identifier: LGPL-3.0-or-later */
#include <stdbool.h>
#include <stdlib.h>
#include "mcb/inst.h"

void
mcb_force_gen_inst(struct mcb_func *fn)
{
	struct mcb_inst *inst;
	if (!fn)
		return;
	if (!fn->inst_arr)
		return;
	inst = fn->inst_arr[fn->inst_arr_count - 1];
	if (!inst)
		return;
	inst->force_gen = true;
}

void
mcb_destroy_inst(struct mcb_inst *inst)
{
	if (!inst)
		return;
	switch (inst->kind) {
	case MCB_CALL_INST:
		free(inst->inner.call.args);
		break;
	default:
		break;
	}
	free(inst);
}
