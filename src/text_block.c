/* SPDX-License-Identifier: LGPL-3.0-or-later */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define LIBMCB_STRIP
#include "ealloc.h"
#include "str.h"
#include "text_block.h"

void
append_text_block(
		struct text_block_root *root,
		struct text_block *blk)
{
	assert(root && blk);
	blk->nex = NULL;
	blk->prv = root->end;
	if (blk->prv) {
		blk->prv->nex = blk;
	} else {
		root->beg = blk;
	}
	root->end = blk;
}

struct text_block *
create_text_block(size_t siz)
{
	struct text_block *res = ecalloc(1, sizeof(*res));
	if (siz == 0)
		siz = DEFAULT_TEXT_BLOCK_STR_SIZ;
	estr_realloc(&res->s, siz);
	return res;
}

void
destroy_text_block(struct text_block *blk)
{
	if (!blk)
		return;
	str_free(&blk->s);
	free(blk);
}

void
insert_text_block(
		struct text_block_root *root,
		struct text_block *prv,
		struct text_block *nex,
		struct text_block *cur)
{
	assert(root && prv && cur);

	if (prv == root->end) {
		assert(nex == root->end->nex);
		root->end = cur;
	}

	if (prv) {
		prv->nex = cur;
	} else {
		root->beg = cur;
	}

	if (nex) {
		nex->prv = cur;
	} else {
		root->end = cur;
	}

	cur->prv = prv;
	cur->nex = nex;
}

void
init_text_block_root(struct text_block_root *root)
{
	assert(root);
	root->beg = NULL;
	root->end = NULL;
}

struct text_block *
text_block_from_str(struct str *s)
{
	assert(s);
	struct text_block *blk = ecalloc(1, sizeof(*blk));
	estr_from_cstr(&blk->s, s->s);
	return blk;
}

struct text_block *
mcb__text_block_from_cstr(const char *s)
{
	assert(s);
	struct text_block *blk = ecalloc(1, sizeof(*blk));
	estr_from_cstr(&blk->s, s);
	return blk;
}
