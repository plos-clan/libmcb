/* SPDX-License-Identifier: LGPL-3.0-or-later */
#include <assert.h>
#include <stdlib.h>
#include "mcb/func.h"
#include "mcb/inst/branch.h"
#include "mcb/label.h"
#include "mcb/value.h"
#include "utils.h"

#include "../ealloc.h"
#include "../err.h"

int
mcb_inst_branch(struct mcb_value *cmp_result,
		struct mcb_label *on_true,
		struct mcb_label *on_false,
		struct mcb_func *fn)
{
	struct mcb_inst *inst;
	if (!cmp_result || !on_true || !on_false || !fn)
		ereturn(1, "!cmp_result || !on_true || !on_false || !fn");
	inst = ecalloc(1, sizeof(*inst));
	inst->kind = MCB_BRANCH_INST;
	inst->inner.branch.cmp_result = cmp_result;
	inst->inner.branch.on_true    = on_true;
	inst->inner.branch.on_false   = on_false;
	if (mcb_use_value(inst, cmp_result))
		goto err_free_inst;
	if (mcb_append_inst(inst, fn))
		goto err_free_inst;
	return 0;
err_free_inst:
	free(inst);
	return 1;
}

void
mcb_output_branch_inst(const struct mcb_branch_inst *inst, FILE *stream)
{
	fprintf(stream, "branch %%%s, @%s, @%s\n",
			inst->cmp_result->name,
			inst->on_true->name,
			inst->on_false->name);
}
