/* SPDX-License-Identifier: LGPL-3.0-or-later */
#include <assert.h>
#include <stdlib.h>
#include "mcb/func.h"
#include "mcb/inst/define_label.h"
#include "mcb/label.h"
#include "mcb/value.h"
#include "utils.h"

#include "../ealloc.h"
#include "../err.h"

int
mcb_inst_define_label(struct mcb_label *label, struct mcb_func *fn)
{
	struct mcb_inst *inst;
	if (!label || !fn)
		ereturn(1, "!label || !fn");
	inst = ecalloc(1, sizeof(*inst));
	inst->kind = MCB_DEFINE_LABEL_INST;
	inst->inner.define_label.label = label;
	if (mcb_append_inst(inst, fn))
		goto err_free_inst;
	return 0;
err_free_inst:
	free(inst);
	return 1;
}

void
mcb_output_define_label_inst(
		const struct mcb_define_label_inst *inst,
		FILE *stream)
{
	fprintf(stream, "@%s:\n", inst->label->name);
}
