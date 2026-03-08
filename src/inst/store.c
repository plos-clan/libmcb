/* SPDX-License-Identifier: LGPL-3.0-or-later */
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "mcb/func.h"
#include "mcb/inst/store.h"
#include "mcb/value.h"
#include "utils.h"

#include "../ealloc.h"
#include "../err.h"

int
mcb_inst_store_int(struct mcb_value *container,
		int64_t data,
		struct mcb_func *fn)
{
	struct mcb_inst *inst;
	if (!container || !fn)
		ereturn(1, "!container || !fn");
	inst = ecalloc(1, sizeof(*inst));
	inst->kind = MCB_STORE_INST;
	inst->inner.store.container = container;
	inst->inner.store.kind = MCB_STORE_INT;
	inst->inner.store.operand.i = data;
	if (mcb_use_value(inst, container))
		goto err_free_inst;
	return mcb_append_inst(inst, fn);
err_free_inst:
	free(inst);
	return 1;
}

int
mcb_inst_store_string(struct mcb_value *container,
		const char *str,
		size_t len,
		struct mcb_func *fn)
{
	struct mcb_inst *inst;
	if (!container || !fn)
		ereturn(1, "!container || !fn");
	inst = ecalloc(1, sizeof(*inst));
	inst->kind = MCB_STORE_INST;
	inst->inner.store.container = container;
	inst->inner.store.kind = MCB_STORE_STRING;
	inst->inner.store.operand.str.str = strdup(str);
	if (!inst->inner.store.operand.str.str)
		goto err_free_inst;
	inst->inner.store.operand.str.len = len;
	if (mcb_use_value(inst, container))
		goto err_free_inst;
	if (mcb_append_inst(inst, fn))
		goto err_free_inst;
	return 0;
err_free_inst:
	free(inst);
	return 1;
}

int
mcb_inst_store_uint(struct mcb_value *container,
		uint64_t data,
		struct mcb_func *fn)
{
	struct mcb_inst *inst;
	if (!container || !fn)
		ereturn(1, "!container || !fn");
	inst = ecalloc(1, sizeof(*inst));
	inst->kind = MCB_STORE_INST;
	inst->inner.store.container = container;
	inst->inner.store.kind = MCB_STORE_UINT;
	inst->inner.store.operand.u = data;
	if (mcb_use_value(inst, container))
		goto err_free_inst;
	if (mcb_append_inst(inst, fn))
		goto err_free_inst;
	return 0;
err_free_inst:
	free(inst);
	return 1;
}

int
mcb_inst_store_value(struct mcb_value *container,
		struct mcb_value *data,
		struct mcb_func *fn)
{
	struct mcb_inst *inst;
	if (!container || !data || !fn)
		ereturn(1, "!container || !data || !fn");
	inst = ecalloc(1, sizeof(*inst));
	inst->kind = MCB_STORE_INST;
	inst->inner.store.container = container;
	inst->inner.store.kind = MCB_STORE_VALUE;
	inst->inner.store.operand.value = data;
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
mcb_output_store_inst(const struct mcb_store_inst *inst, FILE *stream)
{
	char *container_str = mcb_build_value_cstr(inst->container);
	switch (inst->kind) {
	case MCB_STORE_INT:
		fprintf(stream, "%s = store %ld\n",
				container_str,
				inst->operand.i);
		break;
	case MCB_STORE_STRING:
		fprintf(stream, "%s = store \"%s\"\n",
				container_str,
				inst->operand.str.str);
		break;
	case MCB_STORE_UINT:
		fprintf(stream, "%s = store %lu\n",
				container_str,
				inst->operand.u);
		break;
	case MCB_STORE_VALUE:
		fprintf(stream, "%s = store %%%s\n",
				container_str,
				inst->operand.value->name);
		break;
	}
	free(container_str);
}
