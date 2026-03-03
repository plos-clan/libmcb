/* This file is part of libmcb.
   SPDX-License-Identifier: LGPL-3.0-or-later
*/
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "mcb/func.h"
#include "mcb/inst.h"
#include "mcb/inst/jmp.h"
#include "mcb/label.h"

#define LIBMCB_STRIP
#include "gen_mov.h"
#include "gnu_asm.h"
#include "inst.h"
#include "label.h"

#include "../../err.h"
#include "../../str.h"
#include "../../text_block.h"

int
build_jmp_inst(struct mcb_inst *inst_outer,
		struct mcb_func *fn,
		struct gnu_asm *ctx)
{
	struct text_block *blk;
	struct mcb_jmp_inst *inst = &inst_outer->inner.jmp;
	int len;
	char *unwarped_label;
	assert(inst && fn && ctx);

	unwarped_label = unwarp_label(inst->label, fn);
	if (!unwarped_label)
		eabort("unwarp_label()");

	estr_clean(&ctx->buf);
	len = snprintf(ctx->buf.s, ctx->buf.siz,
			"jmp %s\n", unwarped_label);
	free(unwarped_label);
	if (len < 0)
		eabort("gen_mov()");
	blk = text_block_from_str(&ctx->buf);
	append_text_block(&ctx->text, blk);

	return 0;
}
