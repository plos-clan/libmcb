/* SPDX-License-Identifier: LGPL-3.0-or-later */
#include <assert.h>
#include <stdint.h>
#include "mcb/amd64.h"
#include "mcb/amd64_all.h"

#include "../ealloc.h"

int
mcb_amd64_build(struct mcb_ctx *ctx)
{
	struct mcb_amd64_ctx *amd64 = ecalloc(1, sizeof(*amd64));
	assert(ctx);
	for (uint64_t i = 0; i < ctx->nfn; i++) {
		if (mcb_amd64_build_fn(amd64, ctx->fns[i]))
			return 1;
	}
	return 0;
}
