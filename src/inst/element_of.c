/* SPDX-License-Identifier: LGPL-3.0-or-later */
#include <assert.h>
#include <stdlib.h>
#include "mcb/func.h"
#include "mcb/inst/element_of.h"
#include "mcb/value.h"
#include "utils.h"

#include "../ealloc.h"
#include "../err.h"

int
mcb_inst_element_of(
		struct mcb_value *result,
		struct mcb_value *container,
		struct mcb_value *idx,
		struct mcb_func *fn)
{
	struct mcb_inst *inst;
	if (!result || !container || !idx || !fn)
		ereturn(1, "!result || !container || !idx || !fn");

	inst = ecalloc(1, sizeof(*inst));
	inst->kind = MCB_ELEMENT_OF_INST;
	inst->inner.element_of.result    = result;
	inst->inner.element_of.container = container;
	inst->inner.element_of.idx       = idx;

	result->kind = MCB_VAR_VALUE;

	if (mcb_use_value(inst, result))
		goto err_free_inst;
	if (mcb_use_value(inst, container))
		goto err_free_inst;
	if (mcb_use_value(inst, idx))
		goto err_free_inst;

	if (mcb_append_inst(inst, fn))
		goto err_free_inst;
	return 0;
err_free_inst:
	free(inst);
	return 1;
}

void
mcb_output_element_of_inst(
		const struct mcb_element_of_inst *inst,
		FILE *stream)
{
	char *result_str = mcb_build_value_cstr(inst->result);
	fprintf(stream, "%s = element_of %%%s, %%%s\n",
			result_str,
			inst->container->name,
			inst->idx->name);
	free(result_str);
}
