/* This file is part of libmcb.
   SPDX-License-Identifier: LGPL-3.0-or-later
*/
#ifndef LIBMCB_INST_CMP_H
#define LIBMCB_INST_CMP_H
#include "mcb/func.h"
#include "mcb/value.h"

enum MCB_CMP_OPERATOR {
	MCB_EQ,
	MCB_GE,
	MCB_GT,
	MCB_LE,
	MCB_LT,
	MCB_NE,
};

struct mcb_cmp_inst {
	struct mcb_value *result;
	struct mcb_value *lhs, *rhs;
	enum MCB_CMP_OPERATOR operator;
};

int mcb_inst_cmp(struct mcb_value *result,
		struct mcb_value *lhs,
		enum MCB_CMP_OPERATOR operator,
		struct mcb_value *rhs,
		struct mcb_func *fn);

enum MCB_CMP_OPERATOR mcb_reverse_cmp_op(enum MCB_CMP_OPERATOR op);

#endif
