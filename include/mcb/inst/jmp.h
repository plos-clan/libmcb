/* SPDX-License-Identifier: LGPL-3.0-or-later */
#ifndef LIBMCB_INST_JMP_H
#define LIBMCB_INST_JMP_H
#include <stdio.h>
#include "mcb/func.h"
#include "mcb/label.h"

struct mcb_jmp_inst {
	struct mcb_label *label;
};

int mcb_inst_jmp(struct mcb_label *label, struct mcb_func *fn);

void mcb_output_jmp_inst(const struct mcb_jmp_inst *inst, FILE *stream);

#endif
