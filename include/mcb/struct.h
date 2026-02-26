/* This file is part of libmcb.
   SPDX-License-Identifier: LGPL-3.0-or-later
*/
#ifndef LIBMCB_STRUCT_H
#define LIBMCB_STRUCT_H
#include "context.h"
#include "func.h"
#include "type.h"
#include "value.h"

struct mcb_struct_elem {
	char *name;
	enum MCB_TYPE type;
};

struct mcb_struct {
	char *name;

	struct mcb_struct_elem **elems;
	int elems_count;
};

struct mcb_struct *mcb_declare_struct(
		const char *name,
		struct mcb_context *ctx);

/* @return: index of element in struct, return -1 on error. */
int mcb_declare_struct_elem(
		const char *name,
		enum MCB_TYPE type,
		struct mcb_struct *structure);

struct mcb_value *mcb_define_struct_value(
		const char *name,
		struct mcb_struct *structure,
		struct mcb_func *fn);

void mcb_destroy_struct(struct mcb_struct *structure);
void mcb_destroy_struct_elem(struct mcb_struct_elem *elem);

struct mcb_value *mcb_get_value_from_struct(
		struct mcb_value *container,
		struct mcb_struct *structure,
		int idx,
		struct mcb_func *fn);

#endif
