/* This file is part of libmcb.
   SPDX-License-Identifier: LGPL-3.0-or-later
*/
#ifndef LIBMCB_FUNC_H
#define LIBMCB_FUNC_H
#include <stddef.h>
#include "mcb/label.h"
#include "mcb/type.h"
#include "mcb/value.h"

struct mcb_context;

enum MCB_FUNC_EXPORT_TYPE {
	MCB_EXPORT_FUNC,
	MCB_LOCAL_FUNC
};

struct mcb_func_arg {
	char *name;
	enum MCB_TYPE type;
	struct mcb_value *val_link;
};

struct mcb_func {
	int argc;
	struct mcb_func_arg **args;

	char *name;

	enum MCB_FUNC_EXPORT_TYPE export_type;
	enum MCB_TYPE type;

	struct mcb_inst **inst_arr;
	size_t inst_arr_count;

	struct mcb_label **label_arr;
	size_t label_arr_count;

	struct mcb_value **value_arr;
	size_t value_arr_count;

	void *data;
};

struct mcb_func *mcb_define_func(
		const char *name,
		enum MCB_TYPE type,
		enum MCB_FUNC_EXPORT_TYPE export_type,
		struct mcb_context *ctx);

struct mcb_func_arg *mcb_define_func_arg(
		const char *name,
		enum MCB_TYPE type,
		struct mcb_func *fn);

void mcb_destroy_func(struct mcb_func *fn);
void mcb_destroy_func_arg(struct mcb_func_arg *arg);

#endif
