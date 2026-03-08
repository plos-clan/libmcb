/* SPDX-License-Identifier: LGPL-3.0-or-later */
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "mcb/context.h"
#include "mcb/func.h"
#include "mcb/struct.h"

int
mcb_define_context(struct mcb_context *ctx)
{
	if (!ctx)
		return 1;
	memset(ctx, 0, sizeof(*ctx));
	return 0;
}

void
mcb_destroy_context(struct mcb_context *ctx)
{
	if (!ctx)
		return;
	for (size_t i = 0; i < ctx->fn_arr_count; i++)
		mcb_destroy_func(ctx->fn_arr[i]);
	free(ctx->fn_arr);

	free(ctx->struct_arr);
}

void
mcb_output_context(const struct mcb_context *ctx, FILE *stream)
{
	if (!stream || !ctx)
		return;
	for (size_t i = 0; i < ctx->fn_arr_count; i++)
		mcb_output_func(ctx->fn_arr[i], stream);
}
