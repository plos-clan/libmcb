/* SPDX-License-Identifier: LGPL-3.0-or-later */
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "mcb/func.h"
#include "mcb/inst/call.h"
#include "mcb/value.h"

#include "utils.h"

#include "../ealloc.h"
#include "../err.h"

static int check_arg(const struct mcb_func *callee,
		const struct mcb_value *arg,
		int idx);
static int check_argc(const struct mcb_func *callee, int argc);

int
check_arg(const struct mcb_func *callee,
		const struct mcb_value *arg,
		int idx)
{
	assert(callee);
	if (arg == NULL)
		ereturn(1, "args[i] == NULL");
	if (arg->type->builtin != callee->args[idx]->type->builtin)
		ereturnf(1, "pass different type of arugment to function '%s'",
				callee->name);
	return 0;
}

int
check_argc(const struct mcb_func *callee, int argc)
{
	assert(callee);
	if (argc > callee->argc)
		ereturnf(1, "pass too many arguments to function '%s'",
				callee->name);
	if (argc < callee->argc)
		ereturnf(1, "pass too few arguments to function '%s'",
				callee->name);
	return 0;
}

int
mcb_inst_call(struct mcb_value *result,
		const struct mcb_func *callee,
		int argc,
		struct mcb_value **args,
		struct mcb_func *fn)
{
	struct mcb_inst *inst;

	if (!result || !callee || !args || !fn)
		ereturn(1, "!result || !callee || !args || !fn");
	if (check_argc(callee, argc))
		return 1;

	inst = ecalloc(1, sizeof(*inst));
	for (int i = 0; i < argc; i++) {
		if (check_arg(callee, args[i], i))
			goto err_free_inst;
		if (mcb_use_value(inst, args[i]))
			goto err_free_inst;
	}

	inst->kind = MCB_CALL_INST;
	inst->inner.call.result = result;
	inst->inner.call.callee = callee;
	inst->inner.call.argc = argc;
	inst->inner.call.args = malloc(argc * sizeof(*inst->inner.call.args));
	memcpy(inst->inner.call.args, args, argc * sizeof(*inst->inner.call.args));

	if (mcb_use_value(inst, result))
		goto err_free_inst_and_args;

	if (mcb_append_inst(inst, fn))
		goto err_free_inst_and_args;
	return 0;
err_free_inst_and_args:
	free(inst->inner.call.args);
err_free_inst:
	free(inst);
	return 1;
}

void
mcb_output_call_inst(const struct mcb_call_inst *inst, FILE *stream)
{
	char *result_str = mcb_build_value_cstr(inst->result);
	fprintf(stream, "%s = call %s\n",
			result_str,
			inst->callee->name);
	free(result_str);
}
