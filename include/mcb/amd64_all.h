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

static const int reg_alloc_area[] = {
	RAX, RBX, R12, R13, R14, R15,
	RCX, RDX, RDI, RSI, R8,  R9, R10,
	NREG
};

int mcb_amd64_build_blk(struct mcb_amd64_ctx *ctx, struct mcb_blk *blk);
int mcb_amd64_build_fn(struct mcb_amd64_ctx *ctx, struct mcb_fn *fn);
int mcb_amd64_build_inst(struct mcb_amd64_ctx *ctx, struct mcb_inst *inst);
char *mcb_amd64_str_imm(int64_t imm);
const char *mcb_amd64_str_reg(enum REG r, mcb_typ typ);
char *mcb_amd64_str_val(const struct mcb_val *v, mcb_typ typ);

#endif
