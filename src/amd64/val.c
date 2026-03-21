/* SPDX-License-Identifier: LGPL-3.0-or-later */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mcb/amd64_all.h"
#include "mcb/typ.h"
#include "mcb/val.h"

char *
mcb_amd64_str_imm(int64_t imm)
{
	char *buf = malloc(32);
	snprintf(buf, 32, "$%ld", imm);
	return buf;
}

char *
mcb_amd64_str_val(const struct mcb_val *v, mcb_typ typ)
{
	const char *reg;
	char *res = NULL;
	switch (v->kind) {
	case MCB_UNKOWN_VAL:
		break;
	case MCB_IMM_VAL:
		return mcb_amd64_str_imm(v->inner.imm);
	case MCB_REG_VAL:
		reg = mcb_amd64_str_reg(v->inner.reg, typ);
		res = strdup(reg);
		break;
	}
	return res;
}
