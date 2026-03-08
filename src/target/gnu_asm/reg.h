/* SPDX-License-Identifier: LGPL-3.0-or-later */
#ifndef LIBMCB_TARGET_GNU_ASM_REG_H
#define LIBMCB_TARGET_GNU_ASM_REG_H
#include "mcb/func.h"
#include "value_kind.h"

#define AUTO_ALLOC_REG REG_COUNT

enum GNU_ASM_REG {
	RBP, RSP,
	RAX, RBX, RCX, RDX,
	RDI, RSI,
	R8,  R9,  R10, R11, R12, R13, R14, R15,
	REG_COUNT
};

struct gnu_asm_value;

#ifdef LIBMCB_STRIP
#define alloc_reg            mcb__gnu_asm_alloc_reg
#define cstr_from_reg        mcb__gnu_asm_cstr_from_reg
#define drop_reg             mcb__gnu_asm_drop_reg
#define reg_offset_from_kind mcb__gnu_asm_reg_offset_from_kind
#define str_from_reg         mcb__gnu_asm_str_from_reg
#endif

enum GNU_ASM_REG mcb__gnu_asm_alloc_reg(
		enum GNU_ASM_REG reg,
		struct gnu_asm_value *user,
		struct mcb_func *fn);
const char *mcb__gnu_asm_cstr_from_reg(enum GNU_ASM_REG r, int siz);
void mcb__gnu_asm_drop_reg(enum GNU_ASM_REG reg, struct mcb_func *fn);
int mcb__gnu_asm_reg_offset_from_kind(enum GNU_ASM_VALUE_KIND kind);
struct str *mcb__gnu_asm_str_from_reg(struct str *s,
		enum GNU_ASM_REG r,
		int siz);

#endif
