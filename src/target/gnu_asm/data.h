/* SPDX-License-Identifier: LGPL-3.0-or-later */
#ifndef LIBMCB_TARGET_GNU_ASM_DATA_H
#define LIBMCB_TARGET_GNU_ASM_DATA_H
#include <stddef.h>
#include "../../str.h"

struct gnu_asm;
struct gnu_asm_value;

enum GNU_ASM_DATA_KIND {
	DARA,
	RODATA
};

enum GNU_ASM_DATA_TYPE {
	STRING_DATA
};

struct gnu_asm_data_ctx {
	struct gnu_asm_data_obj **objs;
	size_t count;
};

struct gnu_asm_data_obj {
	size_t idx;
	enum GNU_ASM_DATA_KIND kind;
	enum GNU_ASM_DATA_TYPE type;
	struct gnu_asm_value *user;
};

#ifdef LIBMCB_STRIP
#define alloc_str_data   mcb__gnu_asm_alloc_str_data
#define str_from_data    mcb__gnu_asm_str_from_data
#endif

struct gnu_asm_data_obj *mcb__gnu_asm_alloc_str_data(
		const char *cstr,
		struct gnu_asm_value *user,
		struct gnu_asm *ctx);

struct str *mcb__gnu_asm_str_from_data(
		struct str *s,
		const struct gnu_asm_value *v);

#endif
