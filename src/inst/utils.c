/* This file is part of libmcb.
   SPDX-License-Identifier: LGPL-3.0-or-later
*/
#include <assert.h>
#include "mcb/func.h"
#include "mcb/inst.h"
#include "mcb/label.h"

#include "utils.h"

#include "../darr.h"
#include "../err.h"

int
mcb_append_inst(struct mcb_inst *inst, struct mcb_func *fn)
{
	struct mcb_label *last_label = NULL;
	if (!inst || !fn)
		ereturn(1, "!inst || !fn");
	if (fn->label_arr_count)
		last_label = fn->label_arr[fn->label_arr_count - 1];
	if (last_label && last_label->beg == NULL)
		last_label->beg = inst;
	darr_append(fn->inst_arr, fn->inst_arr_count, inst);
	return 0;
}

int
mcb_use_value(struct mcb_inst *inst, struct mcb_value *value)
{
	if (!inst || !value)
		ereturn(1, "!inst || !value");
	value->scope_end = inst;
	return 0;
}
