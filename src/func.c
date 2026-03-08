/* SPDX-License-Identifier: LGPL-3.0-or-later */
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "mcb/context.h"
#include "mcb/func.h"
#include "mcb/inst.h"
#include "mcb/label.h"

#include "darr.h"
#include "ealloc.h"
#include "err.h"

struct mcb_func *
mcb_define_func(const char *name,
		const struct mcb_type *type,
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

int
mcb_append_func_arg(struct mcb_value *val, struct mcb_func *fn)
{
	if (!val || !fn)
		ereturn(1, "!val || !fn");
	val->kind = MCB_FUNC_ARG_VALUE;
	darr_append(fn->args, fn->argc, val);
	return 0;
}

void
mcb_destroy_func(struct mcb_func *fn)
{
	if (!fn)
		return;
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
mcb_output_func(const struct mcb_func *fn, FILE *stream)
{
	fprintf(stream, "fn %s():\n", fn->name);
	for (size_t i = 0, label_i = 0; i < fn->inst_arr_count; i++) {
		if (mcb_can_define_label(fn, label_i, i)) {
			mcb_output_label(fn->label_arr[label_i], stream);
			label_i++;
		}

		mcb_output_inst(fn->inst_arr[i], stream);
	}
}
