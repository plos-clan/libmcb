/* SPDX-License-Identifier: LGPL-3.0-or-later */
#include <assert.h>
#include "mcb/blk.h"
#include "mcb/reg.h"

static int
auto_alloc_reg(int nreg, struct mcb_val *user, struct mcb_blk *blk)
{
	assert(nreg && user && blk);
	assert(blk->using_regs);

	for (int i = 0; i < nreg; i++) {
		if (!blk->using_regs[i]) {
			blk->allocated_regs[i] = true;
			blk->using_regs[i] = user;
			return i;
		}
	}

	return nreg;
}

int
mcb_alloc_reg(int expect, int nreg, struct mcb_val *user, struct mcb_blk *blk)
{
	assert(nreg && user && blk);
	assert(blk->using_regs);

	if (expect == nreg)
		return auto_alloc_reg(nreg, user, blk);

	//TODO: expect

	return nreg;
}
