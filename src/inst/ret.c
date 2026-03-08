/* SPDX-License-Identifier: LGPL-3.0-or-later */
#include <assert.h>
#include <stdlib.h>
#include "mcb/func.h"
#include "mcb/inst/ret.h"
#include "mcb/value.h"
#include "utils.h"

#include "../ealloc.h"
#include "../err.h"

int
mcb_inst_ret(struct mcb_value *val, struct mcb_func *fn)
{
	struct mcb_inst *inst;
	if (!val || !fn)
		ereturn(1, "!val || !fn");
	inst = ecalloc(1, sizeof(*inst));
	inst->kind = MCB_RET_INST;
	inst->inner.ret.val = val;
	if (mcb_use_value(inst, val))
		goto err_free_inst;
	if (mcb_append_inst(inst, fn))
		goto err_free_inst;
	return 0;
err_free_inst:
	free(inst);
	return 1;
}

void
mcb_output_ret_inst(const struct mcb_ret_inst *inst, FILE *stream)
{
	fprintf(stream, "ret %%%s\n", inst->val->name);
}
