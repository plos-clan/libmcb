#include <assert.h>
#include <stdio.h>
#include "mcb/array.h"
#include "mcb/context.h"
#include "mcb/func.h"
#include "mcb/inst.h"
#include "mcb/label.h"
#include "mcb/target/gnu_asm.h"
#include "mcb/value.h"

#define UTILSH_EALLOC_IMPL
#include "../src/ealloc.h"

static void define_main_fn(struct mcb_context *ctx);

void
define_main_fn(struct mcb_context *ctx)
{
	/* fn _start(%a0:i32)
	 * entry:
	 *     %v0:[i32, 2] = alloc_array
	 *     %v0[0] = store 114
	 *     %v0[1] = store 514
	 *     %v1 = add %v0[0], %v0[1]
	 *     ret %v1
	 */
	struct mcb_func *main_fn =
		mcb_define_func("_start", MCB_I32, MCB_EXPORT_FUNC, ctx);
	struct mcb_func_arg *a0 =
		mcb_define_func_arg("a0", MCB_I32, main_fn);
	assert(a0);

	struct mcb_label *entry = mcb_define_label("entry");
	mcb_append_label(entry, main_fn);

	struct mcb_value *v0 = mcb_define_array("v0", MCB_I32, 2, main_fn);
	mcb_inst_alloc_array(v0, main_fn);

	struct mcb_value *v0_0 = mcb_get_value_from_array(v0, 0, main_fn);
	struct mcb_value *v0_1 = mcb_get_value_from_array(v0, 1, main_fn);

	mcb_inst_store_int(v0_0, 114, main_fn);
	mcb_inst_store_int(v0_1, 514, main_fn);

	struct mcb_value *v1 = mcb_define_value("v1", MCB_I32, main_fn);
	mcb_inst_add(v1, v0_0, v0_1, main_fn);

	mcb_inst_ret(v1, main_fn);
}

int
main(void)
{
	struct mcb_context ctx;
	mcb_define_context(&ctx);

	define_main_fn(&ctx);

	// FILE *fp = fopen("/tmp/libmcb_out.s", "w");

	/* output */
	if (mcb_target_gnu_asm(stdout, &ctx))
		return 1;

	// fclose(fp);

	mcb_destroy_context(&ctx);
	return 0;
}
