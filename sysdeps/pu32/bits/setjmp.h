// SPDX-License-Identifier: GPL-2.0-only
// (c) William Fonkou Tambe

#ifndef _BITS_SETJMP_H
#define _BITS_SETJMP_H 1

#if !defined(_SETJMP_H) && !defined(_PTHREAD_H)
# error "Never include <bits/setjmp.h> directly; use <setjmp.h> instead."
#endif

typedef unsigned long __jmp_buf[16 /* Total number of PU32 GPRs */];

#endif /* _BITS_SETJMP_H */
