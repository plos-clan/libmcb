/* This file is part of libmcb.
   SPDX-License-Identifier: LGPL-3.0-or-later
*/
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "mcb/func.h"
#include "mcb/inst/cmp.h"
#include "mcb/value.h"

#include "utils.h"

#include "../ealloc.h"
#include "../err.h"

int
mcb_inst_cmp(struct mcb_value *result,
		struct mcb_value *lhs,
		enum MCB_CMP_OPERATOR operator,
		struct mcb_value *rhs,
		struct mcb_func *fn)
{
	struct mcb_inst *inst;

	if (!result || !lhs || !rhs || !fn)
		ereturn(1, "!result || !lhs || !rhs || !fn");

	inst = ecalloc(1, sizeof(*inst));
	inst->kind = MCB_CMP_INST;
	inst->inner.cmp.result   = result;
	inst->inner.cmp.lhs      = lhs;
	inst->inner.cmp.operator = operator;
	inst->inner.cmp.rhs      = rhs;

	if (mcb_use_value(inst, result))
		goto err_free_inst;
	if (mcb_use_value(inst, lhs))
		goto err_free_inst;
	if (mcb_use_value(inst, rhs))
		goto err_free_inst;

	if (mcb_append_inst(inst, fn))
		goto err_free_inst;
	return 0;
err_free_inst:
	free(inst);
	return 1;
}

enum MCB_CMP_OPERATOR
mcb_reverse_cmp_op(enum MCB_CMP_OPERATOR op)
{
	switch (op) {
	case MCB_EQ: return MCB_NE;
	case MCB_GE: return MCB_LT;
	case MCB_GT: return MCB_LE;
	case MCB_LE: return MCB_GT;
	case MCB_LT: return MCB_GE;
	case MCB_NE: return MCB_EQ;
	}
	eabort("reverse_cmp_op(): location that must not be reached");
	return -1;
}
