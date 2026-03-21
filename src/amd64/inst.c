/* SPDX-License-Identifier: LGPL-3.0-or-later */
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "mcb/amd64/all.h"
#include "mcb/reg.h"
#include "mcb/str.h"
#include "mcb/typ.h"
#include "mcb/val.h"

#include "../ealloc.h"
#include "../err.h"
#include "../macros.h"

#include "inst.h"

#define free_if(P) if (P) { free(P); }

#define _X(I,S) ((I) ? S : 0)
#define _XG(B,W,L,Q,O) (\
		_X(B,SIZ8_BIT  << (O)) | \
		_X(W,SIZ16_BIT << (O)) | \
		_X(L,SIZ32_BIT << (O)) | \
		_X(Q,SIZ64_BIT << (O)))
#define _XG0(B,W,L,Q) _XG(B,W,L,Q,0)
#define _XG4(B,W,L,Q) _XG(B,W,L,Q,4)
#define _XG8(B,W,L,Q) _XG(B,W,L,Q,8)

#define END AMD64_INST_VARIANT_END
#define XS0(IB,IW,IL,IQ, MB,MW,ML,MQ, RB,RW,RL,RQ) \
	(_XG8(IB,IW,IL,IQ) | _XG4(MB,MW,ML,MQ) | _XG0(RB,RW,RL,RQ))
#define XS1(MB,MW,ML,MQ, RB,RW,RL,RQ) \
	(_XG4(MB,MW,ML,MQ) | _XG0(RB,RW,RL,RQ))
#define XD(MB,MW,ML,MQ, RB,RW,RL,RQ) \
	(_XG4(MB,MW,ML,MQ) | _XG0(RB,RW,RL,RQ))

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

static const struct amd64_inst *amd64_insts[] = {
	[MCBO_add]  = &amd64_add_inst,
	[MCBO_smul] = &amd64_imul_inst,
	[MCBO_str]  = &amd64_mov_inst,
	NULL
};

static const char suffix[] = {
	[MCB_I8]  = 'b',
	[MCB_I16] = 'w',
	[MCB_I32] = 'l',
	[MCB_I64] = 'q',
	'\0'
};

static enum REG build_imm_src(char **_s, char **_prepare,
		const struct mcb_val *dst,
		struct mcb_val *src,
		mcb_typ typ,
		unsigned int src_bit,
		struct mcb_amd64_ctx *ctx);
static char *build_inst(struct mcb_amd64_ctx *ctx, struct mcb_inst *inst);
static enum REG build_src(char **_s, char **_prepare,
		const struct mcb_val *dst,
		struct mcb_val *src,
		mcb_typ typ,
		unsigned int src_bit,
		struct mcb_amd64_ctx *ctx);
static unsigned int get_operand_flag(
		bool allow_imm,
		mcb_typ typ,
		enum MCB_VAL_KIND val_kind);
static unsigned int get_operand_flag_base(mcb_typ typ);
static const struct amd64_inst_variant *get_variant(
		const struct amd64_inst *inst_def,
		const struct mcb_inst *inst);
static void format_inst(struct str *s,
		const struct amd64_inst *inst_def,
		const struct amd64_inst_variant *variant,
		char *src0, char *src1, char *dst,
		mcb_typ dst_typ);
static void format_operand(const char **_c, struct str *s, char *op);

enum REG
build_imm_src(char **s, char **prepare,
		const struct mcb_val *dst,
		struct mcb_val *src,
		mcb_typ typ,
		unsigned int src_bit,
		struct mcb_amd64_ctx *ctx)
{
	enum REG allocated = NREG, to = NREG;
	struct mcb_inst fake_inst = {0};

	if (src == NULL)
		return NREG;

	assert(s && prepare && dst && src);

	if (src_bit & IMM_BIT) {
		*s = mcb_amd64_str_imm(src->inner.imm);
		return NREG;
	}

	if (dst->kind == MCB_REG_VAL) {
		*s = mcb_amd64_str_val(dst, typ);
		to = dst->inner.reg;
	} else {
		allocated = mcb_alloc_reg(NREG, NREG,
				reg_alloc_area,
				src, ctx->cur_blk);
		*s = strdup(mcb_amd64_str_reg(allocated, typ));
		to = allocated;
	}

	fake_inst.op = MCBO_str;
	fake_inst.typ[0] = typ;
	fake_inst.typ[2] = typ;
	fake_inst.arg[0] = src;
	fake_inst.dst = &(struct mcb_val){
		.kind = MCB_REG_VAL, .inner.reg = to
	};

	*prepare = build_inst(ctx, &fake_inst);

	return allocated;
}

