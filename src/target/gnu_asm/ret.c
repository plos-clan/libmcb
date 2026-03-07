/* This file is part of libmcb.
   SPDX-License-Identifier: LGPL-3.0-or-later
*/
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "mcb/func.h"
#include "mcb/inst.h"
#include "mcb/inst/ret.h"
#include "mcb/value.h"

#define LIBMCB_STRIP
#include "gen_mov.h"
#include "gnu_asm.h"
#include "inst.h"
#include "value.h"

#include "../../err.h"
#include "../../str.h"
#include "../../text_block.h"

int
build_ret_inst(struct mcb_inst *inst_outer,
		struct mcb_func *fn,
		struct gnu_asm *ctx)
{
	struct text_block *blk;
	struct gnu_asm_value dst, *src;
	struct mcb_ret_inst *inst = &inst_outer->inner.ret;
	assert(inst && fn && ctx);
	assert(inst->val);
	assert(inst->val->data);
	src = inst->val->data;

	dst.kind = remap_value_kind(I8_REG_VALUE, src->kind);
	dst.inner.reg = RAX;

	estr_clean(&ctx->buf);
	if (gen_mov(&ctx->buf, &dst, src, fn, ctx))
		eabort("gen_mov()");
	blk = text_block_from_str(&ctx->buf);
	append_text_block(&ctx->text, blk);

	estr_append_cstr(&blk->s, "leave\nret\n");

	return 0;
}
