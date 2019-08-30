// SPDX-License-Identifier: GPL-2.0-only
// (c) William Fonkou Tambe

#ifndef _SIGCONTEXTINFO_H
#define _SIGCONTEXTINFO_H 1

#include <stdint.h>
#include <sys/ucontext.h>

static inline uintptr_t sigcontext_get_pc (const ucontext_t *ctx) {
	return ctx->uc_mcontext.regs.pc;
}

#endif /* _SIGCONTEXTINFO_H */
