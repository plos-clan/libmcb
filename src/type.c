/* SPDX-License-Identifier: LGPL-3.0-or-later */
#include <stddef.h>
#include "mcb/type.h"

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

const struct mcb_type *
mcb_get_type_from_builtin(enum MCB_BUILTIN_TYPE builtin)
{
	if (builtin == MCB_PTR || builtin == MCB_CMP_RESULT)
		return &butiltin_types[builtin];
	if (builtin > MCB_I64)
		return NULL;
	return &butiltin_types[builtin];
}
