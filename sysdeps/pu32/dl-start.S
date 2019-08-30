// SPDX-License-Identifier: GPL-2.0-only
// (c) William Fonkou Tambe

#include <sysdep.h>

/* Initial entry point code for the dynamic linker.
The C function _dl_start() is the real entry point;
its return value is the user program's entry point. */

        .text

ENTRY(_start)
	cpy %1, %sp
	#rli %sr, _dl_start # TODO: symbol no longer used.
	jl %rp, %sr
#_dl_start_user:
	# Skip extra leading arguments if we were run as a command.
	#rli %2, _dl_skip_args # TODO: symbol no longer used.
	ld32 %2, %2
	# Pop the original argc.
	ld32 %4, %sp
	inc8 %sp, 4
	# Subtract _dl_skip_args from argc.
	sub %4, %2
	# Adjust %sp to skip _dl_skip_args words.
	li8 %sr, 2
	sll %2, %sr
	add %sp, %2
	# Copy %sp to %3 for _dl_init().
	cpy %3, %sp
	# Push argc back on the stack.
	inc8 %sp, -4
	st32 %4, %sp
	# Save the user-entry-point returned by _dl_start().
	inc8 %sp, -4
	st32 %1, %sp
	# Call _dl_init (struct link_map *main_map, int argc, char **argv, char **env)
	rli %1, _rtld_local
	cpy %2, %4
	sll %4, %sr
	add %4, %3
	rli %sr, _dl_init
	jl %rp, %sr
	# Jump to the user-entry-point passing
	# the finalizer function as argument.
	rli %1, _dl_fini
	ld32 %sr, %sp
	inc8 %sp, 4
	jl %rp, %sr
	# We should never return from above jl,
	# otherwise we must infinite loop brk.
	brk
	j %rp
END(_start)
