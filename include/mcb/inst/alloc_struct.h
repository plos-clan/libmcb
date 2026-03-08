/* SPDX-License-Identifier: LGPL-3.0-or-later */
#ifndef LIBMCB_INST_ALLOC_STRUCT_H
#define LIBMCB_INST_ALLOC_STRUCT_H
#include <stdio.h>
#include "mcb/func.h"
#include "mcb/struct.h"
#include "mcb/value.h"

struct mcb_alloc_struct_inst {
	struct mcb_value *container;
	struct mcb_struct *structure;
};

int mcb_inst_alloc_struct(
		struct mcb_value *container,
		struct mcb_struct *structure,
		struct mcb_func *fn);

void mcb_output_alloc_struct_inst(
		const struct mcb_alloc_struct_inst *inst,
		FILE *stream);

#endif
