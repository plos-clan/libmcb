/* SPDX-License-Identifier: LGPL-3.0-or-later */
#ifndef LIBMCB_CONTEXT_H
#define LIBMCB_CONTEXT_H
#include <stdio.h>
#include <stddef.h>

struct mcb_func;
struct mcb_struct;
struct mcb_type;

struct mcb_context {
	struct mcb_func **fn_arr;
	size_t fn_arr_count;

	struct mcb_struct **struct_arr;
	size_t struct_arr_count;

	struct mcb_type **defined_types;
	size_t defined_types_count;
};

struct mcb_context *mcb_define_context(void);
void mcb_free_context(struct mcb_context *ctx);
void mcb_output_context(const struct mcb_context *ctx, FILE *stream);

#endif
