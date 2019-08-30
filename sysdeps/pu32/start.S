// SPDX-License-Identifier: GPL-2.0-only
// (c) William Fonkou Tambe

#include <sysdep.h>

	.text

ENTRY(_start)
	# %sp, %fp and %rp are expected to have been properly set.
	# %1 is expected to be null, or set to a function pointer
	# to be registered with atexit(). This is how the dynamic
	# linker arranges to have DT_FINI functions called for shared
	# libraries that have been loaded before this code runs.

	# Zero section .bss .
	rli %8, __bss_start
	rli %9, __bss_end
	li8 %10, 0
0:
	cpy %11, %8
	sltu %11, %9
	rli %sr, 0f
	jz %11, %sr
	st32 %10, %8 # Write zero.
	inc8 %8, 4
	rli %sr, 0b
	j %sr
0:
	cpy %6, %1 /* rtld_fini */

	# Call __libc_start_main (
	# 	(void *)main,
	#	int argc, char *argv[],
	#	(void *)init,
	#	(void *)fini,
	#	(void *)rtld_fini,
	#	(void *)stack_top);
	# The stack is laid-out as follow:
	# - argc
	# - null-terminated argv pointers array.
	# - null-terminated envp pointers array.
	# - argv null-terminated strings.
	# - envp null-terminated strings.
	rli %1, main
	ld32 %2, %sp
	cpy %3, %sp
	inc8 %3, 4
	rli %4, __libc_csu_init
	rli %5, __libc_csu_fini
	/* %6 == rtld_fini */
	cpy %7, %sp
	rli %sr, __libc_start_main
	jl %rp, %sr
	# We should never return from above jl,
	# otherwise we must infinite loop brk.
	brk
	j %rp

END(_start)
