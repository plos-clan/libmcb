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

/* Declare a label to use. But it won't be appended to [fn].
 * If you want to append this label and really use it,
 * see mcb_inst_define_label().
 *
 * @param [name]: NULL | name:
 *     When you passed NULL to [name], the label will auto
 *     named by counter's number inside [fn] */
struct mcb_label *mcb_declare_label(const char *name, struct mcb_func *fn);

void mcb_free_label(struct mcb_label *l);

void mcb_output_label(const struct mcb_label *l, FILE *stream);

#endif
