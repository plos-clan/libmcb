/* This file is part of libmcb.
   SPDX-License-Identifier: LGPL-3.0-or-later
*/
#include <assert.h>
#include "mcb/func.h"

#define LIBMCB_STRIP
#include "func.h"
#include "gen_mov.h"
#include "gnu_asm.h"
#include "reg.h"
#include "utils.h"
#include "value.h"

#include "../../err.h"
#include "../../str.h"
#include "../../text_block.h"

static int mov_mem_reg_user(
		enum GNU_ASM_REG reg,
		struct mcb_func *fn,
		struct gnu_asm *ctx);

int
mov_mem_reg_user(
		enum GNU_ASM_REG reg,
		struct mcb_func *fn,
		struct gnu_asm *ctx)
{
	struct text_block *blk;
	struct gnu_asm_func *f;
	struct gnu_asm_value dst, src;
	struct gnu_asm_value *user;
	assert(fn && ctx);
	f = fn->data;
	assert(f);
	user = f->allocated_reg[reg];
	assert(user);

	src.kind = remap_value_kind(I8_REG_VALUE, user->kind);
	src.inner.reg = user->inner.mem->base;
	dst.kind = src.kind;
	dst.inner.reg = alloc_reg(AUTO_ALLOC_REG, user, fn);
	user->inner.mem->base = dst.inner.reg;
	if (user->inner.mem->base == REG_COUNT)
		eabort("user->inner.mem.base == REG_COUNT");

	estr_clean(&ctx->buf);
	if (gen_mov(&ctx->buf, &dst, &src, fn, ctx))
		ereturn(1, "gen_mov()");
	blk = text_block_from_str(&ctx->buf);
	append_text_block(&ctx->text, blk);

	return 0;
}

int
mov_reg_user(enum GNU_ASM_REG reg, struct mcb_func *fn, struct gnu_asm *ctx)
{
	struct text_block *blk;
	struct gnu_asm_func *f;
	struct gnu_asm_value src;
	struct gnu_asm_value *user;
	assert(fn && ctx);
	f = fn->data;
	assert(f);
	user = f->allocated_reg[reg];
	assert(user);

	if (IS_MEM(user->kind))
		return mov_mem_reg_user(reg, fn, ctx);
	assert(IS_REG(user->kind));

	src = *user;
	user->inner.reg = alloc_reg(AUTO_ALLOC_REG, user, fn);
	if (user->inner.reg == REG_COUNT)
		return mov_reg_user_to_mem(reg, fn, ctx);

	estr_clean(&ctx->buf);
	if (gen_mov(&ctx->buf, user, &src, fn, ctx))
		ereturn(1, "gen_mov()");
	blk = text_block_from_str(&ctx->buf);
	append_text_block(&ctx->text, blk);

	return 0;
}

int
mov_reg_user_to_mem(
		enum GNU_ASM_REG reg,
		struct mcb_func *fn,
		struct gnu_asm *ctx)
{
	struct text_block *blk;
	struct gnu_asm_func *f;
	struct gnu_asm_value src;
	struct gnu_asm_value *user;
	assert(fn && ctx);
	f = fn->data;
	assert(f);
	user = f->allocated_reg[reg];
	assert(user);

	assert(IS_REG(user->kind));

	src = *user;
	user->kind = remap_value_kind(I8_MEM_VALUE, user->kind);
	user->inner.mem = alloc_stack_mem(map_value_kind_to_bytes(user->kind), user, fn);
	if (!user->inner.mem)
		eabort("alloc_stack_mem()");
	estr_clean(&ctx->buf);
	if (gen_mov(&ctx->buf, user, &src, fn, ctx))
		ereturn(1, "gen_mov()");
	blk = text_block_from_str(&ctx->buf);
	append_text_block(&ctx->text, blk);

	return 0;
}
