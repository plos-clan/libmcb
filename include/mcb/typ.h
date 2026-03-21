/* SPDX-License-Identifier: LGPL-3.0-or-later */
#ifndef LIBMCB_TYPE_H
#define LIBMCB_TYPE_H
#include <stdint.h>

enum {
	/* just integer, no signed or unsigned */
	MCB_I8,
	MCB_I16,
	MCB_I32,
	MCB_I64
};

typedef uint64_t mcb_typ;

mcb_typ mcb_get_typ_of_imm(uint64_t imm);

#endif
