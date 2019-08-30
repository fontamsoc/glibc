// SPDX-License-Identifier: GPL-2.0-only
// (c) William Fonkou Tambe

#include <sysdep.h>

/* int clone (
	int (*fn)(void *),
	void *child_stack,
	int flags,
	void *arg,
	pid_t *ptid,
	void *newtls,
	pid_t *ctid);

   kernel: SYSCALL_DEFINE5(clone,
	unsigned long, clone_flags,
	unsigned long, newsp,
	int __user *, parent_tidptr,
	int __user *, child_tidptr,
	unsigned long, tls); */

        .text

ENTRY (__clone)
#define swap(R1,R2) \
	xor R1, R2; xor R2, R1; xor R1, R2
	swap(%1, %3)
	swap(%5, %3)
	swap(%7, %4)
	swap(%6, %5)
#define cpush(R) \
	inc8 %2, -4; st32 R, %2
	cpush(%6) /* save int(*fn)(void *) in the child stack */
	cpush(%7) /* save void*arg in the child stack */
	li %sr, SYS_ify(clone)
	syscall
	rli %sr, 0f
	jnz %1, %sr
#define cpop(R) \
	ld32 R, %sp; inc8 %sp, 4
	cpop(%1) /* pop void*arg from the stack */
	cpop(%sr) /* pop int(*fn)(void *) from the stack */
	jl %rp, %sr
	li %sr, SYS_ify(exit)
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
0:	j %rp
END(__clone)

libc_hidden_def (__clone)
weak_alias (__clone,clone)
