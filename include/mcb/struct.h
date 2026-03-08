/* SPDX-License-Identifier: LGPL-3.0-or-later */
#ifndef LIBMCB_STRUCT_H
#define LIBMCB_STRUCT_H
#include "context.h"
#include "func.h"
#include "type.h"
#include "value.h"

struct mcb_struct_elem {
	char *name;
	const struct mcb_type *type;
};

struct mcb_struct {
	char *name;

	struct mcb_struct_elem **elems;
	int elems_count;
};

#endif
