/* SPDX-License-Identifier: LGPL-3.0-or-later */
#ifndef LIBMCB_INST_LOAD_H
#define LIBMCB_INST_LOAD_H
#include <stdio.h>
#include "mcb/func.h"
#include "mcb/value.h"

struct mcb_load_inst {
	struct mcb_value *result;
	struct mcb_value *address;
};

int mcb_inst_load(
		struct mcb_value *result,
		struct mcb_value *address,
		struct mcb_func *fn);

void mcb_output_load_inst(const struct mcb_load_inst *inst, FILE *stream);

#endif
