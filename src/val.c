/* SPDX-License-Identifier: LGPL-3.0-or-later */
#include <assert.h>
#include <stdlib.h>
#include "mcb/fn.h"
#include "mcb/val.h"

#include "darr.h"
#include "ealloc.h"
#include "err.h"

struct mcb_val *
mcb_def_imm(struct mcb_fn *fn, int64_t imm)
{
	struct mcb_val *val = mcb_def_val(fn);
	assert(val);
	val->inner.imm = imm;
	val->kind = MCB_IMM_VAL;
	return val;
}

struct mcb_val *
mcb_def_val(struct mcb_fn *fn)
{
	struct mcb_val *val = ecalloc(1, sizeof(*val));
	assert(fn);
	darr_append(fn->vals, fn->nval, val);
	return val;
}

void
mcb_free_val(struct mcb_val *val)
{
	if (!val)
		return;
	free(val);
}

void
mcb_use_val(struct mcb_val *val, struct mcb_inst *user)
{
	assert(val && user);
	val->last_user = user;
}
