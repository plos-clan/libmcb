/* SPDX-License-Identifier: LGPL-3.0-or-later */
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include "mcb/amd64.h"
#include "mcb/amd64/all.h"

#include "../ealloc.h"

struct mcb_amd64_ctx *
mcb_amd64_build(struct mcb_ctx *ctx)
{
	struct mcb_amd64_ctx *amd64 = ecalloc(1, sizeof(*amd64));
	assert(ctx);

	for (uint64_t i = 0; i < ctx->nfn; i++) {
		if (mcb_amd64_build_fn(amd64, ctx->fns[i]))
			goto err_build_fn;
	}

	return amd64;
err_build_fn:
	mcb_amd64_free_ctx(amd64);
	return NULL;
}

void
mcb_amd64_free_ctx(struct mcb_amd64_ctx *ctx)
{
	if (!ctx)
		return;
	mcb_text_block_for_each(cur, ctx->text.beg)
		free(cur);
	free(ctx);
}

void
mcb_amd64_output(FILE *to, const struct mcb_amd64_ctx *ctx)
{
	assert(to && ctx);
	mcb_text_block_for_each(cur, ctx->text.beg)
		fputs(cur->s.s, to);
}
