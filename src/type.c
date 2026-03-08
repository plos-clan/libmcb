/* SPDX-License-Identifier: LGPL-3.0-or-later */
#include <stddef.h>
#include "mcb/type.h"

#include "str.h"

static const char *builtin_type_cstr[] = {
	[MCB_U8]         = "u8",
	[MCB_U16]        = "u16",
	[MCB_U32]        = "u32",
	[MCB_U64]        = "u64",
	[MCB_I8]         = "i8",
	[MCB_I16]        = "i16",
	[MCB_I32]        = "i32",
	[MCB_I64]        = "i64",
	[MCB_PTR]        = "ptr",
	[MCB_STRING]     = "string",
	[MCB_ARRAY]      = "array",
	[MCB_CMP_RESULT] = "cmp_result",
	[MCB_STRUCT]     = "struct",
};

static const struct mcb_type butiltin_types[] = {
	[MCB_U8]         = {MCB_U8},
	[MCB_U16]        = {MCB_U16},
	[MCB_U32]        = {MCB_U32},
	[MCB_U64]        = {MCB_U64},
	[MCB_I8]         = {MCB_I8},
	[MCB_I16]        = {MCB_I16},
	[MCB_I32]        = {MCB_I32},
	[MCB_I64]        = {MCB_I64},
	[MCB_PTR]        = {MCB_PTR},
	[MCB_CMP_RESULT] = {MCB_CMP_RESULT}
};

char *
mcb_build_type_cstr(const struct mcb_type *type)
{
	struct str s;
	if (!type)
		return NULL;
	estr_empty(&s);
	estr_from_cstr(&s, builtin_type_cstr[type->builtin]);
	return s.s;
}

void
mcb_destroy_type(struct mcb_type *type)
{
	if (!type)
		return;
}

const struct mcb_type *
mcb_get_type_from_builtin(enum MCB_BUILTIN_TYPE builtin)
{
	if (builtin == MCB_PTR || builtin == MCB_CMP_RESULT)
		return &butiltin_types[builtin];
	if (builtin > MCB_I64)
		return NULL;
	return &butiltin_types[builtin];
}
