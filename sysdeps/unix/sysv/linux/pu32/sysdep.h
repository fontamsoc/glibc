// SPDX-License-Identifier: GPL-2.0-only
// (c) William Fonkou Tambe

#ifndef __LINUX_PU32_SYSDEP_H
#define __LINUX_PU32_SYSDEP_H 1

#include <sysdeps/unix/sysv/linux/generic/sysdep.h>
#include <sysdeps/pu32/sysdep.h>

/* For Linux the system-call table in the header file
   /usr/include/asm/unistd.h of the kernel is used;
   but these symbols do not follow the SYS_* syntax
   so the `SYS_ify' macro is redefined here. */
#undef SYS_ify
#define SYS_ify(syscall_name) __NR_##syscall_name

/* In PU32 ABI, function call arguments are passed in registers
   %1...%7. The return value is stored in %1.
   Linux syscall uses the same convention with the addition
   that the syscall number is passed in %sr. To enter the kernel,
   the "syscall" instruction is used. */

#ifdef __ASSEMBLER__

/* Linux uses a negative return value to indicate syscall errors.
   Since version 2.1 the return value of a system-call might be
   negative even if the call succeeded.  E.g., the `lseek' system-call
   might return a large offset.  Therefore we must not anymore test
   for < 0, but test for a real error by making sure the value in %1
   is a real error number.  Linus said he will make sure the no syscall
   returns a value in -1...-4095 as a valid result so we can safely
   test with -4095. */
#undef PSEUDO
#define PSEUDO(name, syscall_name, _)  \
	.text;                         \
	ENTRY(name)                    \
	li %sr, SYS_ify(syscall_name); \
	syscall;                       \
	li %9, -4095;                  \
	sgtu %9, %1;                   \
	rli %sr, 0f;                   \
	jnz %9, %sr;                   \
	not %1, %1; inc8 %1, 1;        \
	inc8 %sp, -4; st32 %1, %sp;    \
	inc8 %sp, -4; st32 %rp, %sp;   \
	rli %sr, __errno_location;     \
	jl %rp, %sr;                   \
	ld32 %rp, %sp; inc8 %sp, 4;    \
	ld32 %9, %sp; inc8 %sp, 4;     \
	st32 %9, %1;                   \
	li %1, -1;                     \
	0: j %rp

#undef ret
#define ret

#undef PSEUDO_END
#define PSEUDO_END(name) \
	END(name)

#undef PSEUDO_NOERRNO
#define PSEUDO_NOERRNO(name, syscall_name, _) \
	.text;                                \
	ENTRY(name)                           \
	li %sr, SYS_ify(syscall_name);        \
	syscall;                              \
	j %rp

#undef ret_NOERRNO
#define ret_NOERRNO

#undef PSEUDO_END_NOERRNO
#define PSEUDO_END_NOERRNO(name) \
	END(name)

#undef  PSEUDO_ERRVAL
#define PSEUDO_ERRVAL(name, syscall_name, _) \
	PSEUDO_NOERRNO(name, syscall_name, _)

#undef ret_ERRVAL
#define ret_ERRVAL

#undef  PSEUDO_END_ERRVAL
#define PSEUDO_END_ERRVAL(name) \
	END(name)

#else /* __ASSEMBLER__ */

#define __pu32_syscall0(name, ...) ({                             \
	register void *_a1 __asm__("%1");                             \
	/* set %sr last to avoid it getting overwritten */            \
	__asm__ __volatile__("" ::: "%sr", "memory"); \
	register void *_a0 __asm__("%sr") = (void *)(name); \
	__asm__ __volatile__(                                         \
		"syscall"                                                 \
		: "=r"(_a1)                                               \
		: "r"(_a0)                                                \
		: "memory");                                              \
	__asm__ __volatile__("" ::: "%sr", "memory"); \
	(long)_a1; })

#define __pu32_syscall1(name, a1) ({                              \
	_Pragma("GCC diagnostic push");                               \
	_Pragma("GCC diagnostic ignored \"-Wint-conversion\"");       \
	_Pragma("GCC diagnostic ignored \"-Wint-to-pointer-cast\"");  \
	/* conform to ABI: an argument with a size greater than sizeof(void *) must be passed by reference */                      \
	register void *_a1 __asm__("%1") = ((sizeof(__typeof__(a1)) > sizeof(void *)) ? (void *)&((__typeof__(a1)){(a1)}) : (void *)(a1)); \
	_Pragma("GCC diagnostic pop");                                \
	/* set %sr last to avoid it getting overwritten */            \
	__asm__ __volatile__("" ::: "%sr", "memory"); \
	register void *_a0 __asm__("%sr") = (void *)(name); \
	__asm__ __volatile__(                                         \
		"syscall"                                                 \
		: "+r"(_a1)                                               \
		: "r"(_a0)                                                \
		: "memory");                                              \
	__asm__ __volatile__("" ::: "%sr", "memory"); \
	(long)_a1; })

