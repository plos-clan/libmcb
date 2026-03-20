/* SPDX-License-Identifier: LGPL-3.0-or-later */
#ifndef LIBMCB_AMD64_H
#define LIBMCB_AMD64_H
#include <stddef.h>
#include <stdint.h>
#include "mcb/ctx.h"
#include "mcb/text_block.h"

struct mcb_amd64_ctx {
	char buf[BUFSIZ];

	struct mcb_blk *cur_blk;

	struct mcb_ctx *mcb;

	struct mcb_text_block_root rodata;
	struct mcb_text_block_root text;

	uint64_t nlabel;
	uint64_t nrodata;
};

int mcb_amd64_build(struct mcb_ctx *ctx);

#endif
