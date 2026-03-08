/* SPDX-License-Identifier: LGPL-3.0-or-later */
#ifndef LIBMCB_TARGET_GNU_ASM_UTILS_H
#define LIBMCB_TARGET_GNU_ASM_UTILS_H
#include "mcb/func.h"

#include "gnu_asm.h"
#include "reg.h"

#ifdef LIBMCB_STRIP
#define mov_reg_user        mcb__gnu_asm_mov_reg_user
#define mov_reg_user_to_mem mcb__gnu_asm_mov_reg_user_to_mem
#endif

/* Move the user of [reg] to another register or memory. */
int mcb__gnu_asm_mov_reg_user(
		enum GNU_ASM_REG reg,
		struct mcb_func *fn,
		struct gnu_asm *ctx);

/* Like mov_reg_user(), but move the user of [reg] to memory. */
int mcb__gnu_asm_mov_reg_user_to_mem(
		enum GNU_ASM_REG reg,
		struct mcb_func *fn,
		struct gnu_asm *ctx);

#endif
