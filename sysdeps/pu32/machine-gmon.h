// SPDX-License-Identifier: GPL-2.0-only
// (c) William Fonkou Tambe

#include <sysdep.h>

#define _MCOUNT_DECL(frompc, selfpc) \
	static inline void __mcount_internal ( \
		unsigned long frompc, \
		unsigned long selfpc)

#define MCOUNT \
	void _mcount (void *frompc) { \
		__mcount_internal ( \
			(unsigned long) frompc, \
			(unsigned long) __builtin_return_address (0)); \
	}
