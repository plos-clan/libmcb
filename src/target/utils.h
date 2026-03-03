/* SPDX-License-Identifier: GPL-3.0-or-later */
#ifndef LIBMCB_TARGET_UTILS_H
#define LIBMCB_TARGET_UTILS_H
#include <stdbool.h>
#include "mcb/inst.h"
#include "mcb/value.h"

bool mcb_is_inst_unwanted(
		const struct mcb_value *result,
		const struct mcb_inst *inst);

#endif
