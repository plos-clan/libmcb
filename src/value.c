/* This file is part of libmcb.
   SPDX-License-Identifier: LGPL-3.0-or-later
*/
#include <assert.h>
#include <string.h>
#include "mcb/func.h"
#include "mcb/struct.h"
#include "mcb/value.h"

#include "darr.h"
#include "ealloc.h"
#include "err.h"

struct mcb_value *
mcb_define_value(const char *name,
		enum MCB_TYPE type,
		struct mcb_func *fn)
{
	struct mcb_value *val;
	if (!name || !fn)
		ereturn(NULL, "!val || !name || !fn");
	val = ecalloc(1, sizeof(*val));
	val->kind = MCB_NORMAL_VALUE;
	val->name = strdup(name);
	if (!val->name)
		goto err_null_name;
	val->type = type;
	if (type == MCB_STRUCT)
		val->kind = MCB_STRUCT_VALUE;
	darr_append(fn->value_arr, fn->value_arr_count, val);
	return val;
err_null_name:
	free(val);
	ereturn(NULL, "strdup(name)");
}

struct mcb_value *
mcb_define_value_from_func_arg(
		const char *name,
		struct mcb_func_arg *func_arg,
		struct mcb_func *fn)
{
	struct mcb_value *val;
	if (!name || !func_arg || !fn)
		ereturn(NULL, "!name || !func_arg || !fn");
	val = ecalloc(1, sizeof(*val));
	val->inner.func_arg = func_arg;
	val->kind = MCB_FUNC_ARG_VALUE;
	val->name = strdup(name);
	if (!val->name)
		goto err_null_name;
	val->type = func_arg->type;
	darr_append(fn->value_arr, fn->value_arr_count, val);
	func_arg->val_link = val;
	return val;
err_null_name:
	free(val);
	ereturn(NULL, "strdup(name)");
}

void
mcb_destroy_value(struct mcb_value *val)
{
	if (!val)
		return;
	if (val->kind == MCB_STRUCT_VALUE) {
		for (int i = 0; i < val->inner.structure
				.structure->elems_count; i++)
			mcb_destroy_value(val->inner.structure.values[i]);
		free(val->inner.structure.values);
	}
	free(val->name);
	free(val);
}
