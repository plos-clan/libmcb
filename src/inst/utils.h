/* SPDX-License-Identifier: LGPL-3.0-or-later */
#ifndef LIBMCB_INST_UTILS_H
#define LIBMCB_INST_UTILS_H
#include "mcb/func.h"
#include "mcb/inst.h"

int mcb_append_inst(struct mcb_inst *inst, struct mcb_func *fn);
int mcb_use_value(struct mcb_inst *inst, struct mcb_value *value);

#endif
