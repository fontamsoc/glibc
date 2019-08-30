// SPDX-License-Identifier: GPL-2.0-only
// (c) William Fonkou Tambe

#include "sysdep.h"

/* This source produces:
   void __longjmp (__jmp_buf env, int val)
   Jump to the position specified by ENV, causing the
   setjmp call there to return VAL, or 1 if VAL is 0. */

#define restore(R) \
	ld32 R, %1; inc8 %1, 4

#define swap(R1,R2) \
	xor R1, R2; xor R2, R1; xor R1, R2

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
ENTRY(__longjmp)
	inc8 %1, 4 /* skip restore(%1) */
	restore(%0)
	restore(%3)
	restore(%4)
	restore(%5)
	restore(%6)
	restore(%7)
	restore(%8)
	restore(%9)
	restore(%10)
	restore(%11)
	restore(%12)
	restore(%13)
	restore(%14)
	rli %15, 0f
	jz %2, %15
	restore(%15)
	ld32 %1, %1 /* restore(%2) into %1 */
	swap(%1, %2) /* %2 has value to return, while %1 has restore(%2) */
	j %15
0:	restore(%15)
	ld32 %2, %1 /* restore(%2) */
	li8 %1, 1
	j %15
END(__longjmp)
libc_hidden_def (__longjmp)
