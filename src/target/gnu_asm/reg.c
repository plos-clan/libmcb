/* SPDX-License-Identifier: LGPL-3.0-or-later */
#include <assert.h>
#define LIBMCB_STRIP
#include "func.h"
#include "reg.h"
#include "value_kind.h"
#include "../../err.h"
#include "../../macros.h"

static const enum GNU_ASM_REG reg_alloc_priority[] = {
	RAX, RBX,
	R10, R11, R12, R13, R14, R15,
	R9,  R8,  RCX, RDX, RSI, RDI
};

static const char *regs[REG_COUNT][4] = {
	[RBP]  = {"%rbp", "%ebp",  "%bp",   "%bpl" },
	[RSP]  = {"%rsp", "%esp",  "%sp",   "%spl" },
	[RAX]  = {"%rax", "%eax",  "%ax",   "%al"  },
	[RBX]  = {"%rbx", "%ebx",  "%bx",   "%bl"  },
	[RCX]  = {"%rcx", "%ecx",  "%cx",   "%cl"  },
	[RDX]  = {"%rdx", "%edx",  "%dx",   "%dl"  },
	[RDI]  = {"%rdi", "%edi",  "%di",   "%dil" },
	[RSI]  = {"%rsi", "%esi",  "%si",   "%sil" },
	[R8 ]  = {"%r8",  "%r8d",  "%r8w",  "%r8b" },
	[R9 ]  = {"%r9",  "%r9d",  "%r9w",  "%r9b" },
	[R10]  = {"%r10", "%r10d", "%r10w", "%r10b"},
	[R11]  = {"%r11", "%r11d", "%r11w", "%r11b"},
	[R12]  = {"%r12", "%r12d", "%r12w", "%r12b"},
	[R13]  = {"%r13", "%r13d", "%r13w", "%r13b"},
	[R14]  = {"%r14", "%r14d", "%r14w", "%r14b"},
	[R15]  = {"%r15", "%r15d", "%r15w", "%r15b"}
};

enum GNU_ASM_REG
alloc_reg(enum GNU_ASM_REG reg,
		struct gnu_asm_value *user,
		struct mcb_func *fn)
{
	struct gnu_asm_func *f = fn->data;
	assert(f);

	if (reg != AUTO_ALLOC_REG) {
		if (f->allocated_reg[reg])
			return REG_COUNT;
		f->allocated_reg[reg] = user;
		return reg;
	}

	for (unsigned int i = 0; i < LENGTH(reg_alloc_priority); i++) {
		if (!f->allocated_reg[reg_alloc_priority[i]]) {
			f->allocated_reg[reg_alloc_priority[i]] = user;
			return reg_alloc_priority[i];
		}
	}

	return REG_COUNT;
}

const char *
cstr_from_reg(enum GNU_ASM_REG r, int siz)
{
	if (r >= REG_COUNT)
		return NULL;
	return regs[r][siz];
}

void
drop_reg(enum GNU_ASM_REG reg, struct mcb_func *fn)
{
	struct gnu_asm_func *f;
	assert(fn);
	f = fn->data;
	f->allocated_reg[reg] = NULL;
}

int
reg_offset_from_kind(enum GNU_ASM_VALUE_KIND kind)
{
	switch (kind) {
	case UNKOWN_VALUE: return -1;
	CASE_I8_VALUE:  CASE_U8_VALUE:   return 3;
	CASE_I16_VALUE: CASE_U16_VALUE:  return 2;
	CASE_I32_VALUE: CASE_U32_VALUE:  return 1;
	CASE_I64_VALUE: CASE_U64_VALUE:  return 0;
	case CMP_RESULT_VALUE:
		eabort("unexpected dst_kind 'CMP_RESULT_VALUE'");
		break;
	case STRUCT_VALUE:
		eabort("unexpected dst_kind 'STRUCT_VALUE'");
		break;
	}
	return -1;
}

struct str *
str_from_reg(struct str *s, enum GNU_ASM_REG r, int siz)
{
	assert(s);
	estr_empty(s);
	estr_from_cstr(s, cstr_from_reg(r, siz));
	return s;
}
