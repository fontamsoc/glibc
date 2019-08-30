// SPDX-License-Identifier: GPL-2.0-only
// (c) William Fonkou Tambe

#include "sysdep.h"

/* This source produces 3 functions:
   __sigsetjmp (jmpbuf,savemask)
   setjmp (jmpbuf) - equivalent to __sigsetjump (jmpbuf, 1)
   _setjmp (jmpbuf) - equivalent to __sigsetjump (jmpbuf, 0) */

#define save(R) \
	st32 R, %1; inc8 %1, 4

/* jmpbuf layout:
   Register    jmpbuf offset
   %1          0x00
   %0          0x04
   %3          0x08
   %4          0x0c
   %5          0x10
   %6          0x14
   %7          0x18
   %8          0x1c
   %9          0x20
   %10         0x24
   %11         0x28
   %12         0x2c
   %13         0x30
   %14         0x34
   %15         0x38
   %2          0x3c */
#define saveregs \
	save(%1); \
	save(%0); \
	save(%3); \
	save(%4); \
	save(%5); \
	save(%6); \
	save(%7); \
	save(%8); \
	save(%9); \
	save(%10); \
	save(%11); \
	save(%12); \
	save(%13); \
	save(%14); \
	save(%15); \
	st32 %2, %1 /* save(%2) */ \

	.text

ENTRY(setjmp)
	saveregs
	inc8 %1, -(15*__SIZEOF_POINTER__)
	li8 %2, 1
	rli %sr, __sigjmp_save
	j %sr
END(setjmp)

ENTRY(_setjmp)
	saveregs
	inc8 %1, -(15*__SIZEOF_POINTER__)
	li8 %2, 0
	rli %sr, __sigjmp_save
	j %sr
END(_setjmp)
libc_hidden_def (_setjmp)

ENTRY(__sigsetjmp)
	saveregs
	inc8 %1, -(15*__SIZEOF_POINTER__)
	rli %sr, __sigjmp_save
	j %sr
END(__sigsetjmp)
libc_hidden_def (__sigsetjmp)
