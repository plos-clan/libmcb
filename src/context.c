/* SPDX-License-Identifier: LGPL-3.0-or-later */
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "mcb/context.h"
#include "mcb/func.h"
#include "mcb/struct.h"

#include "ealloc.h"

struct mcb_context *
mcb_define_context(void)
{
	struct mcb_context *ctx = ecalloc(1, sizeof(*ctx));
	return ctx;
}

void
mcb_free_context(struct mcb_context *ctx)
{
	if (!ctx)
		return;
	for (size_t i = 0; i < ctx->fn_arr_count; i++)
		mcb_free_func(ctx->fn_arr[i]);
	for (size_t i = 0; i < ctx->defined_types_count; i++)
		mcb_free_type(ctx->defined_types[i]);
	free(ctx->fn_arr);
	free(ctx->struct_arr);
	free(ctx->defined_types);
	free(ctx);
}

void
mcb_output_context(const struct mcb_context *ctx, FILE *stream)
{
	if (!stream || !ctx)
		return;
	for (size_t i = 0; i < ctx->fn_arr_count; i++)
		mcb_output_func(ctx->fn_arr[i], stream);
}
