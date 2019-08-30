// SPDX-License-Identifier: GPL-2.0-only
// (c) William Fonkou Tambe

#ifndef _DL_VDSO_H
#define _DL_VDSO_H 1
static inline void *dl_vdso_vsym (const char *name) {
	return NULL;
}
#endif /* _DL_VDSO_H */