char *
build_inst(struct mcb_amd64_ctx *ctx, struct mcb_inst *inst)
{
	const struct amd64_inst *inst_def;
	struct str result;
	const struct amd64_inst_variant *variant;

	char *src0 = NULL, *src1 = NULL, *dst = NULL;
	char *prepare_src0 = NULL, *prepare_src1 = NULL;

	enum REG src0_reg = NREG, src1_reg = NREG;

	assert(ctx && inst);

	inst_def = amd64_insts[inst->op];
	variant = get_variant(inst_def, inst);
	if (!variant)
		eabort("get_variant()");

	if (inst->dst->kind == MCB_REG_VAL && inst->dst->inner.reg == -1) {
		inst->dst->inner.reg = mcb_alloc_reg(NREG, NREG,
				reg_alloc_area,
				inst->dst, ctx->cur_blk);
	}

	dst = mcb_amd64_str_val(inst->dst, inst->typ[2]);

	src0_reg = build_src(&src0, &prepare_src0,
			inst->dst, inst->arg[0], inst->typ[0],
			variant->src0, ctx);
	src1_reg = build_src(&src1, &prepare_src1,
			inst->dst, inst->arg[1], inst->typ[1],
			variant->src1, ctx);

	str_empty(&result);
	str_append_cstr(&result, prepare_src1);
	format_inst(&result, inst_def, variant,
			src0, src1, dst, inst->typ[2]);

	if (src0_reg != NREG)
		mcb_drop_reg(src0_reg, ctx->cur_blk);
	if (src1_reg != NREG)
		mcb_drop_reg(src1_reg, ctx->cur_blk);

	if (inst->arg[0] && inst->arg[0]->last_user == inst)
		mcb_drop_val(inst->arg[0], ctx->cur_blk);
	if (inst->arg[1] && inst->arg[1]->last_user == inst)
		mcb_drop_val(inst->arg[1], ctx->cur_blk);

	free_if(src0);
	free_if(src1);
	free_if(dst);
	free_if(prepare_src0);
	free_if(prepare_src1);

	return result.s;
}

enum REG
build_src(char **s, char **prepare,
		const struct mcb_val *dst,
		struct mcb_val *src,
		mcb_typ typ,
		unsigned int src_bit,
		struct mcb_amd64_ctx *ctx)
{
	if (src == NULL)
		return NREG;

	assert(s && prepare && dst && src);

	switch (src->kind) {
	case MCB_IMM_VAL:
		return build_imm_src(s, prepare, dst, src, typ,
				src_bit, ctx);
	case MCB_REG_VAL:
		*s = mcb_amd64_str_val(src, typ);
		break;
	default:
		assert(0);
		break;
	}

	return NREG;
}

void
format_inst(struct str *s,
		const struct amd64_inst *inst_def,
		const struct amd64_inst_variant *variant,
		char *src0, char *src1, char *dst,
		mcb_typ dst_typ)
{
	const char *c;
	char *sc;

	assert(s && inst_def && variant);

	c = inst_def->fmt;
	if (*c == '+')
		c++;
	for (; *c; c++) {
		if (*c != '%') {
			estr_append_chr(s, *c);
			continue;
		}
		c++;
		switch (*c) {
		case 's':
			estr_append_chr(s, suffix[dst_typ]);
			break;
		case '0': format_operand(&c, s, src0); break;
		case '1': format_operand(&c, s, src1); break;
		case '=': format_operand(&c, s, dst);  break;
		case '%':
			estr_append_chr(s, *c);
			break;
		}
	}

	for (sc = s->s + s->len - 1; isspace(*sc); sc--);
	if (*sc == ',')
		s->len = sc - s->s;
	estr_append_chr(s, '\n');
}

unsigned int
get_operand_flag(bool allow_imm, mcb_typ typ, enum MCB_VAL_KIND val_kind)
{
	unsigned int base = get_operand_flag_base(typ);
	switch (val_kind) {
	case MCB_IMM_VAL: return base << (allow_imm ? 8 : 0); break;
	case MCB_REG_VAL: return base; break;
	default:
		break;
	}
	return 0;
}

unsigned int
get_operand_flag_base(mcb_typ typ)
{
	switch (typ) {
	case MCB_I8:  return SIZ8_BIT;  break;
	case MCB_I16: return SIZ16_BIT; break;
	case MCB_I32: return SIZ32_BIT; break;
	case MCB_I64: return SIZ64_BIT; break;
	default:
		break;
	}
	return 0;
}

const struct amd64_inst_variant *
get_variant(const struct amd64_inst *inst_def, const struct mcb_inst *inst)
{
	unsigned int flag0, flag1, flag2;
	mcb_typ src0_typ;
	const struct amd64_inst_variant *variant = NULL;

	if (inst->arg[0]) {
		src0_typ = inst->typ[0];
		if (inst->arg[0]->kind == MCB_IMM_VAL)
			src0_typ = mcb_get_typ_of_imm(inst->arg[0]->inner.imm);
		flag0 = get_operand_flag(true, src0_typ, inst->arg[0]->kind);
	}

	if (inst->arg[1])
		flag1 = get_operand_flag(false, inst->typ[1], inst->arg[1]->kind);
	if (inst->dst)
		flag2 = get_operand_flag(false, inst->typ[2], inst->dst->kind);

	for (variant = inst_def->variants; !is_end_variant(*variant); variant++) {
		if (inst->arg[0] && variant->src0 != 0 && !(flag0 & variant->src0))
			continue;
		if (inst->arg[1] && variant->src1 != 0 && !(flag1 & variant->src1))
			continue;
		if (inst->dst && variant->dst != 0 && !(flag2 & variant->dst))
			continue;
		return variant;
	}

	return NULL;
}

void
format_operand(const char **_c, struct str *s, char *op)
{
	const char *c;
	assert(_c && s);
	c = *_c;

	if (op) {
		str_append_cstr(s, op);
		return;
	}

	for (; *c && *c != ','; c++);
	if (*c == '\0')
		c--;

	*_c = c;
}

int
mcb_amd64_build_inst(struct mcb_amd64_ctx *ctx, struct mcb_inst *inst)
{
	struct mcb_text_block *blk;
	char *s;
	assert(ctx && inst);
	s = build_inst(ctx, inst);
	blk = mcb_text_block_from_cstr(s);
	mcb_append_text_block(&ctx->text, blk);
	free(s);
	return 0;
}
