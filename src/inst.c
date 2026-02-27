/* This file is part of libmcb.
   SPDX-License-Identifier: LGPL-3.0-or-later
*/
#include <stdlib.h>
#include "mcb/inst.h"

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
