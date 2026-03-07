/* This file is part of libmcb.
   SPDX-License-Identifier: LGPL-3.0-or-later
*/
#ifndef LIBMCB_TARGET_GNU_ASM_GEN_MOV_H
#define LIBMCB_TARGET_GNU_ASM_GEN_MOV_H
#include "mcb/func.h"

#include "gnu_asm.h"
#include "value.h"
#include "../../str.h"

#ifdef LIBMCB_STRIP
#define gen_mov mcb__gnu_asm_gen_mov
#endif

int mcb__gnu_asm_gen_mov(struct str *s,
		const struct gnu_asm_value *dst,
		const struct gnu_asm_value *src,
		struct mcb_func *fn,
		struct gnu_asm *ctx);

#endif
