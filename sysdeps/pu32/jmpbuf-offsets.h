// SPDX-License-Identifier: GPL-2.0-only
// (c) William Fonkou Tambe

#ifndef  __ASSEMBLER__
#include <setjmp.h>
#endif

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
#define JB_SP 1
// Helper for generic ____longjmp_chk().
#define JB_FRAME_ADDRESS(buf) \
	((void *)(((unsigned long *)buf)[JB_SP]))
