/* SPDX-License-Identifier: LGPL-3.0-or-later */
#ifndef LIBMCB_AMD64_REG_H
#define LIBMCB_AMD64_REG_H

enum REG {
	RBP, RSP,
	RAX, RBX, RCX, RDX,
	RDI, RSI,
	R8,  R9,  R10, R11, R12, R13, R14, R15,

	NREG
};

static const int reg_alloc_area[] = {
	RAX, RBX, R12, R13, R14, R15,
	RCX, RDX, RDI, RSI, R8,  R9, R10,
	NREG
};

#endif
