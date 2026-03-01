/* SPDX-License-Identifier: LGPL-3.0-or-later */
#include "mcb/func.h"
#include "mcb/syscall.h"

#include "err.h"

int
mcb_func_to_syscall(int syscall_num, struct mcb_func *fn)
{
	if (!fn)
		ereturn(1, "!fn");
	if (syscall_num < 0)
		ereturn(1, "not a syscall number");
	fn->syscall_num = syscall_num;
	return 0;
}
