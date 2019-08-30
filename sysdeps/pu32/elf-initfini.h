// SPDX-License-Identifier: GPL-2.0-only
// (c) William Fonkou Tambe

/* Determine DT_INIT/DT_FINI support in the dynamic loader.
   If 0, the dynamic linker ignores the DT_INIT and DT_FINI tags,
   and static binaries will not call the _init or _fini functions.
   If 1, the old constructor mechanisms are used in addition to
   the initarray/finiarray support.  */
#define ELF_INITFINI 1
