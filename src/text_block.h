/* SPDX-License-Identifier: LGPL-3.0-or-later */
#ifndef LIBMCB_TEXT_BLOCK_H
#define LIBMCB_TEXT_BLOCK_H
#include <stdio.h>
#include "str.h"

#define text_block_for_each_guard(CUR, BEGIN, GUARD) \
	for (struct text_block *CUR = BEGIN, \
			*GUARD; \
			GUARD = CUR ? CUR->nex : NULL, \
			CUR != NULL; \
			CUR = GUARD)

#define text_block_for_each(CUR, BEGIN) \
	text_block_for_each_guard(CUR, BEGIN, __text_block_for_each__next__)

#define DEFAULT_TEXT_BLOCK_STR_SIZ 64

struct text_block {
	struct text_block *nex, *prv;
	struct str s;
};

struct text_block_root {
	struct text_block *beg, *end;
};

#ifdef LIBMCB_STRIP
#define append_text_block    mcb__append_text_block
#define create_text_block    mcb__create_text_block
#define destroy_text_block   mcb__destroy_text_block
#define insert_text_block    mcb__insert_text_block
#define init_text_block_root mcb__init_text_block_root
#define text_block_from_str  mcb__text_block_from_str
#define text_block_from_cstr mcb__text_block_from_cstr
#endif

void mcb__append_text_block(struct text_block_root *root,
		struct text_block *blk);

/**
 * Create a text block and allocate the size of `siz` to `str`.
 * @param siz: pass 0 will allocate 64 bytes with `calloc`.
 */
struct text_block *mcb__create_text_block(size_t siz);

void mcb__destroy_text_block(struct text_block *blk);

/**
 * Insert `blk` between `b0` and `b1`.
 * @param b0: mustn't be NULL.
 * @param b1: pass NULL will insert between `b0` and the next of `b0`.
 */
void mcb__insert_text_block(
		struct text_block_root *root,
		struct text_block *prv,
		struct text_block *nex,
		struct text_block *cur);

void mcb__init_text_block_root(struct text_block_root *root);

struct text_block *mcb__text_block_from_str(struct str *s);
struct text_block *mcb__text_block_from_cstr(const char *s);

#endif
