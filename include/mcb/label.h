/* This file is part of libmcb.
   SPDX-License-Identifier: LGPL-3.0-or-later
*/
#ifndef LIBMCB_LABEL_H
#define LIBMCB_LABEL_H

struct mcb_func;
struct mcb_inst;

struct mcb_label {
	char *name;

	struct mcb_inst *beg;
};

int mcb_append_label(struct mcb_label *label, struct mcb_func *fn);

struct mcb_label *mcb_define_label(const char *name);

void mcb_destroy_label(struct mcb_label *l);

#endif
