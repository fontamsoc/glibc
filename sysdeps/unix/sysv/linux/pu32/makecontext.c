// SPDX-License-Identifier: GPL-2.0-only
// (c) William Fonkou Tambe

#include <stdarg.h>
#include <ucontext.h>

// Number of arguments that go in registers.
#define NREG_ARGS 7

void __makecontext (ucontext_t *ucp, void (*func) (void), int argc, ...) {

	unsigned long *sp = (unsigned long *)
		(((unsigned long)ucp->uc_stack.ss_sp + ucp->uc_stack.ss_size) & ~7);

	if (argc > NREG_ARGS)
		sp -= (argc - NREG_ARGS);

	ucp->uc_mcontext.regs.sp = (unsigned long)sp;
	// Not correct but using callee saved register %fp to store ucp->uc_link.
	ucp->uc_mcontext.regs.fp = (unsigned long)ucp->uc_link;
	ucp->uc_mcontext.regs.pc = (unsigned long)func;

	extern void __startcontext (void);
	ucp->uc_mcontext.regs.rp = (unsigned long)__startcontext;

	va_list vl;
	va_start (vl, argc);

	unsigned long *r = &ucp->uc_mcontext.regs.r1;

	unsigned long n = 0;

	// The first NREG_ARGS arguments go into registers.
	for (; (n < argc) && (n < NREG_ARGS); ++n)
		*r++ = va_arg (vl, unsigned long);

	// The remainder arguments go on the stack.
	for (; n < argc; ++n)
		*sp++ = va_arg (vl, unsigned long);

	va_end (vl);
}
weak_alias (__makecontext, makecontext)
