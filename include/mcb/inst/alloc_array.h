/* This file is part of libmcb.
   SPDX-License-Identifier: LGPL-3.0-or-later
*/
#ifndef LIBMCB_INST_ALLOC_ARRAY_H
#define LIBMCB_INST_ALLOC_ARRAY_H
#include "mcb/func.h"
#include "mcb/value.h"

struct mcb_alloc_array_inst {
	struct mcb_value *container;
	size_t size;
};

int mcb_inst_alloc_array(
		struct mcb_value *container,
		size_t size,
		struct mcb_func *fn);

#endif
