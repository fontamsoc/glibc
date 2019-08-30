// SPDX-License-Identifier: GPL-2.0-only
// (c) William Fonkou Tambe

/*  int __getcontext (ucontext_t *ucp)

  Saves the machine context in UCP such that when it is activated,
  it appears as if __getcontext() returned again.

  This implementation in intended to be used for *synchronous* context
  switches only.  Therefore, it does not have to save anything
  other than the PRESERVED state.  */

#include <sysdep.h>
#include "ucontext_i.h"
