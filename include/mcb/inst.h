/* This file is part of libmcb.
   SPDX-License-Identifier: LGPL-3.0-or-later
*/
#ifndef LIBMCB_INST_H
#define LIBMCB_INST_H
#include <stdbool.h>
#include "mcb/inst/add.h"
#include "mcb/inst/address_of.h"
#include "mcb/inst/alloc_array.h"
#include "mcb/inst/alloc_struct.h"
#include "mcb/inst/alloc_var.h"
#include "mcb/inst/branch.h"
#include "mcb/inst/call.h"
#include "mcb/inst/cmp.h"
#include "mcb/inst/div.h"
#include "mcb/inst/element_of.h"
#include "mcb/inst/jmp.h"
#include "mcb/inst/load.h"
#include "mcb/inst/mul.h"
#include "mcb/inst/ret.h"
#include "mcb/inst/store.h"
#include "mcb/inst/sub.h"

struct mcb_func;

enum MCB_INST_KIND {
	MCB_ADD_INST,
	MCB_ADDRESS_OF_INST,
	MCB_ALLOC_ARRAY_INST,
	MCB_ALLOC_STRUCT_INST,
	MCB_ALLOC_VAR_INST,
	MCB_BRANCH_INST,
	MCB_CALL_INST,
	MCB_CMP_INST,
	MCB_DIV_INST,
	MCB_ELEMENT_OF_INST,
	MCB_JMP_INST,
	MCB_LOAD_INST,
	MCB_MUL_INST,
	MCB_RET_INST,
	MCB_STORE_INST,
	MCB_SUB_INST
};

struct mcb_inst {
	bool force_gen;
	enum MCB_INST_KIND kind;
	union {
		struct mcb_add_inst          add;
		struct mcb_address_of_inst   address_of;
		struct mcb_alloc_array_inst  alloc_array;
		struct mcb_alloc_struct_inst alloc_struct;
		struct mcb_alloc_var_inst    alloc_var;
		struct mcb_branch_inst       branch;
		struct mcb_call_inst         call;
		struct mcb_cmp_inst          cmp;
		struct mcb_div_inst          div;
		struct mcb_element_of_inst   element_of;
		struct mcb_jmp_inst          jmp;
		struct mcb_load_inst         load;
		struct mcb_mul_inst          mul;
		struct mcb_ret_inst          ret;
		struct mcb_store_inst        store;
		struct mcb_sub_inst          sub;
	} inner;
};

void mcb_force_gen_inst(struct mcb_func *fn);

void mcb_destroy_inst(struct mcb_inst *inst);

#endif
