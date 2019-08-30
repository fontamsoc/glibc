// SPDX-License-Identifier: GPL-2.0-only
// (c) William Fonkou Tambe

#ifndef _FENV_H
# error "Never use <bits/fenv.h> directly; include <fenv.h> instead."
#endif

#define FE_INEXACT (1<<0)
#define FE_DIVBYZERO (1<<1)
#define FE_UNDERFLOW (1<<2)
#define FE_OVERFLOW (1<<3)
#define FE_INVALID (1<<4)

// PU32 does not support floating-point exceptions.
#define FE_ALL_EXCEPT 0

enum {
	FE_TONEAREST  =
	#define FE_TONEAREST  (0x0)   /* default */
	FE_TONEAREST,
	FE_TOWARDZERO =
	#define FE_TOWARDZERO (0x1)
	FE_TOWARDZERO,
	FE_DOWNWARD   =
	#define FE_DOWNWARD   (0x2)
	FE_DOWNWARD,
	FE_UPWARD     =
	#define FE_UPWARD     (0x3)
	FE_UPWARD,
};

// Type representing exception flags (if there were any).
typedef unsigned int fexcept_t;

// Type representing floating-point environment.
typedef unsigned int fenv_t;

// If the default argument is used, we use this value.
#define FE_DFL_ENV ((const fenv_t *)-1l)

#if __GLIBC_USE(IEC_60559_BFP_EXT_C2X)

// Type representing floating-point control modes.
typedef unsigned int femode_t;

// Default floating-point control modes.
#define FE_DFL_MODE ((const femode_t *)-1L)

#endif
