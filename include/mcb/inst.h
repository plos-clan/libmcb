/* SPDX-License-Identifier: LGPL-3.0-or-later */
#ifndef LIBMCB_INST_H
#define LIBMCB_INST_H
#include <stdint.h>

struct mcb_blk;
struct mcb_val;

enum MCB_INST_OP {
	MCBO_add,

	MCBO_copy
};

struct mcb_inst {
	enum MCB_INST_OP op;

	/* arg0_typ, arg1_typ, dst_typ */
	uint64_t typ[3];
	struct mcb_val *arg[2];
	struct mcb_val *dst;
};

void mcb_free_inst(struct mcb_inst *inst);
int mcb_inst(struct mcb_blk *blk,
		enum MCB_INST_OP op,
		uint64_t arg0_typ, struct mcb_val *arg0,
		uint64_t arg1_typ, struct mcb_val *arg1,
		uint64_t dst_typ,  struct mcb_val *dst);

#endif
