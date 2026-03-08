/* SPDX-License-Identifier: LGPL-3.0-or-later */
#ifndef LIBMCB_INST_ADD_H
#define LIBMCB_INST_ADD_H
#include <stdio.h>
#include "mcb/func.h"
#include "mcb/value.h"

struct mcb_add_inst {
	struct mcb_value *result;
	struct mcb_value *lhs, *rhs;
};

int mcb_inst_add(struct mcb_value *result,
		struct mcb_value *lhs,
		struct mcb_value *rhs,
		struct mcb_func *fn);

void mcb_output_add_inst(const struct mcb_add_inst *inst, FILE *stream);

#endif
