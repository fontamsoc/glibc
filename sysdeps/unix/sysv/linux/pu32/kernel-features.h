// SPDX-License-Identifier: GPL-2.0-only
// (c) William Fonkou Tambe

#include <endian.h>
#include_next <kernel-features.h>

#undef __ASSUME_SYSVIPC_DEFAULT_IPC_64

// Define that PU32 is a ILP32 ABI to set the correct interface
// to pass 64-bits values through syscalls.
#define __ASSUME_WORDSIZE64_ILP32 1
