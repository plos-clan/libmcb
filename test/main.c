#include <assert.h>
#include <stdio.h>
#include "mcb/array.h"
#include "mcb/context.h"
#include "mcb/func.h"
#include "mcb/inst.h"
#include "mcb/label.h"
#include "mcb/target/gnu_asm.h"
#include "mcb/type.h"
#include "mcb/value.h"

#define UTILSH_EALLOC_IMPL
#include "../src/ealloc.h"

static void define_main_fn(struct mcb_context *ctx);

void
define_main_fn(struct mcb_context *ctx)
{
	/* fn _start(%a0:i32)
	 * entry:
	 *     ret %v1
	 */

	const struct mcb_type *i32_type = mcb_get_integer_type(MCB_I32);

	struct mcb_func *main_fn =
		mcb_define_func("_start", i32_type, MCB_EXPORT_FUNC, ctx);

	struct mcb_value *va0 =
		mcb_define_value("%va0", i32_type, main_fn);
	mcb_append_func_arg(va0, main_fn);

	struct mcb_value *v1 =
		mcb_define_value("%v1", i32_type, main_fn);
	mcb_inst_store_int(v1, 1, main_fn);

	struct mcb_value *v2 =
		mcb_define_value("%v2", i32_type, main_fn);
	mcb_inst_add(v2, va0, v1, main_fn);

	mcb_inst_ret(v2, main_fn);
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
