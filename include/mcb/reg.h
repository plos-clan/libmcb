/* SPDX-License-Identifier: LGPL-3.0-or-later */
#ifndef LIBMCB_REG_H
#define LIBMCB_REG_H

struct mcb_blk;
struct mcb_val;

int mcb_alloc_reg(
		int expect,
		int nreg,
		const int *in,
		struct mcb_val *user,
		struct mcb_blk *blk);

struct mcb_val *mcb_drop_reg(int reg, struct mcb_blk *blk);

#endif
