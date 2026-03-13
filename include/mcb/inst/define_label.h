/* SPDX-License-Identifier: LGPL-3.0-or-later */
#ifndef LIBMCB_INST_DEFINE_LABEL_H
#define LIBMCB_INST_DEFINE_LABEL_H
#include <stdio.h>
#include "mcb/func.h"
#include "mcb/label.h"
#include "mcb/value.h"

struct mcb_define_label_inst {
	struct mcb_label *label;
};

int mcb_inst_define_label(struct mcb_label *label, struct mcb_func *fn);

void mcb_output_define_label_inst(
		const struct mcb_define_label_inst *inst,
		FILE *stream);

#endif
