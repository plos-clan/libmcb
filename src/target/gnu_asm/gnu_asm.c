/* This file is part of libmcb.
   SPDX-License-Identifier: LGPL-3.0-or-later
*/
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mcb/func.h"
#include "mcb/inst.h"
#include "mcb/target/gnu_asm.h"
#include "mcb/value.h"

#define LIBMCB_STRIP
#include "func.h"
#include "gnu_asm.h"
#include "reg.h"
#include "value.h"

#include "../../str.h"
#include "../../text_block.h"
#include "../../macros.h"

int
mcb_target_gnu_asm(FILE *stream, struct mcb_context *ctx)
{
	struct text_block *blk;
	struct gnu_asm gnu_asm_ctx = {0};
	int ret = 0;

	if (!stream || !ctx)
		return 1;

	blk = text_block_from_cstr(".text\n");
	append_text_block(&gnu_asm_ctx.text, blk);
	blk = text_block_from_cstr(".section .rodata\n");
	append_text_block(&gnu_asm_ctx.rodata, blk);

	estr_empty(&gnu_asm_ctx.buf);
	estr_expand_siz(&gnu_asm_ctx.buf, 8192);
	gnu_asm_ctx.ctx = ctx;
	gnu_asm_ctx.stream = stream;

	for (size_t i = 0; i < ctx->fn_arr_count; i++) {
		ret = define_func(ctx->fn_arr[i], &gnu_asm_ctx);
		if (ret)
			return 1;
	}

	text_block_for_each(cur, gnu_asm_ctx.text.beg) {
		if (cur->s.s)
			fwrite(cur->s.s, sizeof(*cur->s.s), cur->s.len, stream);
		destroy_text_block(cur);
	}
	text_block_for_each(cur, gnu_asm_ctx.rodata.beg) {
		if (cur->s.s)
			fwrite(cur->s.s, sizeof(*cur->s.s), cur->s.len, stream);
		destroy_text_block(cur);
	}
	fflush(stream);

	for (size_t i = 0; i < ctx->fn_arr_count; i++)
		destroy_func(ctx->fn_arr[i]);

	str_free(&gnu_asm_ctx.buf);

	return 0;
}
