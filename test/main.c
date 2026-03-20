#include "mcb/amd64.h"
#include "mcb/blk.h"
#include "mcb/ctx.h"
#include "mcb/fn.h"
#include "mcb/inst.h"
#include "mcb/type.h"
#include "mcb/val.h"

int main() {
	struct mcb_ctx *ctx;
	struct mcb_fn *main_fn;

	ctx = mcb_def_ctx();

	main_fn = mcb_def_fn(ctx, "main");

	struct mcb_blk *begin = mcb_def_blk(main_fn);
	struct mcb_val *v0 = mcb_def_imm(main_fn, 1);
	struct mcb_val *v1 = mcb_def_imm(main_fn, 114);
	struct mcb_val *r0 = mcb_def_val(main_fn);
	mcb_inst(begin, MCBO_add,
			MCB_I32, v0,
			MCB_I32, v1,
			MCB_I32, r0);

	mcb_amd64_build(ctx);

	mcb_free_ctx(ctx);

	return 0;
}
