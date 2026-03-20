/* SPDX-License-Identifier: LGPL-3.0-or-later */
#ifndef LIBMCB_BLOCK_H
#define LIBMCB_BLOCK_H
#include <stdbool.h>
#include <stdint.h>

struct mcb_inst;
struct mcb_blk {
	bool *allocated_regs;
	struct mcb_val **using_regs;

	struct mcb_fn *fn;

	struct mcb_inst **insts;
	uint64_t ninst;
};

struct mcb_blk *mcb_def_blk(struct mcb_fn *fn);
void mcb_free_blk(struct mcb_blk *blk);

#endif
