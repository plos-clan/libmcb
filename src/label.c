/* SPDX-License-Identifier: LGPL-3.0-or-later */
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "mcb/func.h"
#include "mcb/label.h"

#include "darr.h"
#include "ealloc.h"
#include "err.h"

static char *auto_name(struct mcb_func *fn);

char *
auto_name(struct mcb_func *fn)
{
	char *name;
	assert(fn);

	name = ecalloc(32, sizeof(*name));
	snprintf(name, 32, "%lu", fn->label_auto_named);
	fn->label_auto_named++;

	return name;
}

struct mcb_label *
mcb_declare_label(const char *name, struct mcb_func *fn)
{
	struct mcb_label *l;
	if (!fn)
		ereturn(NULL, "!fn");
	l = ecalloc(1, sizeof(*l));

	if (name)
		l->name = strdup(name);
	else
		l->name = auto_name(fn);
	if (!l->name)
		goto err_free_l;

	darr_append(fn->label_arr, fn->label_arr_count, l);
	return l;
err_free_l:
	free(l);
	return NULL;
}

void
mcb_free_label(struct mcb_label *l)
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
	fprintf(stream, "@%s:\n", l->name);
}
