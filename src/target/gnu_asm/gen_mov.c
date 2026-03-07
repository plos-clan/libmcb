/* This file is part of libmcb.
   SPDX-License-Identifier: LGPL-3.0-or-later
*/
#include <assert.h>
#include <stdio.h>

#define LIBMCB_STRIP
#include "gen_mov.h"
#include "inst.h"
#include "reg.h"
#include "utils.h"
#include "value.h"

#include "../../err.h"
#include "../../str.h"

static int gen_mov_m2m(struct str *s,
		const struct gnu_asm_value *dst,
		const struct gnu_asm_value *src,
		struct mcb_func *fn,
		struct gnu_asm *ctx);

int
gen_mov_m2m(struct str *s,
		const struct gnu_asm_value *dst,
		const struct gnu_asm_value *src,
		struct mcb_func *fn,
		struct gnu_asm *ctx)
{
	int len;
	struct str dst_str, src_str, tmp_str;
	enum GNU_ASM_REG tmp_reg;
	assert(s && dst && src);

	tmp_reg = alloc_reg(AUTO_ALLOC_REG, NULL, fn);
	if (tmp_reg == REG_COUNT) {
		if (mov_reg_user_to_mem(REG_COUNT - 1, fn, ctx))
			eabort("mov_reg_user_to_mem()");
		drop_reg(REG_COUNT - 1, fn);
		tmp_reg = alloc_reg(REG_COUNT - 1, NULL, fn);
	}

	str_from_reg(&tmp_str, tmp_reg, reg_offset_from_kind(dst->kind));
	str_from_value(&dst_str, dst);
	str_from_value(&src_str, src);

	estr_realloc(s, 16 + dst_str.len + src_str.len + tmp_str.len * 2 + 1);
	len = snprintf(s->s, s->siz, "mov%c %s, %s\nmov%c %s, %s\n",
			get_inst_suffix(dst->kind),
			src_str.s,
			tmp_str.s,
			get_inst_suffix(dst->kind),
			tmp_str.s,
			dst_str.s);
	if (len < 0)
		eabort("snprintf()");
	s->len = len;

	str_free(&dst_str);
	str_free(&src_str);
	str_free(&tmp_str);

	return 0;
}

int
gen_mov(struct str *s,
		const struct gnu_asm_value *dst,
		const struct gnu_asm_value *src,
		struct mcb_func *fn,
		struct gnu_asm *ctx)
{
	int len;
	struct str dst_str, src_str;
	assert(s && dst && src);

	if (IS_IMM(dst->kind))
		ereturn(1, "move to imm");

	if (IS_REG(dst->kind) && IS_REG(src->kind)) {
		if (dst->inner.reg == src->inner.reg)
			return 0;
	}

	if (IS_MEM(dst->kind) && IS_MEM(src->kind))
		return gen_mov_m2m(s, dst, src, fn, ctx);

	str_from_value(&dst_str, dst);
	str_from_value(&src_str, src);

	estr_realloc(s, 16 + dst_str.len + src_str.len + 1);
	len = snprintf(s->s, s->siz, "mov%c %s, %s\n",
			get_inst_suffix(dst->kind),
			src_str.s,
			dst_str.s);
	if (len < 0)
		eabort("snprintf()");
	s->len = len;

	str_free(&dst_str);
	str_free(&src_str);

	return 0;
}
