/* SPDX-License-Identifier: LGPL-3.0-or-later */
#include <assert.h>
#include <stdint.h>
#include "mcb/amd64_all.h"

int
mcb_amd64_build_fn(struct mcb_amd64_ctx *ctx, struct mcb_fn *fn)
{
	assert(ctx && fn);
	for (uint64_t i = 0; i < fn->nblk; i++) {
		if (mcb_amd64_build_blk(ctx, fn->blks[i]))
			return 1;
	}
	return 0;
}
