/* SPDX-License-Identifier: LGPL-3.0-or-later */
#include <assert.h>
#include "mcb/amd64/all.h"
#include "mcb/typ.h"

static const char *regs[NREG][4] = {
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

const char *
mcb_amd64_str_reg(enum REG r, mcb_typ typ)
{
	int off = 0;
	assert(r < NREG);
	switch (typ) {
	case MCB_I8:  off = 3; break;
	case MCB_I16: off = 2; break;
	case MCB_I32: off = 1; break;
	case MCB_I64: off = 0; break;
	}
	return regs[r][off];
}
