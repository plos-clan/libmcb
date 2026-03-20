/* SPDX-License-Identifier: LGPL-3.0-or-later */
#ifndef LIBMCB_CTX_H
#define LIBMCB_CTX_H
#include <stdint.h>

struct mcb_fn;
struct mcb_ctx {
	struct mcb_fn **fns;
	uint64_t nfn;
};

struct mcb_ctx *mcb_def_ctx(void);
void mcb_free_ctx(struct mcb_ctx *ctx);

#endif
