// SPDX-License-Identifier: GPL-2.0-only
// (c) William Fonkou Tambe

#define GETSP() ({ register uintptr_t sp asm ("%sp"); sp; })

#define GETTIME(LO,HI) ({ \
	inline unsigned long get_cycles_hi (void) { \
		unsigned long hi; \
		asm volatile ("getclkcyclecnth %0\n" : "=r"(hi)); \
		return hi; \
	} \
	unsigned long lo, hi; \
	do { \
		hi = get_cycles_hi(); \
		asm volatile ("getclkcyclecnt %0\n" : "=r"(lo)); \
	} while (hi != get_cycles_hi()); \
	(HI) = hi; (LO) = lo; })

#include <sysdeps/generic/memusage.h>
