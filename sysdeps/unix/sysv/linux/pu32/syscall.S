// SPDX-License-Identifier: GPL-2.0-only
// (c) William Fonkou Tambe

#include <sysdep.h>

        .text

ENTRY (syscall)
	cpy %sr, %1
	cpy %1, %2
	cpy %2, %3
	cpy %3, %4
	cpy %4, %5
	cpy %5, %6
	cpy %6, %7
	syscall
/* Linux uses a negative return value to indicate syscall errors.
   Since version 2.1 the return value of a system-call might be
   negative even if the call succeeded.  E.g., the `lseek' system-call
   might return a large offset.  Therefore we must not anymore test
   for < 0, but test for a real error by making sure the value in %1
   is a real error number.  Linus said he will make sure the no syscall
   returns a value in -1...-4095 as a valid result so we can safely
   test with -4095. */
	li %9, -4095
	sgtu %9, %1
	rli %sr, 0f
	jnz %9, %sr
	not %1, %1
	inc8 %1, 1
	inc8 %sp, -4
	st32 %1, %sp
	inc8 %sp, -4
	st32 %rp, %sp
	rli %sr, __errno_location
	jl %rp, %sr
	ld32 %rp, %sp
	inc8 %sp, 4
	ld32 %9, %sp
	inc8 %sp, 4
	st32 %9, %1
	li %1, -1
0:	j %rp
END(syscall)
