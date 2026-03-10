/* SPDX-License-Identifier: LGPL-3.0-or-later */
#ifndef LIBMCB_INST_ELEMENT_OF_H
#define LIBMCB_INST_ELEMENT_OF_H
#include <stdio.h>
#include "mcb/func.h"
#include "mcb/value.h"

struct mcb_element_of_inst {
	struct mcb_value *result;
	struct mcb_value *container;
	struct mcb_value *idx;
};

int mcb_inst_element_of(
		struct mcb_value *result,
		struct mcb_value *container,
		struct mcb_value *idx,
		struct mcb_func *fn);

void mcb_output_element_of_inst(
		const struct mcb_element_of_inst *inst,
		FILE *stream);

#endif
