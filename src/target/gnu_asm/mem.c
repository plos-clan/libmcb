/* SPDX-License-Identifier: LGPL-3.0-or-later */
#include <assert.h>
#include <stdbool.h>
#include "mcb/func.h"

#define LIBMCB_STRIP
#include "func.h"
#include "mem.h"
#include "value.h"

#include "../../darr.h"
#include "../../ealloc.h"
#include "../../str.h"

static struct str *str_from_mem_by_index(
		struct str *s,
		const struct gnu_asm_value *v);

struct str *
str_from_mem_by_index(
		struct str *s,
		const struct gnu_asm_value *v)
{
	int len;
	const struct gnu_asm_mem_obj *mem;
	const char *base_reg_cstr = NULL, *index_reg_cstr = NULL;

	assert(s && v);
	assert(s->siz);
	assert(IS_MEM(v->kind));

	mem = v->inner.mem;
	base_reg_cstr = cstr_from_reg(mem->base, 0);
	index_reg_cstr = cstr_from_reg(mem->index, 0);

	if (mem->offset == 0) {
		len = snprintf(s->s, s->siz, "(%s,%s,%d)",
				base_reg_cstr,
				index_reg_cstr,
				mem->scale);
	} else {
		len = snprintf(s->s, s->siz, "%d(%s,%s,%d)",
				mem->offset,
				base_reg_cstr,
				index_reg_cstr,
				mem->scale);
	}

	if (len < 0)
		return NULL;
	s->len = len;
	return s;
}

struct gnu_asm_mem_obj *
alloc_stack_mem(int bytes,
		struct gnu_asm_value *user,
		struct mcb_func *fn)
{
	struct gnu_asm_func *f;
	struct gnu_asm_mem_obj *last, *mem;
	assert(user && fn);
	f = fn->data;
	assert(f);

	mem = ecalloc(1, sizeof(*mem));
	mem->base = RBP;
	mem->user = user;
	mem->kind = TMP_MEM;
	if (f->allocated_mem_count) {
		last = f->allocated_mem[f->allocated_mem_count - 1];
		mem->offset = last->offset - bytes;
	} else {
		mem->offset -= bytes;
	}

	darr_append(f->allocated_mem, f->allocated_mem_count, mem);
	return mem;
}

void
drop_mem(struct gnu_asm_mem_obj *mem, struct mcb_func *fn)
{
	assert(mem && fn);
	if (mem->scale != 0)
		drop_reg(mem->index, fn);
	if (mem->base != RBP)
		drop_reg(mem->base, fn);
}

struct str *
str_from_mem(struct str *s, const struct gnu_asm_value *v)
{
	int len;
	const struct gnu_asm_mem_obj *mem;
	const char *base_reg_cstr = NULL;

	assert(s && v);
	assert(IS_MEM(v->kind));
	mem = v->inner.mem;
	assert(mem);

	/* register size offset must be 0 in 64bit, %ebp is wrong result */
	base_reg_cstr = cstr_from_reg(mem->base, 0);
	estr_empty(s);
	estr_realloc(s, 32);

	if (mem->scale != 0) {
		if (!str_from_mem_by_index(s, v))
			goto err_free_s;
		return s;
	}

	if (mem->offset == 0) {
		len = snprintf(s->s, s->siz, "(%s)", base_reg_cstr);
	} else {
		len = snprintf(s->s, s->siz, "%d(%s)", mem->offset, base_reg_cstr);
	}

	if (len < 0)
		goto err_free_s;
	s->len = len;

	return s;
err_free_s:
	str_free(s);
	return NULL;
}
