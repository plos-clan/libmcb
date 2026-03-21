/* SPDX-License-Identifier: LGPL-3.0-or-later */
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "mcb/fn.h"
#include "mcb/blk.h"
#include "mcb/val.h"

#include "darr.h"
#include "ealloc.h"
#include "err.h"

struct mcb_fn *
mcb_def_fn(struct mcb_ctx *ctx, const char *name,
		enum MCB_FN_EXPORT_TYPE export_type)
{
	struct mcb_fn *fn = ecalloc(1, sizeof(*fn));
	assert(ctx && name);
	fn->name = strdup(name);
	fn->export_type = export_type;
	darr_append(ctx->fns, ctx->nfn, fn);
	return fn;
}

void
mcb_free_fn(struct mcb_fn *fn)
{
	if (!fn)
		return;
	free(fn->name);
	for (uint64_t i = 0; i < fn->nblk; i++)
		mcb_free_blk(fn->blks[i]);
	for (uint64_t i = 0; i < fn->nval; i++)
		mcb_free_val(fn->vals[i]);
	free(fn);
}
