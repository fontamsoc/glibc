// SPDX-License-Identifier: GPL-2.0-only
// (c) William Fonkou Tambe

#ifndef dl_machine_h
#define dl_machine_h 1

#define ELF_MACHINE_NAME "pu32"

// Return nonzero iff ELF header is compatible with the running host.
static inline int
	elf_machine_matches_host (const Elf32_Ehdr *ehdr) {
	return (ehdr->e_machine == EM_PU32);
}

/* Return the link-time address of _DYNAMIC.
Conveniently, this is the first element of the GOT,
a special entry that is never relocated. */
static inline Elf32_Addr
	elf_machine_dynamic (void) {
	extern const Elf32_Addr _GLOBAL_OFFSET_TABLE_[] attribute_hidden;
	return _GLOBAL_OFFSET_TABLE_[0];
}

// Return the run-time load address of the shared object.
static inline Elf32_Addr
	elf_machine_load_address (void) {
	/* Compute the difference between the runtime address
	of _DYNAMIC as seen by an IP-relative reference, and
	the link-time address found in the special unrelocated
	first GOT entry. */
	extern Elf32_Dyn _DYNAMIC[] attribute_hidden;
	return ((Elf32_Addr)&_DYNAMIC - elf_machine_dynamic());
}

#define elf_machine_fixup_plt(map, t, refsym, sym, reloc, reloc_addr, value) \
	((void)refsym, *(Elf32_Addr *)reloc_addr = (Elf32_Addr)value)
#define elf_machine_plt_value(map, reloc, value) (value)
#define elf_machine_runtime_setup(l, scope, lazy, profile) (lazy)

// The actual _start code is in dl-start.S .
// Assembly to let dl-start.o see _dl_start.
#define RTLD_START asm (".globl _dl_start");

#define elf_machine_type_class(type) \
	(((type) == R_PU32_32) * ELF_RTYPE_CLASS_COPY)

#define ELF_MACHINE_JMP_SLOT R_PU32_NONE

// PU32 never uses Elf32_Rel relocations.
#define ELF_MACHINE_NO_REL 1
#define ELF_MACHINE_NO_RELA 0
#define PLTREL ElfW(Rela)

#endif /* dl_machine_h */

#define ARCH_LA_PLTENTER pu32_gnu_pltenter
#define ARCH_LA_PLTEXIT pu32_gnu_pltexit

#ifdef RESOLVE_MAP

#include <abort-instr.h>

// Perform the relocation specified by RELOC and SYM (which is fully resolved).
// MAP is the object containing the reloc.
static inline void __attribute__ ((always_inline))
	elf_machine_rela (
		struct link_map *map,
		struct r_scope_elem *scope[],
		const Elf32_Rela *reloc,
		const Elf32_Sym *sym,
		const struct r_found_version *version,
		void *const reloc_addr_arg,
		int skip_ifunc) {
	unsigned long r_type = ELF32_R_TYPE(reloc->r_info);
	if (r_type == R_PU32_32) {
		struct link_map *sym_map = RESOLVE_MAP (map, scope, &sym, version, r_type);
		if (ELF32_ST_TYPE(sym->st_info) == STT_TLS) {
			#ifdef RTLD_BOOTSTRAP
			// During startup the dynamic linker is always index 1.
			*(Elf32_Addr *)reloc_addr_arg = 1;
			#else
			if (sym_map != NULL)
				*(Elf32_Addr *)reloc_addr_arg = sym_map->l_tls_modid;
			#endif
		} else
			*(Elf32_Addr *)reloc_addr_arg =
				SYMBOL_ADDRESS (sym_map, sym, true);
	} else
		_dl_reloc_bad_type (map, r_type, 0);
}

static inline void __attribute__ ((always_inline))
	elf_machine_rela_relative (
		Elf32_Addr l_addr,
		const Elf32_Rela *reloc,
		void *const reloc_addr_arg) {
	_dl_error_printf (
		"%s: %s: unexpected\n",
		__FUNCTION__, RTLD_PROGNAME);
	ABORT_INSTRUCTION;
}

static inline void __attribute__ ((always_inline))
	elf_machine_lazy_rel (
		struct link_map *map,
		struct r_scope_elem *scope[],
		Elf32_Addr l_addr,
		const Elf32_Rela *reloc,
		int skip_ifunc) {
	_dl_error_printf (
		"%s: %s: unexpected\n",
		__FUNCTION__, RTLD_PROGNAME);
	ABORT_INSTRUCTION;
}

#endif /* RESOLVE_MAP */
