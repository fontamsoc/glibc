// SPDX-License-Identifier: GPL-2.0-only
// (c) William Fonkou Tambe

#include <sysdep.h>
#include "ucontext_i.h"

/* int swapcontext (ucontext_t *oucp, const ucontext_t *ucp) */

	.text

ENTRY(__swapcontext)

	/* Similar to getcontext(). */
	/* Save to UCONTEXT_MCONTEXT */
	li %sr, UCONTEXT_MCONTEXT
	add %sr, %1
	/* Save only %sp, %tp, %fp, %rp, %pc. */
	st32 %0, %sr
	inc8 %sr, -4*10 /* skip %1 %2 %3 %4 %5 %6 %7 %8 %9 */
	st32 %10, %sr
	inc8 %sr, -4*4 /* skip %11 %12 %13 */
	st32 %14, %sr
	inc8 %sr, -4
	st32 %15, %sr
	inc8 %sr, -4
	st32 %15, %sr /* store %rp to %pc */

	/* Save %2 across syscall. */
	inc8 %sp, -4
	st32 %2, %sp

	/* Get signal mask.
	   rt_sigprocmask (SIG_BLOCK, NULL, &ucp->uc_sigmask, _NSIG8) */
	li %4, _NSIG8
	li %3, 0
	li %2, UCONTEXT_SIGMASK
	add %2, %1
	li %1, SIG_BLOCK
	li %sr, SYS_ify(rt_sigprocmask)
	syscall
/* Linux uses a negative return value to indicate syscall errors.
   Since version 2.1 the return value of a system-call might be
   negative even if the call succeeded.  E.g., the `lseek' system-call
   might return a large offset.  Therefore we must not anymore test
   for < 0, but test for a real error by making sure the value in %1
   is a real error number.  Linus said he will make sure the no syscall
   returns a value in -1...-4095 as a valid result so we can safely
   test with -4095. */
0:	li %9, -4095
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
	j %rp
0:	/* Retrieve saved %2 into %1 without poping the stack. */
	ld32 %1, %sp

	/* Same as setcontext(). */
	/* Restore signal mask.
	   rt_sigprocmask (SIG_SETMASK, &ucp->uc_sigmask, NULL, _NSIG8) */
	li %4, _NSIG8
	li %3, 0
	li %2, UCONTEXT_SIGMASK
	add %2, %1
	li %1, SIG_SETMASK
	li %sr, SYS_ify(rt_sigprocmask)
	syscall
/* Linux uses a negative return value to indicate syscall errors.
   Since version 2.1 the return value of a system-call might be
   negative even if the call succeeded.  E.g., the `lseek' system-call
   might return a large offset.  Therefore we must not anymore test
   for < 0, but test for a real error by making sure the value in %1
   is a real error number.  Linus said he will make sure the no syscall
   returns a value in -1...-4095 as a valid result so we can safely
   test with -4095. */
0:	li %9, -4095
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
	inc8 %sp, 4 /* discard saved %1 */
	j %rp
0:	/* restore saved %1 */
	ld32 %1, %sp
	inc8 %sp, 4

	/* restore from UCONTEXT_MCONTEXT */
	li %sr, UCONTEXT_MCONTEXT
	add %sr, %1
	/* Restore only %sp, makecontext() register arguments, %tp, %fp, %rp, %pc. */
	ld32 %0, %sr
	inc8 %sr, 4
	ld32 %1, %sr
	inc8 %sr, 4
	ld32 %2, %sr
	inc8 %sr, 4
	ld32 %3, %sr
	inc8 %sr, 4
	ld32 %4, %sr
	inc8 %sr, 4
	ld32 %5, %sr
	inc8 %sr, 4
	ld32 %6, %sr
	inc8 %sr, 4
	ld32 %7, %sr
	inc8 %sr, 4*3 /* skip %8 %9 */
	ld32 %10, %sr
	inc8 %sr, 4*4 /* skip %11 %12 %13 */
	ld32 %14, %sr
	inc8 %sr, 4
	ld32 %15, %sr
	inc8 %sr, 4
	ld32 %sr, %sr /* retrieve %pc */

	j %sr

END (__swapcontext)
weak_alias (__swapcontext, swapcontext)
