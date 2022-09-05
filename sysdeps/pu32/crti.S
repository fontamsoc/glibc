// SPDX-License-Identifier: GPL-2.0-only
// (c) William Fonkou Tambe

#include <sysdep.h>

#define push(R) \
	inc8 %sp, -4; st32 R, %sp

.file "crti.S"

.section .init
ENTRY(_init)
	push(%rp)
	push(%fp)

.section .fini
ENTRY(_fini)
	push(%rp)
	push(%fp)
