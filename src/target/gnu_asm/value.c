/* This file is part of libmcb.
   SPDX-License-Identifier: LGPL-3.0-or-later
*/
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "mcb/func.h"
#include "mcb/value.h"

#define LIBMCB_STRIP
#include "reg.h"
#include "struct.h"
#include "value.h"
#include "value_kind.h"

#include "../../err.h"
#include "../../str.h"

void
destroy_value(struct mcb_value *container)
{
	struct gnu_asm_value *val;
	if (!container)
		return;

	if (container->type->builtin == MCB_STRUCT) {
		destroy_struct_value(container);
		goto end;
	}

	if (container->type->builtin == MCB_ARRAY)
		return;

	val = container->data;
	if (!val)
		return;
	if (IS_MEM(val->kind) || val->kind == STRUCT_VALUE)
		free(val->inner.mem);
end:
	free(container->data);
	container->data = NULL;
}

void
drop_value(struct mcb_value *val, struct mcb_func *fn)
{
	struct gnu_asm_value *gval;
	assert(val);
	gval = val->data;
	if (!gval)
		return;

	if (IS_REG(gval->kind))
		drop_reg(gval->inner.reg, fn);
}

enum GNU_ASM_VALUE_KIND
map_bytes_to_value_kind(enum GNU_ASM_VALUE_KIND base, int bytes)
{
	assert(base != UNKOWN_VALUE);
	assert(bytes != -1);
	switch (bytes) {
	case 1: return base;
	case 2: return base + 1;
	case 4: return base + 2;
	case 8: return base + 3;
	}
	eabort("map_bytes_to_value_kind()");
	return UNKOWN_VALUE;
}

enum GNU_ASM_VALUE_KIND
map_type_to_value_kind(enum GNU_ASM_VALUE_KIND base, const struct mcb_type *t)
{
	assert(base != UNKOWN_VALUE);
	switch (t->builtin) {
	case MCB_U8:  case MCB_I8:  return base;
	case MCB_U16: case MCB_I16: return base + 1;
	case MCB_U32: case MCB_I32: return base + 2;
	case MCB_U64: case MCB_I64: return base + 3;
	case MCB_PTR:
	case MCB_STRING:
	case MCB_ARRAY:
		return base + 3;
	case MCB_STRUCT:
		eabort("unexpected type 'MCB_STRUCT'");
		break;
	case MCB_CMP_RESULT:
		eabort("unexpected type 'MCB_CMP_RESULT'");
		break;
	}
	eabort("map_type_to_value_kind()");
	return UNKOWN_VALUE;
}

int
map_value_kind_to_bytes(enum GNU_ASM_VALUE_KIND kind)
{
	switch (kind) {
	case UNKOWN_VALUE: return -1;
	CASE_I8_VALUE:     return 1;
	CASE_I16_VALUE:    return 2;
	CASE_I32_VALUE:    return 4;
	CASE_I64_VALUE:    return 8;
	case CMP_RESULT_VALUE:
		eabort("unexpected dst_kind 'CMP_RESULT_VALUE'");
		break;
	case STRUCT_VALUE:
		eabort("unexpected dst_kind 'STRUCT_VALUE'");
		break;
	}
	eabort("map_value_kind_to_bytes()");
	return -1;
}

enum GNU_ASM_VALUE_KIND
remap_value_kind(
		enum GNU_ASM_VALUE_KIND base,
		enum GNU_ASM_VALUE_KIND kind)
{
	int bytes = map_value_kind_to_bytes(kind);
	return map_bytes_to_value_kind(base, bytes);
}

struct str *
str_from_imm(struct str *s, const struct gnu_asm_value *v)
{
	int ret = 0;
	assert(s && v);
	estr_empty(s);
	estr_realloc(s, 32);
	if (IS_INT_IMM(v->kind)) {
		ret = snprintf(s->s, s->siz, "$%ld", v->inner.imm.i);
		if (ret < 0)
			goto err;
		s->len = ret;
		return s;
	}
err:
	eabort("str_from_imm()");
	return NULL;
}

struct str *
str_from_value(struct str *s, const struct gnu_asm_value *v)
{
	int reg_off;
	assert(s && v);
	switch (v->kind) {
	case UNKOWN_VALUE:
		return NULL;
	CASE_DATA_VALUE:
		return str_from_data(s, v);
	CASE_IMM_VALUE:
		return str_from_imm(s, v);
	CASE_MEM_VALUE:
		return str_from_mem(s, v);
	CASE_REG_VALUE:
		reg_off = reg_offset_from_kind(v->kind);
		assert(reg_off != -1);
		return str_from_reg(s, v->inner.reg, reg_off);
	case CMP_RESULT_VALUE:
		eabort("unexpected dst_kind 'CMP_RESULT_VALUE'");
		break;
	case STRUCT_VALUE:
		eabort("unexpected dst_kind 'STRUCT_VALUE'");
		break;
	}
	eabort("str_from_value()");
	return NULL;
}
