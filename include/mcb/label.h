/* SPDX-License-Identifier: LGPL-3.0-or-later */
#ifndef LIBMCB_LABEL_H
#define LIBMCB_LABEL_H
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>

struct mcb_func;
struct mcb_inst;

struct mcb_label {
	char *name;

	struct mcb_inst *beg;
};

int mcb_append_label(struct mcb_label *label, struct mcb_func *fn);

bool mcb_can_define_label(const struct mcb_func *fn,
		size_t label_idx,
		size_t inst_idx);

struct mcb_label *mcb_define_label(const char *name);

void mcb_destroy_label(struct mcb_label *l);

void mcb_output_label(const struct mcb_label *l, FILE *stream);

#endif
