// SPDX-License-Identifier: GPL-2.0-only
// (c) William Fonkou Tambe

#define RTLD_PRIVATE_ERRNO 0

#ifndef __ASSEMBLER__
extern int _dl_discover_osversion (void) attribute_hidden;
# define HAVE_DL_DISCOVER_OSVERSION 1
#endif
