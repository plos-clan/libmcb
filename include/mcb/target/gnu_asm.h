/* SPDX-License-Identifier: LGPL-3.0-or-later */
#ifndef LIBMCB_TARGET_GNU_ASM_H
#define LIBMCB_TARGET_GNU_ASM_H
#include <stdio.h>
#include "mcb/context.h"

int mcb_target_gnu_asm(FILE *stream, struct mcb_context *ctx);

#endif
