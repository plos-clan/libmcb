/* SPDX-License-Identifier: LGPL-3.0-or-later */
#ifndef LIBMCB_SRC_TARGET_GNU_ASM_H
#define LIBMCB_SRC_TARGET_GNU_ASM_H
#include <stdio.h>
#include "mcb/context.h"

#include "data.h"

#include "../../str.h"
#include "../../text_block.h"

struct gnu_asm {
	struct str buf;
	struct mcb_context *ctx;
	FILE *stream;

	struct text_block_root text;
	struct text_block_root rodata;

	struct gnu_asm_data_ctx data_ctx;
};

#endif
