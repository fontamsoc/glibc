// SPDX-License-Identifier: GPL-2.0-only
// (c) William Fonkou Tambe

#include <csu/libc-tls.c>

/* On PU32, linker optimizations are not required, so __tls_get_addr
   can be called even in statically linked binaries.  In this case module
   must be always 1 and PT_TLS segment exist in the binary, otherwise it
   would not link.  */

void *__tls_get_addr (size_t m, size_t offset) {
	dtv_t *dtv = THREAD_DTV();
	return ((char *)dtv[1].pointer.val + offset);
}
