/* This file is part of libmcb.
   SPDX-License-Identifier: LGPL-3.0-or-later
*/
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "mcb/context.h"
#include "mcb/func.h"
#include "mcb/struct.h"
#include "mcb/value.h"

#include "darr.h"
#include "ealloc.h"
#include "err.h"
#include "inst/utils.h"
#include "str.h"

struct mcb_struct *
mcb_declare_struct(const char *name, struct mcb_context *ctx)
{
	struct mcb_struct *structure;
	if (!name)
		ereturn(NULL, "!name");
	structure = ecalloc(1, sizeof(*structure));
	structure->name = strdup(name);
	if (!structure->name)
		goto err_free_structure;
	darr_append(ctx->struct_arr, ctx->struct_arr_count, structure);
	return structure;
err_free_structure:
	free(structure);
	return NULL;
}

int
mcb_declare_struct_elem(
		const char *name,
		enum MCB_TYPE type,
		struct mcb_struct *structure)
{
	struct mcb_struct_elem *elem;
	if (!name || !structure)
		ereturn(-1, "!name || !structure");
	elem = ecalloc(1, sizeof(*elem));
	elem->name = strdup(name);
	if (!elem->name)
		goto err_dup_name;
	elem->type = type;
	darr_append(structure->elems, structure->elems_count, elem);
	return structure->elems_count - 1;
err_dup_name:
	free(elem);
	ereturn(-1, "strdup(name)");
}

struct mcb_value *
mcb_define_struct_value(
		const char *name,
		struct mcb_struct *structure,
		struct mcb_func *fn)
{
	struct mcb_value *result;
	if (!name || !structure)
		ereturn(NULL, "!name || !structure");
	result = mcb_define_value(name, MCB_STRUCT, fn);
	result->kind = MCB_STRUCT_VALUE;
	result->inner.structure.structure = structure;
	result->inner.structure.values = ecalloc(
			structure->elems_count,
			sizeof(*result->inner.structure.values));
	return result;
}

void
mcb_destroy_struct(struct mcb_struct *structure)
{
	if (!structure)
		return;
	for (int i = 0; i < structure->elems_count; i++)
		mcb_destroy_struct_elem(structure->elems[i]);
	if (structure->elems)
		free(structure->elems);
	free(structure->name);
	free(structure);
}

void
mcb_destroy_struct_elem(struct mcb_struct_elem *elem)
{
	if (!elem)
		return;
	free(elem->name);
	free(elem);
}

struct mcb_value *
mcb_get_value_from_struct(
		struct mcb_value *container,
		struct mcb_struct *structure,
		int idx,
		struct mcb_func *fn)
{
	struct mcb_struct_elem *elem;
	int len;
	struct str name;
	struct mcb_value *value;

	if (!container || !fn)
		ereturn(NULL, "!container || !fn");
	if (container->kind != MCB_STRUCT_VALUE)
		ereturn(NULL, "container isn't MCB_STRUCT_VALUE");
	if (idx >= structure->elems_count)
		ereturnf(NULL, "no such element in struct by index %d", idx);

	elem = structure->elems[idx];
	assert(elem);

	estr_empty(&name);
	value = ecalloc(1, sizeof(*value));
	value->kind = MCB_STRUCT_ELEM_VALUE;
	value->type = container->type;
	value->inner.structure_elem.structure = structure;
	value->inner.structure_elem.structure_container = container;
	value->inner.structure_elem.idx = idx;
	estr_realloc(&name,
			strlen(container->name) +
			strlen(elem->name) + 2);
	len = snprintf(name.s, name.siz, "%s.%s", container->name, elem->name);
	if (len < 0)
		eabort("snprintf()");
	value->name = name.s;
	container->inner.structure.values[idx] = value;

	return value;
}
