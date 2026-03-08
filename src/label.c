/* SPDX-License-Identifier: LGPL-3.0-or-later */
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "mcb/func.h"
#include "mcb/label.h"

#include "darr.h"
#include "ealloc.h"
#include "err.h"

int
mcb_append_label(struct mcb_label *label, struct mcb_func *fn)
{
	if (!label || !fn)
		ereturn(1, "!label || !fn");
	darr_append(fn->label_arr, fn->label_arr_count, label);
	return 0;
}

bool
mcb_can_define_label(const struct mcb_func *fn,
		size_t label_idx,
		size_t inst_idx)
{
	if (!fn->label_arr)
		return false;
	if (label_idx >= fn->label_arr_count)
		return false;
	if (fn->label_arr[label_idx]->beg == fn->inst_arr[inst_idx])
		return true;
	return false;
}

struct mcb_label *
mcb_define_label(const char *name)
{
	struct mcb_label *l;
	if (!name)
		ereturn(NULL, "!name");
	l = ecalloc(1, sizeof(*l));
	l->name = strdup(name);
	if (!l->name)
		goto err_free_l;
	return l;
err_free_l:
	free(l);
	return NULL;
}

void
mcb_destroy_label(struct mcb_label *l)
{
	if (!l)
		return;
	free(l->name);
	free(l);
}

void
mcb_output_label(const struct mcb_label *l, FILE *stream)
{
	if (!l)
		return;
	fprintf(stream, "%s:\n", l->name);
}
