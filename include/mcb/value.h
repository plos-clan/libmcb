/* This file is part of libmcb.
   SPDX-License-Identifier: LGPL-3.0-or-later
*/
#ifndef LIBMCB_VALUE_H
#define LIBMCB_VALUE_H
#include <stddef.h>
#include "type.h"

struct mcb_func;
struct mcb_func_arg;

enum MCB_VALUE_KIND {
	MCB_NORMAL_VALUE,
	MCB_ARRAY_VALUE,
	MCB_ARRAY_ELEM_VALUE,
	MCB_FUNC_ARG_VALUE,
	MCB_STRUCT_VALUE,
	MCB_STRUCT_ELEM_VALUE,
	MCB_VAR_VALUE
};

struct mcb_array_value {
	struct mcb_value **elems;
	size_t size;
};

struct mcb_array_elem_value {
	struct mcb_value *array_container;
};

struct mcb_value_inner_struct {
	struct mcb_struct *structure;
	struct mcb_value **values;
};

struct mcb_value_inner_struct_elem {
	struct mcb_struct *structure;
	struct mcb_value *structure_container;
	int idx;
};

struct mcb_value {
	char *name;
	enum MCB_VALUE_KIND kind;
	enum MCB_TYPE type;

	struct mcb_inst *scope_end;

	union {
		struct mcb_array_value array;
		struct mcb_array_elem_value array_elem;
		struct mcb_func_arg *func_arg;
		struct mcb_value_inner_struct structure;
		struct mcb_value_inner_struct_elem structure_elem;
	} inner;

	void *data;
};

struct mcb_value *mcb_define_value(
		const char *name,
		enum MCB_TYPE type,
		struct mcb_func *fn);

struct mcb_value *mcb_define_value_from_func_arg(
		const char *name,
		struct mcb_func_arg *func_arg,
		struct mcb_func *fn);

void mcb_destory_value(struct mcb_value *val);

#endif
