// SPDX-License-Identifier: GPL-2.0-only
// (c) William Fonkou Tambe

#define __stringify_(x...)	#x
#define __stringify(x...)	__stringify_(x)

#ifdef __ASSEMBLER__

#undef ENTRY
#define ENTRY(name)            \
	.globl name;           \
	.type name, @function; \
	.p2align 1;            \
	name:

#undef END
#define END(name) .size name, (. - name)

#endif /* __ASSEMBLER__ */
