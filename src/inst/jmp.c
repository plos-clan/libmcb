/* This file is part of libmcb.
   SPDX-License-Identifier: LGPL-3.0-or-later
*/
#include <assert.h>
#include <stdlib.h>
#include "mcb/func.h"
#include "mcb/inst/jmp.h"
#include "mcb/label.h"
#include "utils.h"

#include "../ealloc.h"
#include "../err.h"

int
mcb_inst_jmp(struct mcb_label *label, struct mcb_func *fn)
{
	struct mcb_inst *inst;
	if (!label || !fn)
		ereturn(1, "!label || !fn");
	inst = ecalloc(1, sizeof(*inst));
	inst->kind = MCB_JMP_INST;
	inst->inner.jmp.label = label;
	if (mcb_append_inst(inst, fn))
		goto err_free_inst;
	return 0;
err_free_inst:
	free(inst);
	return 1;
}
