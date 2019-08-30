// SPDX-License-Identifier: GPL-2.0-only
// (c) William Fonkou Tambe

#ifndef _PU32_LDSODEFS_H
#define _PU32_LDSODEFS_H 1

#include <elf.h>

struct La_pu32_regs;
struct La_pu32_retval;

#define ARCH_PLTENTER_MEMBERS \
  Elf32_Addr (*pu32_gnu_pltenter) (Elf32_Sym *, \
                                   unsigned int, \
                                   uintptr_t *, \
                                   uintptr_t *, \
                                   struct La_pu32_regs *, \
                                   unsigned int *, \
                                   const char *name, \
                                   long int *framesizep);

#define ARCH_PLTEXIT_MEMBERS \
  unsigned int (*pu32_gnu_pltexit) (Elf32_Sym *, \
                                    unsigned int, \
                                    uintptr_t *, \
                                    uintptr_t *, \
                                    const struct La_pu32_regs *, \
                                    struct La_pu32_retval *, \
                                    const char *);

#include_next <ldsodefs.h>

#endif
