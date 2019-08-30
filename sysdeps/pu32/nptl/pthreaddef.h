// SPDX-License-Identifier: GPL-2.0-only
// (c) William Fonkou Tambe

// Default stack size.
#define ARCH_STACK_DEFAULT_SIZE (2 * 1024 * 1024)

// Minimum guard size.
#define ARCH_MIN_GUARD_SIZE 0

// Required stack pointer alignment at beginning.
#define STACK_ALIGN 8

// Minimal stack size after allocating thread descriptor and guard size.
#define MINIMAL_REST_STACK 2048

// Location of current stack frame.
#define CURRENT_STACK_FRAME __builtin_frame_address (0)
