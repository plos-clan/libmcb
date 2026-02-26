/* This file is part of libmcb.
   SPDX-License-Identifier: LGPL-3.0-or-later
*/
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "mcb/context.h"
#include "mcb/func.h"
#include "mcb/inst.h"

#include "darr.h"
#include "ealloc.h"
#include "err.h"

struct mcb_func *
mcb_define_func(const char *name,
		enum MCB_TYPE type,
		enum MCB_FUNC_EXPORT_TYPE export_type,
		struct mcb_context *ctx)
{
	struct mcb_func *fn;
	if (!name || !ctx)
		ereturn(NULL, "!name || !ctx");
	fn = ecalloc(1, sizeof(*fn));
	fn->name = strdup(name);
	if (!fn->name)
		goto err_null_name;
	fn->export_type = export_type;
	fn->type = type;
	darr_append(ctx->fn_arr, ctx->fn_arr_count, fn);
	return fn;
err_null_name:
	free(fn);
	ereturn(NULL, "strdup(name)");
}

struct mcb_func_arg *
mcb_define_func_arg(const char *name,
		enum MCB_TYPE type,
		struct mcb_func *fn)
{
	struct mcb_func_arg *arg;
	if (!name || !fn)
		ereturn(NULL, "!name || !fn");
	arg = ecalloc(1, sizeof(*arg));
	arg->name = strdup(name);
	if (!arg->name)
		goto err_null_name;
	arg->type = type;
	darr_append(fn->args, fn->argc, arg);
	return arg;
err_null_name:
	free(arg);
	ereturn(NULL, "strdup(name)");
}

void
mcb_destroy_func(struct mcb_func *fn)
{
	if (!fn)
		return;
	for (int i = 0; i < fn->argc; i++)
		mcb_destroy_func_arg(fn->args[i]);
	free(fn->args);

	for (size_t i = 0; i < fn->inst_arr_count; i++)
		mcb_destroy_inst(fn->inst_arr[i]);
	free(fn->inst_arr);

	for (size_t i = 0; i < fn->label_arr_count; i++)
		mcb_destroy_label(fn->label_arr[i]);
	free(fn->label_arr);

	for (size_t i = 0; i < fn->value_arr_count; i++)
		mcb_destroy_value(fn->value_arr[i]);
	free(fn->value_arr);

	free(fn->name);
	free(fn);
}

void
mcb_destroy_func_arg(struct mcb_func_arg *arg)
{
	if (!arg)
		return;
	free(arg->name);
	free(arg);
}