#define __pu32_syscall2(name, a1, a2) ({                          \
	_Pragma("GCC diagnostic push");                               \
	_Pragma("GCC diagnostic ignored \"-Wint-conversion\"");       \
	_Pragma("GCC diagnostic ignored \"-Wint-to-pointer-cast\"");  \
	/* conform to ABI: an argument with a size greater than sizeof(void *) must be passed by reference */                      \
	register void *_a1 __asm__("%1") = ((sizeof(__typeof__(a1)) > sizeof(void *)) ? (void *)&((__typeof__(a1)){(a1)}) : (void *)(a1)); \
	register void *_a2 __asm__("%2") = ((sizeof(__typeof__(a2)) > sizeof(void *)) ? (void *)&((__typeof__(a2)){(a2)}) : (void *)(a2)); \
	_Pragma("GCC diagnostic pop");                                \
	/* set %sr last to avoid it getting overwritten */            \
	__asm__ __volatile__("" ::: "%sr", "memory"); \
	register void *_a0 __asm__("%sr") = (void *)(name); \
	__asm__ __volatile__(                                         \
		"syscall"                                                 \
		: "+r"(_a1)                                               \
		: "r"(_a0),                                               \
		  "r"(_a2)                                                \
		: "memory");                                              \
	__asm__ __volatile__("" ::: "%sr", "memory"); \
	(long)_a1; })

#define __pu32_syscall3(name, a1, a2, a3) ({                      \
	_Pragma("GCC diagnostic push");                               \
	_Pragma("GCC diagnostic ignored \"-Wint-conversion\"");       \
	_Pragma("GCC diagnostic ignored \"-Wint-to-pointer-cast\"");  \
	/* conform to ABI: an argument with a size greater than sizeof(void *) must be passed by reference */                      \
	register void *_a1 __asm__("%1") = ((sizeof(__typeof__(a1)) > sizeof(void *)) ? (void *)&((__typeof__(a1)){(a1)}) : (void *)(a1)); \
	register void *_a2 __asm__("%2") = ((sizeof(__typeof__(a2)) > sizeof(void *)) ? (void *)&((__typeof__(a2)){(a2)}) : (void *)(a2)); \
	register void *_a3 __asm__("%3") = ((sizeof(__typeof__(a3)) > sizeof(void *)) ? (void *)&((__typeof__(a3)){(a3)}) : (void *)(a3)); \
	_Pragma("GCC diagnostic pop");                                \
	/* set %sr last to avoid it getting overwritten */            \
	__asm__ __volatile__("" ::: "%sr", "memory"); \
	register void *_a0 __asm__("%sr") = (void *)(name); \
	__asm__ __volatile__(                                         \
		"syscall"                                                 \
		: "+r"(_a1)                                               \
		: "r"(_a0),                                               \
		  "r"(_a2),                                               \
		  "r"(_a3)                                                \
		: "memory");                                              \
	__asm__ __volatile__("" ::: "%sr", "memory"); \
	(long)_a1; })

#define __pu32_syscall4(name, a1, a2, a3, a4) ({                  \
	_Pragma("GCC diagnostic push");                               \
	_Pragma("GCC diagnostic ignored \"-Wint-conversion\"");       \
	_Pragma("GCC diagnostic ignored \"-Wint-to-pointer-cast\"");  \
	/* conform to ABI: an argument with a size greater than sizeof(void *) must be passed by reference */                      \
	register void *_a1 __asm__("%1") = ((sizeof(__typeof__(a1)) > sizeof(void *)) ? (void *)&((__typeof__(a1)){(a1)}) : (void *)(a1)); \
	register void *_a2 __asm__("%2") = ((sizeof(__typeof__(a2)) > sizeof(void *)) ? (void *)&((__typeof__(a2)){(a2)}) : (void *)(a2)); \
	register void *_a3 __asm__("%3") = ((sizeof(__typeof__(a3)) > sizeof(void *)) ? (void *)&((__typeof__(a3)){(a3)}) : (void *)(a3)); \
	register void *_a4 __asm__("%4") = ((sizeof(__typeof__(a4)) > sizeof(void *)) ? (void *)&((__typeof__(a4)){(a4)}) : (void *)(a4)); \
	_Pragma("GCC diagnostic pop");                                \
	/* set %sr last to avoid it getting overwritten */            \
	__asm__ __volatile__("" ::: "%sr", "memory"); \
	register void *_a0 __asm__("%sr") = (void *)(name); \
	__asm__ __volatile__(                                         \
		"syscall"                                                 \
		: "+r"(_a1)                                               \
		: "r"(_a0),                                               \
		  "r"(_a2),                                               \
		  "r"(_a3),                                               \
		  "r"(_a4)                                                \
		: "memory");                                              \
	__asm__ __volatile__("" ::: "%sr", "memory"); \
	(long)_a1; })

