/* SPDX-License-Identifier: LGPL-3.0-or-later */
#ifndef LIBMCB_INST_SUB_H
#define LIBMCB_INST_SUB_H
#include <stdio.h>
#include "mcb/func.h"
#include "mcb/value.h"

struct mcb_sub_inst {
	struct mcb_value *result;
	struct mcb_value *lhs, *rhs;
};

int mcb_inst_sub(struct mcb_value *result,
		struct mcb_value *lhs,
		struct mcb_value *rhs,
		struct mcb_func *fn);

void mcb_output_sub_inst(const struct mcb_sub_inst *inst, FILE *stream);

#endif
