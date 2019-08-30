// SPDX-License-Identifier: GPL-2.0-only
// (c) William Fonkou Tambe

#include <setjmp.h>
#include <jmpbuf-offsets.h>
#include <stdint.h>
#include <unwind.h>

// Test if longjmp to JMPBUF would unwind the
// frame containing a local variable at ADDRESS.
#define _JMPBUF_UNWINDS(_jmpbuf, _address, _demangle) \
	((void *)(_address) < (void *)_demangle(((unsigned long *)_jmpbuf)[JB_SP]))

#define _JMPBUF_UNWINDS_ADJ(_jmpbuf, _address, _adj) \
	((uintptr_t)(_address) - (_adj) < (uintptr_t)(((unsigned long *)_jmpbuf)[JB_SP]) - (_adj))

#define _JMPBUF_CFA_UNWINDS_ADJ(_jmpbuf, _context, _adj) \
	_JMPBUF_UNWINDS_ADJ (_jmpbuf, (void *)_Unwind_GetCFA(_context), _adj)

// We use the normal longjmp for unwinding.
#define __libc_unwind_longjmp(buf, val) __libc_longjmp (buf, val)
