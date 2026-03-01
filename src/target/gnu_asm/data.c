/* SPDX-License-Identifier: LGPL-3.0-or-later */
#include <assert.h>
#define LIBMCB_STRIP
#include "data.h"
#include "gnu_asm.h"
#include "value.h"

#include "../../darr.h"
#include "../../ealloc.h"
#include "../../err.h"

#define LC_TEMP ".LC%lu"

static struct gnu_asm_data_obj *alloc_data(
		enum GNU_ASM_DATA_KIND kind,
		enum GNU_ASM_DATA_TYPE type,
		struct gnu_asm_value *user,
		struct gnu_asm *ctx);

struct gnu_asm_data_obj *
alloc_data(
		enum GNU_ASM_DATA_KIND kind,
		enum GNU_ASM_DATA_TYPE type,
		struct gnu_asm_value *user,
		struct gnu_asm *ctx)
{
	struct gnu_asm_data_obj *obj;
	assert(user && ctx);
	obj = ecalloc(1, sizeof(*obj));
	obj->idx = ctx->data_ctx.count;
	obj->kind = kind;
	obj->type = type;
	darr_append(ctx->data_ctx.objs, ctx->data_ctx.count, obj);
	return obj;
}

struct gnu_asm_data_obj *
alloc_str_data(
		const char *cstr,
		struct gnu_asm_value *user,
		struct gnu_asm *ctx)
{
	struct text_block *blk;
	int len;
	struct gnu_asm_data_obj *obj;
	assert(cstr && user && ctx);
	obj = alloc_data(RODATA, STRING_DATA, user, ctx);
	estr_clean(&ctx->buf);
	len = snprintf(ctx->buf.s, ctx->buf.siz,
			LC_TEMP":\n.string \"%s\"\n",
			obj->idx,
			cstr);
	if (len < 0)
		eabort("snprintf()");
	ctx->buf.len = len;
	blk = text_block_from_str(&ctx->buf);
	append_text_block(&ctx->rodata, blk);
	return obj;
}

struct str *
str_from_data(
		struct str *s,
		const struct gnu_asm_value *v)
{
	int len;
	estr_empty(s);
	estr_realloc(s, 32);
	len = snprintf(s->s, s->siz, "$"LC_TEMP, v->inner.data->idx);
	if (len < 0)
		eabort("snprintf()");
	s->len = len;
	return s;
}
