/* SPDX-License-Identifier: LGPL-3.0-or-later */
#ifndef LIBMCB_TARGET_GNU_ASM_LABEL_H
#define LIBMCB_TARGET_GNU_ASM_LABEL_H
#include "mcb/func.h"
#include "mcb/label.h"
#include "gnu_asm.h"

#ifdef LIBMCB_STRIP
#define unwarp_label mcb__gnu_asm_unwarp_label
#endif

char *mcb__gnu_asm_unwarp_label(
		struct mcb_label *label,
		struct mcb_func *fn);

#endif
