/* SPDX-License-Identifier: LGPL-3.0-or-later */
#ifndef LIBMCB_TARGET_GNU_ASM_MEM_H
#define LIBMCB_TARGET_GNU_ASM_MEM_H
#include <stdbool.h>
#include "mcb/func.h"

#include "reg.h"

#include "../../str.h"

enum GNU_ASM_MEM_KIND {
	UNUSING_MEM,
	TMP_MEM,
	VAR_MEM
};

struct gnu_asm_mem_obj {
	enum GNU_ASM_REG base;
	int offset;

	enum GNU_ASM_MEM_KIND kind;
	struct gnu_asm_value *user;
};

#ifdef LIBMCB_STRIP
#define alloc_stack_mem mcb__gnu_asm_alloc_stack_mem
#define str_from_mem    mcb__gnu_asm_str_from_mem
#endif

struct gnu_asm_mem_obj *mcb__gnu_asm_alloc_stack_mem(
		int bytes,
		struct gnu_asm_value *user,
		struct mcb_func *fn);

struct str *mcb__gnu_asm_str_from_mem(
		struct str *s,
		const struct gnu_asm_value *v);

#endif
