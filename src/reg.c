/* SPDX-License-Identifier: LGPL-3.0-or-later */
#include <assert.h>
#include <stddef.h>
#include "mcb/blk.h"
#include "mcb/reg.h"

static int
auto_alloc_reg(int nreg, const int *in, struct mcb_val *user, struct mcb_blk *blk)
{
	assert(nreg && in && user && blk);
	assert(blk->using_regs);

	for (const int *c = in; *c; c++) {
		if (!blk->using_regs[*c]) {
			blk->allocated_regs[*c] = true;
			blk->using_regs[*c] = user;
			return *c;
		}
	}

	return nreg;
}

int
mcb_alloc_reg(int expect, int nreg, const int *in, struct mcb_val *user, struct mcb_blk *blk)
{
	assert(nreg && user && blk);
	assert(blk->using_regs);

	if (expect == nreg)
		return auto_alloc_reg(nreg, in, user, blk);

	//TODO: expect
	assert(!blk->using_regs[expect]);
	blk->allocated_regs[expect] = true;
	blk->using_regs[expect] = user;

	return expect;
}

struct mcb_val *
mcb_drop_reg(int reg, struct mcb_blk *blk)
{
	struct mcb_val *user;
	assert(blk && blk->using_regs);
	user = blk->using_regs[reg];
	blk->using_regs[reg] = NULL;
	return user;
}
