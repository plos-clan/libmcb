/* SPDX-License-Identifier: LGPL-3.0-or-later */
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "mcb/amd64_all.h"
#include "mcb/reg.h"
#include "mcb/str.h"
#include "mcb/type.h"
#include "mcb/val.h"

#include "../ealloc.h"
#include "../err.h"
#include "../macros.h"

#define SIZ8_OFF  0
#define SIZ16_OFF 1
#define SIZ32_OFF 2
#define SIZ64_OFF 3

enum {
	FROM_IMM8  = 1 << SIZ8_OFF,
	FROM_IMM16 = 1 << SIZ16_OFF,
	FROM_IMM32 = 1 << SIZ32_OFF,
	FROM_IMM64 = 1 << SIZ64_OFF,
	FROM_MEM8  = 1 << SIZ8_OFF  << 4,
	FROM_MEM16 = 1 << SIZ16_OFF << 4,
	FROM_MEM32 = 1 << SIZ32_OFF << 4,
	FROM_MEM64 = 1 << SIZ64_OFF << 4,
	FROM_REG8  = 1 << SIZ8_OFF  << 8,
	FROM_REG16 = 1 << SIZ16_OFF << 8,
	FROM_REG32 = 1 << SIZ32_OFF << 8,
	FROM_REG64 = 1 << SIZ64_OFF << 8
};

struct amd64_inst_variant {
	unsigned int src0:12, src1:8, dst:8;
};

struct amd64_inst {
	const char *fmt;
	struct amd64_inst_variant *variants;
};

#define _X(I,S) ((I) ? S : 0)
#define _XG(B,W,L,Q) (\
		_X(B,FROM_IMM8)  | \
		_X(W,FROM_IMM16) | \
		_X(L,FROM_IMM32) | \
		_X(Q,FROM_IMM64))

#define END {0,0,0}
#define XS0(IB,IW,IL,IQ, MB,MW,ML,MQ, RB,RW,RL,RQ) \
	(_XG(IB,IW,IL,IQ) | _XG(MB,MW,ML,MQ) | _XG(RB,RW,RL,RQ))
#define XS1(MB,MW,ML,MQ, RB,RW,RL,RQ) \
	(_XG(MB,MW,ML,MQ) | _XG(RB,RW,RL,RQ))
#define XD(MB,MW,ML,MQ, RB,RW,RL,RQ) \
	(_XG(MB,MW,ML,MQ) | _XG(RB,RW,RL,RQ))

/* bwlq: byte, word, long, quadruple word
 * i: imm
 * m: mem
 * r: reg */
#include "inst.def.h"

#undef _X
#undef _XG

#undef END
#undef XS0
#undef XS1
#undef XD

static const char suffix[] = {
	[MCB_I8]  = 'b',
	[MCB_I16] = 'w',
	[MCB_I32] = 'l',
	[MCB_I64] = 'q',
	'\0'
};

int
mcb_amd64_build_inst(struct mcb_amd64_ctx *ctx, struct mcb_inst *inst)
{
	assert(ctx && inst);

	return 0;
}

char *
mcb_amd64_format_inst(char *buf,
		const char *fmt,
		struct mcb_inst *inst)
{
	return NULL;
}
