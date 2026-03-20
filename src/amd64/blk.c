/* SPDX-License-Identifier: LGPL-3.0-or-later */
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include "mcb/amd64_all.h"

#include "../ealloc.h"

int
mcb_amd64_build_blk(struct mcb_amd64_ctx *ctx, struct mcb_blk *blk)
{
	assert(ctx && blk);

	ctx->cur_blk = blk;

	blk->allocated_regs = ecalloc(NREG, sizeof(*blk->allocated_regs));
	blk->using_regs = ecalloc(NREG, sizeof(*blk->using_regs));

	for (uint64_t i = 0; i < blk->ninst; i++) {
		if (mcb_amd64_build_inst(ctx, blk->insts[i]))
			return 1;
	}

	free(blk->allocated_regs);
	free(blk->using_regs);

	return 0;
}
