/* SPDX-License-Identifier: LGPL-3.0-or-later */
#ifndef LIBMCB_CONTEXT_H
#define LIBMCB_CONTEXT_H
#include <stdio.h>
#include <stddef.h>
#include "mcb/func.h"

struct mcb_context {
	struct mcb_func **fn_arr;
	size_t fn_arr_count;

	struct mcb_struct **struct_arr;
	size_t struct_arr_count;
};

int mcb_define_context(struct mcb_context *ctx);
void mcb_destroy_context(struct mcb_context *ctx);
void mcb_output_context(const struct mcb_context *ctx, FILE *stream);

#endif
