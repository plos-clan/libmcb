/* SPDX-License-Identifier: LGPL-3.0-or-later */
#ifndef LIBMCB_FN_H
#define LIBMCB_FN_H
#include <stdint.h>
#include "mcb/blk.h"
#include "mcb/ctx.h"

struct mcb_fn {
	struct mcb_blk **blks;
	uint64_t nblk;

	struct mcb_val **vals;
	uint64_t nval;
};

struct mcb_fn *mcb_def_fn(struct mcb_ctx *ctx, const char *name);
void mcb_free_fn(struct mcb_fn *fn);

#endif
