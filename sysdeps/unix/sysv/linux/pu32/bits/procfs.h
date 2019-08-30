// SPDX-License-Identifier: GPL-2.0-only
// (c) William Fonkou Tambe

#ifndef _SYS_PROCFS_H
#error "Never include <bits/procfs.h> directly; use <sys/procfs.h> instead."
#endif

#include <asm/ptrace.h>

// Type for a general-purpose register.
typedef unsigned long elf_greg_t;
// Type for a floating-point registers.
typedef unsigned long elf_fpreg_t;

#define ELF_NGREG (sizeof (struct pt_regs) / sizeof (elf_greg_t))
typedef elf_greg_t elf_gregset_t[ELF_NGREG];

#define ELF_NFPREG 0
typedef elf_fpreg_t elf_fpregset_t[ELF_NFPREG];
