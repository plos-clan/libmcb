/* SPDX-License-Identifier: LGPL-3.0-or-later */
#include <assert.h>
#include <stdlib.h>
#include "mcb/func.h"
#include "mcb/inst/load.h"
#include "mcb/value.h"
#include "utils.h"

#include "../ealloc.h"
#include "../err.h"

int
mcb_inst_load(struct mcb_value *result,
		struct mcb_value *address,
		struct mcb_func *fn)
{
	struct mcb_inst *inst;
	if (!result || !address || !fn)
		ereturn(1, "!result || !address || !fn");
	if (address->type->builtin != MCB_PTR)
		ereturn(1, "address not pointer");
	inst = ecalloc(1, sizeof(*inst));
	inst->kind = MCB_LOAD_INST;
	inst->inner.load.result = result;
	inst->inner.load.address = address;
	if (mcb_use_value(inst, result))
		goto err_free_inst;
	if (mcb_use_value(inst, address))
		goto err_free_inst;
	if (mcb_append_inst(inst, fn))
		goto err_free_inst;
	return 0;
err_free_inst:
	free(inst);
	return 1;
}

void
mcb_output_load_inst(const struct mcb_load_inst *inst, FILE *stream)
{
	char *result_str = mcb_build_value_cstr(inst->result);
	fprintf(stream, "%s = load %%%s\n",
			result_str,
			inst->address->name);
	free(result_str);
}
