/* SPDX-License-Identifier: LGPL-3.0-or-later */
#include <assert.h>
#include <stdint.h>
#include "mcb/amd64/all.h"

static struct mcb_text_block *
build_enter(struct mcb_amd64_ctx *ctx, struct mcb_fn *fn)
{
	struct mcb_text_block *blk;
	int len = 0;

	assert(ctx && fn);

	if (fn->export_type == MCB_EXPORT_FN)
		len = snprintf(ctx->buf, BUFSIZ, ".globl %s\n", fn->name);
	snprintf(&ctx->buf[len], BUFSIZ - len,
			"%s:\n"
			"pushq %%rbp\n"
			"movq %%rsp, %%rbp\n", fn->name);

	blk = mcb_text_block_from_cstr(ctx->buf);
	mcb_append_text_block(&ctx->text, blk);

	return blk;
}

static struct mcb_text_block *
build_exit(struct mcb_amd64_ctx *ctx)
{
	struct mcb_text_block *blk;

	assert(ctx);

	blk = mcb_text_block_from_cstr("leave\nret\n");
	mcb_append_text_block(&ctx->text, blk);

	return blk;
}

int
mcb_amd64_build_fn(struct mcb_amd64_ctx *ctx, struct mcb_fn *fn)
{
	struct mcb_text_block *enter_blk;

	assert(ctx && fn);

	enter_blk = build_enter(ctx, fn);

	for (uint64_t i = 0; i < fn->nblk; i++) {
		if (mcb_amd64_build_blk(ctx, fn->blks[i]))
			return 1;
	}

	build_exit(ctx);

	return 0;
}
