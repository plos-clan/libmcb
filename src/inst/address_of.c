/* SPDX-License-Identifier: LGPL-3.0-or-later */
#include <assert.h>
#include <stdlib.h>
#include "mcb/func.h"
#include "mcb/inst/address_of.h"
#include "mcb/value.h"
#include "utils.h"

#include "../ealloc.h"
#include "../err.h"

int
mcb_inst_address_of(
		struct mcb_value *result,
		struct mcb_value *val,
		struct mcb_func *fn)
{
	struct mcb_inst *inst;
	if (!result || !val || !fn)
		ereturn(1, "!result || !val || !fn");
	inst = ecalloc(1, sizeof(*inst));
	inst->kind = MCB_ADDRESS_OF_INST;
	inst->inner.address_of.result = result;
	inst->inner.address_of.val = val;
	if (mcb_use_value(inst, result))
		goto err_free_inst;
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
mcb_output_address_of_inst(const struct mcb_address_of_inst *inst, FILE *stream)
{
	char *result_str = mcb_build_value_cstr(inst->result);
	fprintf(stream, "%s = address_of %%%s\n",
			result_str,
			inst->val->name);
	free(result_str);
}
