/* SPDX-License-Identifier: LGPL-3.0-or-later */
#ifndef LIBMCB_VALUE_H
#define LIBMCB_VALUE_H
#include <stddef.h>
#include "type.h"

enum MCB_VALUE_KIND {
	MCB_NORMAL_VALUE,
	MCB_FUNC_ARG_VALUE,
	MCB_VAR_VALUE
};

struct mcb_func;
struct mcb_func_arg;

struct mcb_array_value {
	struct mcb_value **elems;
	size_t size;
};

struct mcb_array_elem_value {
	struct mcb_value *array_container;
};

struct mcb_struct_value {
	struct mcb_struct *structure;
	struct mcb_value **values;
};

struct mcb_struct_elem_value {
	struct mcb_struct *structure;
	struct mcb_value *structure_container;
	int idx;
};

struct mcb_value {
	char *name;
	enum MCB_VALUE_KIND kind;
	const struct mcb_type *type;

	struct mcb_inst *scope_end;

	union {
		struct mcb_array_value array;
		struct mcb_array_elem_value array_elem;
		struct mcb_func_arg *func_arg;
		struct mcb_struct_value structure;
		struct mcb_struct_elem_value structure_elem;
	} inner;

	void *data;
};

char *mcb_build_value_cstr(const struct mcb_value *value);

struct mcb_value *mcb_define_value(
		const char *name,
		const struct mcb_type *type,
		struct mcb_func *fn);

void mcb_destroy_value(struct mcb_value *val);

#endif
