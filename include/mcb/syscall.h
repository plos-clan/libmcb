/* SPDX-License-Identifier: LGPL-3.0-or-later
 *
 * Convert function to syscall is very simple and stupid.
 * If you create a new *syscall* struct to implement it,
 * Just like fucking eat shits💩 */
#ifndef LIBMCB_SYSCALL_H
#define LIBMCB_SYSCALL_H
#include "func.h"

int mcb_func_to_syscall(int syscall_num, struct mcb_func *fn);

#endif
