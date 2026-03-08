/* SPDX-License-Identifier: LGPL-3.0-or-later */
#include <assert.h>
#include <stdlib.h>
#include "mcb/func.h"
#include "mcb/inst/div.h"
#include "mcb/value.h"
#include "utils.h"

#include "../ealloc.h"
#include "../err.h"

int
mcb_inst_div(struct mcb_value *result,
		struct mcb_value *rem,
		struct mcb_value *lhs,
		struct mcb_value *rhs,
		struct mcb_func *fn)
{
	struct mcb_inst *inst;
	if (!result || !lhs || !rhs || !rem || !fn)
		ereturn(1, "!result || !lhs || !rhs || !rem || !fn");
	inst = ecalloc(1, sizeof(*inst));
	inst->kind = MCB_DIV_INST;
	inst->inner.div.result = result;
	inst->inner.div.rem = rem;
	inst->inner.div.lhs = lhs;
	inst->inner.div.rhs = rhs;
	if (mcb_use_value(inst, result))
		goto err_free_inst;
	if (mcb_use_value(inst, rem))
		goto err_free_inst;
	if (mcb_use_value(inst, lhs))
		goto err_free_inst;
	if (mcb_use_value(inst, rhs))
		goto err_free_inst;
	if (mcb_append_inst(inst, fn))
		goto err_free_inst;
	return 0;
err_free_inst:
	free(inst);
	return 1;
}

void
mcb_output_div_inst(const struct mcb_div_inst *inst, FILE *stream)
{
	char *result_str = mcb_build_value_cstr(inst->result);
	fprintf(stream, "%s = div %%%s, %%%s, %%%s\n",
			result_str,
			inst->rem->name,
			inst->lhs->name,
			inst->rhs->name);
	free(result_str);
}
