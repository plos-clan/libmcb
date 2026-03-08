/* SPDX-License-Identifier: LGPL-3.0-or-later */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "mcb/func.h"
#include "mcb/inst.h"
#include "mcb/inst/element_of.h"
#include "mcb/value.h"

#define LIBMCB_STRIP
#include "gen_mov.h"
#include "gnu_asm.h"
#include "inst.h"
#include "mem.h"
#include "reg.h"
#include "struct.h"
#include "utils.h"
#include "value.h"

#include "../../ealloc.h"
#include "../../err.h"
#include "../../str.h"
#include "../../text_block.h"

static enum GNU_ASM_REG mov_idx_to_reg(
		struct mcb_element_of_inst *inst,
		struct mcb_func *fn,
		struct gnu_asm *ctx);

enum GNU_ASM_REG
mov_idx_to_reg(
		struct mcb_element_of_inst *inst,
		struct mcb_func *fn,
		struct gnu_asm *ctx)
{
	struct text_block *blk;
	struct gnu_asm_value *idx;
	enum GNU_ASM_REG result;
	struct gnu_asm_value src;

	assert(inst && fn && ctx);
	assert(inst->idx);
	assert(inst->idx->data);

	idx = inst->idx->data;

	if (IS_REG(idx->kind))
		return idx->inner.reg;

	src = *idx;
	idx = ecalloc(1, sizeof(*idx));
	idx->kind = remap_value_kind(I8_REG_VALUE, src.kind);
	idx->inner.reg = alloc_reg(AUTO_ALLOC_REG, inst->idx->data, fn);
	if (idx->inner.reg == REG_COUNT) {
		if (mov_reg_user(AUTO_ALLOC_REG - 1, fn, ctx))
			eabort("mov_reg_user()");
		drop_reg(AUTO_ALLOC_REG - 1, fn);
		idx->inner.reg = alloc_reg(AUTO_ALLOC_REG - 1, inst->idx->data, fn);
		if (idx->inner.reg == REG_COUNT)
			eabort("alloc_reg()");
	}

	estr_clean(&ctx->buf);
	if (gen_mov(&ctx->buf, idx, &src, fn, ctx))
		eabort("gen_mov()");
	blk = text_block_from_str(&ctx->buf);
	append_text_block(&ctx->text, blk);

	result = idx->inner.reg;
	free(idx);

	return result;
}

int
build_element_of_inst(struct mcb_inst *inst_outer,
		struct mcb_func *fn,
		struct gnu_asm *ctx)
{
	const struct gnu_asm_value *container;
	enum GNU_ASM_REG idx_reg;
	struct mcb_element_of_inst *inst;
	struct gnu_asm_mem_obj *mem;
	struct gnu_asm_value *result;
	assert(inst_outer && fn && ctx);
	inst = &inst_outer->inner.element_of;
	assert(inst);

	assert(inst->result->data == NULL);
	assert(inst->container->data);

	container = inst->container->data;
	idx_reg = mov_idx_to_reg(inst, fn, ctx);

	result = ecalloc(1, sizeof(*result));
	result->container = inst->result;
	result->kind = map_type_to_value_kind(
			I8_MEM_VALUE,
			inst->result->type);

	assert(IS_MEM(container->kind));
	mem = ecalloc(1, sizeof(*mem));
	mem->base = container->inner.mem->base;
	mem->kind = VAR_MEM;
	mem->offset = container->inner.mem->offset;
	mem->index = idx_reg;
	mem->scale = map_value_kind_to_bytes(result->kind);
	mem->user = result;

	result->inner.mem = mem;
	inst->result->data = result;
	
	return 0;
}
