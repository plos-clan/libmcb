/* SPDX-License-Identifier: LGPL-3.0-or-later */
#include <limits.h>
#include <stdint.h>
#include "mcb/typ.h"

mcb_typ
mcb_get_typ_of_imm(uint64_t imm)
{
	if (imm <= UINT8_MAX)
		return MCB_I8;
	if (imm <= UINT16_MAX)
		return MCB_I16;
	if (imm <= UINT32_MAX)
		return MCB_I32;
	if (imm <= UINT64_MAX)
		return MCB_I64;
	return MCB_I8;
}
