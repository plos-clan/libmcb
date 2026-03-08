/* SPDX-License-Identifier: LGPL-3.0-or-later */
#ifndef LIBMCB_INST_CALL_H
#define LIBMCB_INST_CALL_H
#include <stdio.h>
#include "mcb/func.h"
#include "mcb/value.h"

struct mcb_call_inst {
	struct mcb_value *result;
	const struct mcb_func *callee;
	int argc;
	const struct mcb_value **args;
};

#define mcb__inst_va_call_arr_length(ARR) \
	(sizeof(ARR) / sizeof((ARR)[0]))

#define mcb__inst_va_call_get_argc(...) \
	(mcb__inst_va_call_arr_length(mcb__inst_va_call_get_args(__VA_ARGS__)))

#define mcb__inst_va_call_get_args(...) \
	((struct mcb_value *[]){__VA_ARGS__})

#define mcb_inst_va_call(RESULT, CALLEE, FN, ...) \
	mcb_inst_call((RESULT), \
			(CALLEE), \
			mcb__inst_va_call_get_argc(__VA_ARGS__), \
			mcb__inst_va_call_get_args(__VA_ARGS__), \
			(FN))

int mcb_inst_call(struct mcb_value *result,
		const struct mcb_func *callee,
		int argc,
		struct mcb_value **args,
		struct mcb_func *fn);

void mcb_output_call_inst(const struct mcb_call_inst *inst, FILE *stream);

#endif
