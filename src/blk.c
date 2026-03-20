/* SPDX-License-Identifier: LGPL-3.0-or-later */
#include <stdint.h>
#include <stdlib.h>
#include "mcb/blk.h"
#include "mcb/fn.h"
#include "mcb/inst.h"

#include "darr.h"
#include "ealloc.h"

struct mcb_blk *
mcb_def_blk(struct mcb_fn *fn)
{
	struct mcb_blk *blk = ecalloc(1, sizeof(*blk));
	blk->fn = fn;
	darr_append(fn->blks, fn->nblk, blk);
	return blk;
}

void
mcb_free_blk(struct mcb_blk *blk)
{
	if (!blk)
		return;
	for (uint64_t i = 0; i < blk->ninst; i++)
		mcb_free_inst(blk->insts[i]);
	free(blk);
}
