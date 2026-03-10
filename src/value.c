/* SPDX-License-Identifier: LGPL-3.0-or-later */
#include <assert.h>
#include <string.h>
#include "mcb/func.h"
#include "mcb/struct.h"
#include "mcb/type.h"
#include "mcb/value.h"

#include "darr.h"
#include "ealloc.h"
#include "err.h"
#include "str.h"

static char *auto_name(struct mcb_func *fn);

char *
auto_name(struct mcb_func *fn)
{
	char *name;
	assert(fn);

	name = ecalloc(32, sizeof(*name));
	snprintf(name, 32, "%lu", fn->value_auto_named);
	fn->value_auto_named++;

	return name;
}

char *
mcb_build_value_cstr(const struct mcb_value *value)
{
	struct str s;
	char *type;
	if (!value)
		return NULL;
	type = mcb_build_type_cstr(value->type);
	estr_empty(&s);
	estr_expand_siz(&s, strlen(value->name) + strlen(type) + 3);
	snprintf(s.s, s.siz, "%%%s:%s", value->name, type);
	free(type);
	return s.s;
}

struct mcb_value *
mcb_define_value(const char *name,
		const struct mcb_type *type,
		struct mcb_func *fn)
{
	struct mcb_value *val;
	if (!type || !fn)
		ereturn(NULL, "!type || !fn");
	val = ecalloc(1, sizeof(*val));
	val->kind = MCB_NORMAL_VALUE;

	if (name)
		val->name = strdup(name);
	else
		val->name = auto_name(fn);
	if (!val->name)
		goto err_null_name;

	val->type = type;
	darr_append(fn->value_arr, fn->value_arr_count, val);
	return val;
err_null_name:
	free(val);
	ereturn(NULL, "strdup(name)");
}

void
mcb_free_value(struct mcb_value *val)
{
	if (!val)
		return;
	if (val->type->builtin == MCB_ARRAY)
		free(val->inner.array.elems);
	free(val->name);
	free(val);
}
