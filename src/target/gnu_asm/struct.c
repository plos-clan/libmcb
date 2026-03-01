/* This file is part of libmcb.
   SPDX-License-Identifier: LGPL-3.0-or-later
*/
#include <assert.h>
#include <stdlib.h>

#define LIBMCB_STRIP
#include "struct.h"
#include "value.h"

void
destroy_struct_value(struct mcb_value *container)
{
	struct gnu_asm_struct_value *struct_value;
	if (!container)
		return;
	assert(container->type->builtin == MCB_STRUCT);

	struct_value = container->data;
	for (int i = 0; i < container->inner.structure
			.structure->elems_count; i++) {
		if (container->inner.structure.values[i] == NULL) {
			free(struct_value->values[i]->inner.mem);
			free(struct_value->values[i]);
		}
		destroy_value(container->inner.structure.values[i]);
	}
	free(struct_value->values);

	free(container->data);
	container->data = NULL;
}
