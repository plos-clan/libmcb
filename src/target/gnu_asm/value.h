/* This file is part of libmcb.
   SPDX-License-Identifier: LGPL-3.0-or-later
*/
#ifndef LIBMCB_TARGET_GNU_ASM_VALUE_H
#define LIBMCB_TARGET_GNU_ASM_VALUE_H
#include <stdint.h>
#include "mcb/inst/cmp.h"
#include "mcb/value.h"

#include "mem.h"
#include "reg.h"
#include "value_kind.h"

union gnu_asm_imm_value {
	int64_t i;
	uint64_t ui;
};

struct gnu_asm_value {
	const struct mcb_value *container;
	enum GNU_ASM_VALUE_KIND kind;
	union {
		union gnu_asm_imm_value imm;
		struct gnu_asm_mem_obj *mem;
		enum MCB_CMP_OPERATOR operator;
		enum GNU_ASM_REG reg;
	} inner;
};

#ifdef LIBMCB_STRIP
#define destroy_value           mcb__gnu_asm_destroy_value
#define drop_value              mcb__gnu_asm_drop_value
#define map_bytes_to_value_kind mcb__gnu_asm_map_bytes_to_value_kind
#define map_type_to_value_kind  mcb__gnu_asm_map_type_to_value_kind
#define map_value_kind_to_bytes mcb__gnu_asm_map_value_kind_to_bytes
#define remap_value_kind        mcb__gnu_asm_remap_value_kind

#define str_from_value          mcb__gnu_asm_str_from_value
#define str_from_imm            mcb__gnu_asm_str_from_imm
#endif

void mcb__gnu_asm_destroy_value(struct mcb_value *container);
void mcb__gnu_asm_drop_value(struct mcb_value *val, struct mcb_func *fn);
enum GNU_ASM_VALUE_KIND mcb__gnu_asm_map_bytes_to_value_kind(
		enum GNU_ASM_VALUE_KIND base,
		int bytes);
enum GNU_ASM_VALUE_KIND mcb__gnu_asm_map_type_to_value_kind(
		enum GNU_ASM_VALUE_KIND base,
		enum MCB_TYPE t);
int mcb__gnu_asm_map_value_kind_to_bytes(enum GNU_ASM_VALUE_KIND kind);
enum GNU_ASM_VALUE_KIND mcb__gnu_asm_remap_value_kind(
		enum GNU_ASM_VALUE_KIND base,
		enum GNU_ASM_VALUE_KIND kind);
struct str *mcb__gnu_asm_str_from_value(
		struct str *s,
		const struct gnu_asm_value *v);
struct str *mcb__gnu_asm_str_from_imm(
		struct str *s,
		const struct gnu_asm_value *v);

#endif
