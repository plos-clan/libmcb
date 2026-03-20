/* SPDX-License-Identifier: LGPL-3.0-or-later */
#ifndef LIBMCB_AMD64_ALL_H
#define LIBMCB_AMD64_ALL_H
#include "mcb/amd64.h"
#include "mcb/blk.h"
#include "mcb/fn.h"
#include "mcb/inst.h"

enum REG {
	RBP, RSP,
	RAX, RBX, RCX, RDX,
	RDI, RSI,
	R8,  R9,  R10, R11, R12, R13, R14, R15,

	NREG
};

int mcb_amd64_build_blk(struct mcb_amd64_ctx *ctx, struct mcb_blk *blk);
int mcb_amd64_build_fn(struct mcb_amd64_ctx *ctx, struct mcb_fn *fn);
int mcb_amd64_build_inst(struct mcb_amd64_ctx *ctx, struct mcb_inst *inst);
char *mcb_amd64_format_inst(char *buf,
		const char *fmt,
		struct mcb_inst *inst);
const char *mcb_amd64_str_reg(enum REG r, int siz_off);

#endif
