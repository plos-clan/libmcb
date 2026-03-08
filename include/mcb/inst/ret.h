/* SPDX-License-Identifier: LGPL-3.0-or-later */
#ifndef LIBMCB_INST_RET_H
#define LIBMCB_INST_RET_H
#include <stdio.h>
#include "mcb/func.h"
#include "mcb/value.h"

struct mcb_ret_inst {
	struct mcb_value *val;
};

int mcb_inst_ret(struct mcb_value *val, struct mcb_func *fn);

void mcb_output_ret_inst(const struct mcb_ret_inst *inst, FILE *stream);

#endif
