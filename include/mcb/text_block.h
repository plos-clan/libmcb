/* SPDX-License-Identifier: LGPL-3.0-or-later */
#ifndef LIBMCB_TEXT_BLOCK_H
#define LIBMCB_TEXT_BLOCK_H
#include <stdio.h>
#include "mcb/str.h"

#define mcb_text_block_for_each_guard(CUR, BEGIN, GUARD) \
	for (struct mcb_text_block *CUR = BEGIN, \
			*GUARD; \
			GUARD = CUR ? CUR->nex : NULL, \
			CUR != NULL; \
			CUR = GUARD)

#define mcb_text_block_for_each(CUR, BEGIN) \
	mcb_text_block_for_each_guard(CUR, BEGIN, __text_block_for_each__next__)

#define DEFAULT_TEXT_BLOCK_STR_SIZ 64

struct mcb_text_block {
	struct mcb_text_block *nex, *prv;
	struct str s;
};

struct mcb_text_block_root {
	struct mcb_text_block *beg, *end;
};

void mcb_append_text_block(struct mcb_text_block_root *root,
		struct mcb_text_block *blk);

/* Create a text block and allocate the size of [siz] to [str].
 * @param siz: pass 0 will allocate 64 bytes with [calloc]. */
struct mcb_text_block *mcb_create_text_block(size_t siz);

void mcb_destroy_text_block(struct mcb_text_block *blk);

/** Insert [blk] between [b0] and [b1].
 * @param b0: mustn't be NULL.
 * @param b1: pass NULL will insert between [b0] and the next of [b0]. */
void mcb_insert_text_block(
		struct mcb_text_block_root *root,
		struct mcb_text_block *prv,
		struct mcb_text_block *nex,
		struct mcb_text_block *cur);

void mcb_init_text_block_root(struct mcb_text_block_root *root);

struct mcb_text_block *mcb_text_block_from_str(struct str *s);
struct mcb_text_block *mcb_text_block_from_cstr(const char *s);

#endif
