/* SPDX-License-Identifier: LGPL-3.0-or-later */
#ifndef LIBMCB_FUNC_H
#define LIBMCB_FUNC_H
#include <stdio.h>
#include <stddef.h>
#include "mcb/label.h"
#include "mcb/type.h"
#include "mcb/value.h"

struct mcb_context;

enum MCB_FUNC_EXPORT_TYPE {
	MCB_EXPORT_FUNC,
	MCB_LOCAL_FUNC
};

struct mcb_func {
	int argc;
	struct mcb_value **args;

	char *name;

	enum MCB_FUNC_EXPORT_TYPE export_type;
	const struct mcb_type *type;

	/* 'syscall_num < 0' isn't a syscall function */
	int syscall_num;

	struct mcb_inst **inst_arr;
	size_t inst_arr_count;

	struct mcb_label **label_arr;
	size_t label_arr_count;
	size_t label_auto_named;

	struct mcb_value **value_arr;
	size_t value_arr_count;
	size_t value_auto_named;

	void *data;
};

struct mcb_func *mcb_define_func(
		const char *name,
		const struct mcb_type *type,
		enum MCB_FUNC_EXPORT_TYPE export_type,
		struct mcb_context *ctx);

int mcb_append_func_arg(struct mcb_value *val, struct mcb_func *fn);

void mcb_free_func(struct mcb_func *fn);

void mcb_output_func(const struct mcb_func *fn, FILE *stream);

#endif
