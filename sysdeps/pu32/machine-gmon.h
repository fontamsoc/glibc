// SPDX-License-Identifier: GPL-2.0-only
// (c) William Fonkou Tambe

#include <sysdep.h>

static void mcount_internal (
	unsigned long frompc,
	unsigned long selfpc);

#define _MCOUNT_DECL(frompc, selfpc)         \
	static inline void mcount_internal ( \
		unsigned long frompc,        \
		unsigned long selfpc)

#define MCOUNT                                                                  \
	void _mcount (void *frompc) {                                           \
		mcount_internal ((unsigned long) frompc,                        \
			         (unsigned long) __builtin_return_address (0)); \
	}
