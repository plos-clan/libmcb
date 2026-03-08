/* SPDX-License-Identifier: LGPL-3.0-or-later */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mcb/func.h"
#include "mcb/inst/alloc_array.h"
#include "mcb/value.h"
#include "utils.h"

#include "../ealloc.h"
#include "../err.h"

static struct mcb_value *alloc_elem(
		struct mcb_value *container,
		size_t idx,
		struct mcb_func *fn);

struct mcb_value *
alloc_elem(struct mcb_value *container, size_t idx, struct mcb_func *fn)
{
	char *buf;
	struct mcb_value *elem;
	int len;
	size_t siz;
	assert(fn);

	siz = strlen(container->name) + 32;
	buf = ecalloc(siz, sizeof(*buf));
	len = snprintf(buf, siz, "%s.%lu", container->name, idx);
	if (len < 0)
		eabort("snprintf()");
	elem = mcb_define_value(buf, container->type->inner, fn);

	return elem;
}

int
mcb_inst_alloc_array(
		struct mcb_value *container,
		size_t size,
		struct mcb_func *fn)
{
	struct mcb_inst *inst;
	if (!container || !fn)
		ereturn(1, "!container || !fn");
	if (container->type->builtin != MCB_ARRAY)
		ereturn(1, "container not a array");

	container->inner.array.elems =
		ecalloc(size, sizeof(*container->inner.array.elems));
	container->inner.array.size = size;
	for (size_t i = 0; i < size; i++)
		container->inner.array.elems[i] =
			alloc_elem(container, i, fn);

	inst = ecalloc(1, sizeof(*inst));
	inst->kind = MCB_ALLOC_ARRAY_INST;
	inst->inner.alloc_array.container = container;
	inst->inner.alloc_array.size      = size;
	if (mcb_use_value(inst, container))
		goto err_free_inst;
	if (mcb_append_inst(inst, fn))
		goto err_free_inst;

	return 0;
err_free_inst:
	free(inst);
	return 1;
}

void
mcb_output_alloc_array_inst(const struct mcb_alloc_array_inst *inst, FILE *stream)
{
	char *container_str = mcb_build_value_cstr(inst->container);
	fprintf(stream, "%s = alloc_array %lu\n",
			container_str,
			inst->size);
	free(container_str);
}
