#include "mcb/amd64.h"
#include "mcb/blk.h"
#include "mcb/ctx.h"
#include "mcb/fn.h"
#include "mcb/inst.h"
#include "mcb/typ.h"
#include "mcb/val.h"

int main() {
	struct mcb_ctx *ctx;
	struct mcb_fn *main_fn;

	ctx = mcb_def_ctx();

	main_fn = mcb_def_fn(ctx, "main", MCB_EXPORT_FN);

	struct mcb_blk *begin = mcb_def_blk(main_fn);
	struct mcb_val *v0 = mcb_def_imm(main_fn, 1);
	struct mcb_val *v1 = mcb_def_imm(main_fn, 2);
	struct mcb_val *r0 = mcb_def_val(main_fn);
	mcb_inst(begin, MCBO_add,
			MCB_I32, v0,
			MCB_I32, v1,
			MCB_I32, r0);

	struct mcb_val *v2 = mcb_def_imm(main_fn, 3);
	struct mcb_val *v3 = mcb_def_imm(main_fn, 4);
	struct mcb_val *r1 = mcb_def_val(main_fn);
	mcb_inst(begin, MCBO_add,
			MCB_I32, v2,
			MCB_I32, v3,
			MCB_I32, r1);

	struct mcb_val *r2 = mcb_def_val(main_fn);
	mcb_inst(begin, MCBO_smul,
			MCB_I32, r0,
			MCB_I32, r1,
			MCB_I32, r2);

	struct mcb_amd64_ctx *amd64_ctx = mcb_amd64_build(ctx);
	mcb_amd64_output(stdout, amd64_ctx);
	mcb_amd64_free_ctx(amd64_ctx);

	mcb_free_ctx(ctx);

	return 0;
}
