/* SPDX-License-Identifier: LGPL-3.0-or-later */
#ifndef LIBMCB_INST_STORE_H
#define LIBMCB_INST_STORE_H
#include <stdint.h>
#include <stdio.h>
#include "mcb/func.h"
#include "mcb/value.h"

enum MCB_STORE_INST_KIND {
	MCB_STORE_INT,
	MCB_STORE_STRING,
	MCB_STORE_UINT,
	MCB_STORE_VALUE
};

struct mcb_store_string_operand {
	char *str;
	size_t len;
};

struct mcb_store_inst {
	struct mcb_value *container;
	enum MCB_STORE_INST_KIND kind;
	union {
		int64_t i;
		uint64_t u;
		struct mcb_value *value;
		struct mcb_store_string_operand str;
	} operand;
};

int mcb_inst_store_int(struct mcb_value *container,
		int64_t data,
		struct mcb_func *fn);

int mcb_inst_store_string(struct mcb_value *container,
		const char *str,
		size_t len,
		struct mcb_func *fn);

int mcb_inst_store_uint(struct mcb_value *container,
		uint64_t data,
		struct mcb_func *fn);

int mcb_inst_store_value(struct mcb_value *container,
		struct mcb_value *data,
		struct mcb_func *fn);

void mcb_output_store_inst(const struct mcb_store_inst *inst, FILE *stream);

#endif
