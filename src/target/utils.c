/* SPDX-License-Identifier: GPL-3.0-or-later */
#include <assert.h>
#include <stdbool.h>
#include "utils.h"

bool
mcb_is_inst_unwanted(
		const struct mcb_value *result,
		const struct mcb_inst *inst)
{
	assert(result && inst);
	if (inst->force_gen)
		return false;
	if (result->scope_end == inst)
		return true;
	return false;
}
