/* SPDX-License-Identifier: LGPL-3.0-or-later */
#include <stdint.h>
#include <stdlib.h>
#include "mcb/ctx.h"
#include "mcb/fn.h"

#include "ealloc.h"

struct mcb_ctx *
mcb_def_ctx(void)
{
	struct mcb_ctx *ctx = ecalloc(1, sizeof(*ctx));
	return ctx;
}

void
mcb_free_ctx(struct mcb_ctx *ctx)
{
	if (!ctx)
		return;
	for (uint64_t i = 0; i < ctx->nfn; i++)
		mcb_free_fn(ctx->fns[i]);
	free(ctx);
}
