/* SPDX-License-Identifier: LGPL-3.0-or-later */
#ifndef LIBMCB_AMD64_INST_H
#define LIBMCB_AMD64_INST_H
#include "mcb/amd64/reg.h"

#define SIZ8_OFF  0
#define SIZ16_OFF 1
#define SIZ32_OFF 2
#define SIZ64_OFF 3

#define USE_DST_SET (NREG + 1)

enum {
	SIZ8_BIT  = 1 << SIZ8_OFF,
	SIZ16_BIT = 1 << SIZ16_OFF,
	SIZ32_BIT = 1 << SIZ32_OFF,
	SIZ64_BIT = 1 << SIZ64_OFF,

	IMM8_BIT  = SIZ8_BIT  << 8,
	IMM16_BIT = SIZ16_BIT << 8,
	IMM32_BIT = SIZ32_BIT << 8,
	IMM64_BIT = SIZ64_BIT << 8,
	MEM8_BIT  = SIZ8_BIT  << 4,
	MEM16_BIT = SIZ16_BIT << 4,
	MEM32_BIT = SIZ32_BIT << 4,
	MEM64_BIT = SIZ64_BIT << 4,
	REG8_BIT  = SIZ8_BIT,
	REG16_BIT = SIZ16_BIT,
	REG32_BIT = SIZ32_BIT,
	REG64_BIT = SIZ64_BIT,

	IMM_BIT = IMM8_BIT | IMM16_BIT | IMM32_BIT | IMM64_BIT,
	MEM_BIT = MEM8_BIT | MEM16_BIT | MEM32_BIT | MEM64_BIT,
	REG_BIT = REG8_BIT | REG16_BIT | REG32_BIT | REG64_BIT
};

#define AMD64_INST_VARIANT_END {0,0,0,{NREG,NREG},{NREG,NREG},{NREG,NREG}}
#define is_end_variant(V) ((V).src0 == 0 && (V).src1 == 0 && (V).dst == 0)
struct amd64_inst_variant {
	unsigned int src0:12, src1:12, dst:8;
	enum REG src0_reg[2];
	enum REG src1_reg[2];
	enum REG dst_reg[2];
};

struct amd64_inst {
	const char *fmt;
	const struct amd64_inst_variant *variants;
};

#endif
