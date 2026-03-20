/* SPDX-License-Identifier: LGPL-3.0-or-later */
#ifndef LIBMCB_VAL_H
#define LIBMCB_VAL_H
#include <stdint.h>

struct mcb_inst;

enum MCB_VAL_KIND {
	MCB_UNKOWN_VAL,
	MCB_IMM_VAL,
	MCB_REG_VAL
};

struct mcb_val {
	enum MCB_VAL_KIND kind;

	union {
		int64_t imm;
		int reg;
	} inner;

	struct mcb_inst *last_user;
};

struct mcb_val *mcb_def_imm(struct mcb_fn *fn, int64_t imm);
struct mcb_val *mcb_def_val(struct mcb_fn *fn);
void mcb_free_val(struct mcb_val *val);
void mcb_use_val(struct mcb_val *val, struct mcb_inst *user);

#endif
