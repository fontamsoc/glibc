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

// PU32 supports only round-to-nearest.
enum {
	FE_TONEAREST =
	#define FE_TONEAREST 0
	FE_TONEAREST,
	__FE_UNDEFINED = -1,
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
