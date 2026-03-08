/* SPDX-License-Identifier: LGPL-3.0-or-later */
#ifndef LIBMCB_TYPE_H
#define LIBMCB_TYPE_H

enum MCB_BUILTIN_TYPE {
	MCB_U8, MCB_U16, MCB_U32, MCB_U64,
	MCB_I8, MCB_I16, MCB_I32, MCB_I64,

	MCB_PTR, MCB_STRING,

	MCB_ARRAY, MCB_CMP_RESULT, MCB_STRUCT
};

struct mcb_type;
struct mcb_type {
	enum MCB_BUILTIN_TYPE builtin;
	struct mcb_type *inner;
};

char *mcb_build_type_cstr(const struct mcb_type *type);

void mcb_destroy_type(struct mcb_type *type);

const struct mcb_type *mcb_get_type_from_builtin(enum MCB_BUILTIN_TYPE builtin);

#endif