#define __pu32_syscall5(name, a1, a2, a3, a4, a5) ({              \
	_Pragma("GCC diagnostic push");                               \
	_Pragma("GCC diagnostic ignored \"-Wint-conversion\"");       \
	_Pragma("GCC diagnostic ignored \"-Wint-to-pointer-cast\"");  \
	/* conform to ABI: an argument with a size greater than sizeof(void *) must be passed by reference */                      \
	register void *_a1 __asm__("%1") = ((sizeof(__typeof__(a1)) > sizeof(void *)) ? (void *)&((__typeof__(a1)){(a1)}) : (void *)(a1)); \
	register void *_a2 __asm__("%2") = ((sizeof(__typeof__(a2)) > sizeof(void *)) ? (void *)&((__typeof__(a2)){(a2)}) : (void *)(a2)); \
	register void *_a3 __asm__("%3") = ((sizeof(__typeof__(a3)) > sizeof(void *)) ? (void *)&((__typeof__(a3)){(a3)}) : (void *)(a3)); \
	register void *_a4 __asm__("%4") = ((sizeof(__typeof__(a4)) > sizeof(void *)) ? (void *)&((__typeof__(a4)){(a4)}) : (void *)(a4)); \
	register void *_a5 __asm__("%5") = ((sizeof(__typeof__(a5)) > sizeof(void *)) ? (void *)&((__typeof__(a5)){(a5)}) : (void *)(a5)); \
	_Pragma("GCC diagnostic pop");                                \
	/* set %sr last to avoid it getting overwritten */            \
	__asm__ __volatile__("" ::: "%sr", "memory"); \
	register void *_a0 __asm__("%sr") = (void *)(name); \
	__asm__ __volatile__(                                         \
		"syscall"                                                 \
		: "+r"(_a1)                                               \
		: "r"(_a0),                                               \
		  "r"(_a2),                                               \
		  "r"(_a3),                                               \
		  "r"(_a4),                                               \
		  "r"(_a5)                                                \
		: "memory");                                              \
	__asm__ __volatile__("" ::: "%sr", "memory"); \
	(long)_a1; })

#define __pu32_syscall6(name, a1, a2, a3, a4, a5, a6) ({          \
	_Pragma("GCC diagnostic push");                               \
	_Pragma("GCC diagnostic ignored \"-Wint-conversion\"");       \
	_Pragma("GCC diagnostic ignored \"-Wint-to-pointer-cast\"");  \
	/* conform to ABI: an argument with a size greater than sizeof(void *) must be passed by reference */                      \
	register void *_a1 __asm__("%1") = ((sizeof(__typeof__(a1)) > sizeof(void *)) ? (void *)&((__typeof__(a1)){(a1)}) : (void *)(a1)); \
	register void *_a2 __asm__("%2") = ((sizeof(__typeof__(a2)) > sizeof(void *)) ? (void *)&((__typeof__(a2)){(a2)}) : (void *)(a2)); \
	register void *_a3 __asm__("%3") = ((sizeof(__typeof__(a3)) > sizeof(void *)) ? (void *)&((__typeof__(a3)){(a3)}) : (void *)(a3)); \
	register void *_a4 __asm__("%4") = ((sizeof(__typeof__(a4)) > sizeof(void *)) ? (void *)&((__typeof__(a4)){(a4)}) : (void *)(a4)); \
	register void *_a5 __asm__("%5") = ((sizeof(__typeof__(a5)) > sizeof(void *)) ? (void *)&((__typeof__(a5)){(a5)}) : (void *)(a5)); \
	register void *_a6 __asm__("%6") = ((sizeof(__typeof__(a6)) > sizeof(void *)) ? (void *)&((__typeof__(a6)){(a6)}) : (void *)(a6)); \
	_Pragma("GCC diagnostic pop");                                \
	/* set %sr last to avoid it getting overwritten */            \
	__asm__ __volatile__("" ::: "%sr", "memory"); \
	register void *_a0 __asm__("%sr") = (void *)(name); \
	__asm__ __volatile__(                                         \
		"syscall"                                                 \
		: "+r"(_a1)                                               \
		: "r"(_a0),                                               \
		  "r"(_a2),                                               \
		  "r"(_a3),                                               \
		  "r"(_a4),                                               \
		  "r"(_a5),                                               \
		  "r"(_a6)                                                \
		: "memory");                                              \
	__asm__ __volatile__("" ::: "%sr", "memory"); \
	(long)_a1; })

#undef INTERNAL_SYSCALL
#define INTERNAL_SYSCALL(name, nr, args...) \
	__pu32_syscall##nr(SYS_ify(name), args)

// The _NCS variant is used when the syscall number
// is a non-constant.
#undef INTERNAL_SYSCALL_NCS
#define INTERNAL_SYSCALL_NCS(number, nr, args...) \
	__pu32_syscall##nr(number, args)

// Pointer mangling is not supported for PU32.
#define PTR_MANGLE(var) (void)(var)
#define PTR_DEMANGLE(var) (void)(var)

#endif /* __ASSEMBLER__ */

#endif /* __LINUX_PU32_SYSDEP_H */
