/* SPDX-License-Identifier: LGPL-3.0-or-later */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mcb/inst.h"

void
mcb_destroy_inst(struct mcb_inst *inst)
{
	if (!inst)
		return;
	switch (inst->kind) {
	case MCB_CALL_INST:
		free(inst->inner.call.args);
		break;
	default:
		break;
	}
	free(inst);
}

void
mcb_output_inst(const struct mcb_inst *inst, FILE *stream)
{
	fputc('\t', stream);
	switch (inst->kind) {
	case MCB_ADD_INST:
		mcb_output_add_inst(&inst->inner.add, stream);
		break;
	case MCB_ADDRESS_OF_INST:
		mcb_output_address_of_inst(&inst->inner.address_of, stream);
		break;
	case MCB_ALLOC_ARRAY_INST:
		mcb_output_alloc_array_inst(
				&inst->inner.alloc_array, stream);
		break;
	case MCB_ALLOC_STRUCT_INST:
		mcb_output_alloc_struct_inst(
				&inst->inner.alloc_struct, stream);
		break;
	case MCB_ALLOC_VAR_INST:
		mcb_output_alloc_var_inst(&inst->inner.alloc_var, stream);
		break;
	case MCB_BRANCH_INST:
		mcb_output_branch_inst(&inst->inner.branch, stream);
		break;
	case MCB_CALL_INST:
		mcb_output_call_inst(&inst->inner.call, stream);
		break;
	case MCB_CMP_INST:
		mcb_output_cmp_inst(&inst->inner.cmp, stream);
		break;
	case MCB_DIV_INST:
		mcb_output_div_inst(&inst->inner.div, stream);
		break;
	case MCB_ELEMENT_OF_INST:
		mcb_output_element_of_inst(&inst->inner.element_of, stream);
		break;
	case MCB_JMP_INST:
		mcb_output_jmp_inst(&inst->inner.jmp, stream);
		break;
	case MCB_LOAD_INST:
		mcb_output_load_inst(&inst->inner.load, stream);
		break;
	case MCB_MUL_INST:
		mcb_output_mul_inst(&inst->inner.mul, stream);
		break;
	case MCB_RET_INST:
		mcb_output_ret_inst(&inst->inner.ret, stream);
		break;
	case MCB_STORE_INST:
		mcb_output_store_inst(&inst->inner.store, stream);
		break;
	case MCB_SUB_INST:
		mcb_output_sub_inst(&inst->inner.sub, stream);
		break;
	}
}
