/* SPDX-License-Identifier: LGPL-3.0-or-later */
#ifndef LIBMCB_TARGET_GNU_ASM_STRUCT_H
#define LIBMCB_TARGET_GNU_ASM_STRUCT_H
#include "mcb/struct.h"
#include "mcb/context.h"

#include "gnu_asm.h"

struct gnu_asm_struct_value {
	struct gnu_asm_value **values;
};

#ifdef LIBMCB_STRIP
#define destroy_struct_value mcb__gnu_asm_destroy_struct_value
#endif

void mcb__gnu_asm_destroy_struct_value(
		struct mcb_value *container);

#endif
