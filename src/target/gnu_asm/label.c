/* SPDX-License-Identifier: LGPL-3.0-or-later */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mcb/func.h"
#include "mcb/inst.h"
#include "mcb/inst/define_label.h"
#include "mcb/label.h"

#define LIBMCB_STRIP
#include "inst.h"
#include "label.h"

#include "../../err.h"
#include "../../str.h"
#include "../../text_block.h"

int
build_define_label_inst(struct mcb_inst *inst_outer,
		struct mcb_func *fn,
		struct gnu_asm *ctx)
{
	struct text_block *blk;
	struct mcb_define_label_inst *inst;
	struct mcb_label *label;
	char *unwarped_label = NULL;

	assert(inst_outer && fn && ctx);
	inst = &inst_outer->inner.define_label;
	assert(inst);

	label = inst->label;
	assert(label);

	estr_clean(&ctx->buf);
	unwarped_label = unwarp_label(label, fn);
	if (!unwarped_label)
		eabort("unwarp_label()");
	blk = text_block_from_cstr(unwarped_label);
	estr_append_cstr(&blk->s, ":\n");
	append_text_block(&ctx->text, blk);
	free(unwarped_label);

	return 0;
}

void
define_label(struct mcb_label *label,
		struct mcb_func *fn,
		struct gnu_asm *ctx)
{
	struct text_block *blk;
	char *unwarped_label = NULL;
	assert(label && ctx);

	estr_clean(&ctx->buf);
	unwarped_label = unwarp_label(label, fn);
	if (!unwarped_label)
		eabort("unwarp_label()");
	blk = text_block_from_cstr(unwarped_label);
	estr_append_cstr(&blk->s, ":\n");
	append_text_block(&ctx->text, blk);
	free(unwarped_label);
}

char *
unwarp_label(struct mcb_label *label, struct mcb_func *fn)
{
	int len;
	size_t siz;
	char *result;

	assert(label && fn);

	siz = 5 + strlen(fn->name) + strlen(label->name);
	result = malloc(siz);
	len = snprintf(result, siz, ".L%s__%s",
			fn->name, label->name);
	if (len < 0)
		eabort("snprintf()");
	return result;
}
