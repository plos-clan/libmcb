/* This file is part of libmcb.
   SPDX-License-Identifier: LGPL-3.0-or-later
*/
#ifndef LIBMCB_TARGET_GNU_ASM_INST_H
#define LIBMCB_TARGET_GNU_ASM_INST_H
#include "mcb/func.h"
#include "mcb/inst.h"

#include "gnu_asm.h"
#include "value.h"

#ifdef LIBMCB_STRIP
#define build_add_inst       mcb__gnu_asm_build_add_inst    /* add.c    */

#define build_address_of_inst \
	mcb__gnu_asm_build_address_of_inst           /*    address_of.c */

#define build_alloc_array_inst \
	mcb__gnu_asm_build_alloc_array_inst          /*   alloc_array.c */

#define build_alloc_struct_inst \
	mcb__gnu_asm_build_alloc_struct_inst         /*  alloc_struct.c */

#define build_alloc_var_inst \
	mcb__gnu_asm_build_alloc_var_inst            /*     alloc_var.c */

#define build_branch_inst    mcb__gnu_asm_build_branch_inst /* branch.c */
#define build_call_inst      mcb__gnu_asm_build_call_inst   /* func.c   */
#define build_cmp_inst       mcb__gnu_asm_build_cmp_inst    /* cmp.c    */
#define build_div_inst       mcb__gnu_asm_build_div_inst    /* div.c    */
#define build_element_of_inst \
	mcb__gnu_asm_build_element_of_inst           /*    element_of.c */
#define build_inst           mcb__gnu_asm_build_inst        /* inst.c   */
#define build_load_inst      mcb__gnu_asm_build_load_inst   /* load.c   */
#define build_mul_inst       mcb__gnu_asm_build_mul_inst    /* mul.c    */
#define build_ret_inst       mcb__gnu_asm_build_ret_inst    /* ret.c    */
#define build_store_inst     mcb__gnu_asm_build_store_inst  /* store.c  */
#define build_sub_inst       mcb__gnu_asm_build_sub_inst    /* sub.c    */
#define get_inst_suffix      mcb__gnu_asm_get_inst_suffix   /* inst.c   */
#endif

int mcb__gnu_asm_build_add_inst(struct mcb_inst *inst_outer,
		struct mcb_func *fn,
		struct gnu_asm *ctx);

int mcb__gnu_asm_build_address_of_inst(struct mcb_inst *inst_outer,
		struct mcb_func *fn,
		struct gnu_asm *ctx);

int mcb__gnu_asm_build_alloc_array_inst(struct mcb_inst *inst_outer,
		struct mcb_func *fn,
		struct gnu_asm *ctx);

int mcb__gnu_asm_build_alloc_struct_inst(struct mcb_inst *inst_outer,
		struct mcb_func *fn,
		struct gnu_asm *ctx);

int mcb__gnu_asm_build_alloc_var_inst(struct mcb_inst *inst_outer,
		struct mcb_func *fn,
		struct gnu_asm *ctx);

int mcb__gnu_asm_build_branch_inst(struct mcb_inst *inst_outer,
		struct mcb_func *fn,
		struct gnu_asm *ctx);

int mcb__gnu_asm_build_call_inst(struct mcb_inst *inst_outer,
		struct mcb_func *fn,
		struct gnu_asm *ctx);

int mcb__gnu_asm_build_cmp_inst(struct mcb_inst *inst_outer,
		struct mcb_func *fn,
		struct gnu_asm *ctx);

int mcb__gnu_asm_build_div_inst(struct mcb_inst *inst_outer,
		struct mcb_func *fn,
		struct gnu_asm *ctx);

int mcb__gnu_asm_build_element_of_inst(struct mcb_inst *inst_outer,
		struct mcb_func *fn,
		struct gnu_asm *ctx);

int mcb__gnu_asm_build_inst(struct mcb_inst *inst,
		struct mcb_func *fn,
		struct gnu_asm *ctx);

int mcb__gnu_asm_build_load_inst(struct mcb_inst *inst,
		struct mcb_func *fn,
		struct gnu_asm *ctx);

int mcb__gnu_asm_build_mul_inst(struct mcb_inst *inst_outer,
		struct mcb_func *fn,
		struct gnu_asm *ctx);

int mcb__gnu_asm_build_ret_inst(struct mcb_inst *inst_outer,
		struct mcb_func *fn,
		struct gnu_asm *ctx);

int mcb__gnu_asm_build_store_inst(struct mcb_inst *inst_outer,
		struct gnu_asm *ctx);

int mcb__gnu_asm_build_sub_inst(struct mcb_inst *inst_outer,
		struct mcb_func *fn,
		struct gnu_asm *ctx);

char mcb__gnu_asm_get_inst_suffix(enum GNU_ASM_VALUE_KIND dst_kind);

#endif
