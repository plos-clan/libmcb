/* SPDX-License-Identifier: LGPL-3.0-or-later */
#ifndef LIBMCB_TARGET_GNU_ASM_FUNC_H
#define LIBMCB_TARGET_GNU_ASM_FUNC_H
#include <stdbool.h>
#include "gnu_asm.h"
#include "mem.h"
#include "reg.h"
#include "value.h"

struct gnu_asm_func {
	struct gnu_asm_mem_obj **allocated_mem;
	int allocated_mem_count;

	bool allocated_reg[REG_COUNT];
	struct gnu_asm_value *using_reg[REG_COUNT];

	struct text_block **exit_points;
	int exit_points_count;

	struct text_block *beg_blk;
};

#ifdef LIBMCB_STRIP
#define define_func  mcb__gnu_asm_define_func
#define destroy_func mcb__gnu_asm_destroy_func
#endif

int mcb__gnu_asm_define_func(struct mcb_func *fn, struct gnu_asm *ctx);
void mcb__gnu_asm_destroy_func(struct mcb_func *container);

#endif
