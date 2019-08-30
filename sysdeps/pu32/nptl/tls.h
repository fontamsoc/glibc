// SPDX-License-Identifier: GPL-2.0-only
// (c) William Fonkou Tambe

#ifndef _PU32_TLS_H
#define _PU32_TLS_H 1

#include <dl-sysdep.h>

#ifndef __ASSEMBLER__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <dl-dtv.h>
#include <sysdep.h>

// The thread local storage pointer
// points to a thread control block.
#define TLS_TCB_AT_TP 1
#define TLS_DTV_AT_TP 0

typedef struct {
	dtv_t *dtv;	/* Using TCB variant 1 layout where offset zero contains
			   a pointer to the dynamic thread vector for the thread. */
	void *self;	// Pointer to the thread descriptor.
	void *tcb;	/* Pointer to the TCB. Not necessarily
			   the thread descriptor used by libpthread. */
	int multiple_threads;
	int gscope_flag;
} tcbhead_t;

// This is the size of the initial TCB.
#define TLS_INIT_TCB_SIZE sizeof (struct pthread)

// This is the size of the TCB.
#define TLS_TCB_SIZE TLS_INIT_TCB_SIZE

// Alignment requirements for the initial TCB.
#define TLS_INIT_TCB_ALIGN __alignof__ (struct pthread)

// Alignment requirements for the TCB.
#define TLS_TCB_ALIGN TLS_INIT_TCB_ALIGN

// Code to initially initialize the thread local storage pointer.
#define TLS_INIT_TP(tcbp) ({ \
	((tcbhead_t *)(tcbp))->self = tcbp; \
	((tcbhead_t *)(tcbp))->tcb = tcbp; \
	__pu32_syscall1 (__NR_settls, tcbp); \
	NULL; })

// Value passed to 'clone' for initialization
// of the thread local storage pointer.
#define TLS_DEFINE_INIT_TP(tp, pd) void *tp = (pd)

// Return the address of the dtv for the current thread.
#define THREAD_DTV() \
	((tcbhead_t *)__pu32_syscall0(__NR_gettls))->dtv

// Return the thread descriptor for the current thread.
#define THREAD_SELF \
	((struct pthread *)__pu32_syscall0(__NR_gettls))

// Install the dtv pointer.
// The pointer passed is to the element
// with index -1 which contain the length.
#define INSTALL_DTV(tcbp, dtvp) \
	(((tcbhead_t *)(tcbp))->dtv = (dtvp) + 1)

// Install new dtv for current thread.
#define INSTALL_NEW_DTV(dtv) \
	(THREAD_DTV() = (dtv))

// Return dtv of given thread descriptor.
#define GET_DTV(tcbp) \
	((tcbhead_t *)(tcbp))->dtv

// Macros for accessing thread descriptor data.
#define THREAD_GETMEM(descr, member) \
	descr->member
#define THREAD_GETMEM_NC(descr, member, idx) \
	descr->member[idx]
#define THREAD_SETMEM(descr, member, value) \
	descr->member = (value)
#define THREAD_SETMEM_NC(descr, member, idx, value) \
	descr->member[idx] = (value)

// l_tls_offset == 0 is perfectly valid, so we have
// to use some different value to mean unset l_tls_offset.
#define NO_TLS_OFFSET -1

/* Get and set the global scope generation counter in struct pthread.  */
#define THREAD_GSCOPE_IN_TCB      1
#define THREAD_GSCOPE_FLAG_UNUSED 0
#define THREAD_GSCOPE_FLAG_USED   1
#define THREAD_GSCOPE_FLAG_WAIT   2
#define THREAD_GSCOPE_RESET_FLAG()					\
	do {								\
		int __res = atomic_exchange_rel (			\
			&THREAD_SELF->header.gscope_flag,		\
			THREAD_GSCOPE_FLAG_UNUSED);			\
		if (__res == THREAD_GSCOPE_FLAG_WAIT)			\
			lll_futex_wake (				\
				&THREAD_SELF->header.gscope_flag,	\
				1, LLL_PRIVATE);			\
	} while (0)
#define THREAD_GSCOPE_SET_FLAG() 						\
	do {									\
		THREAD_SELF->header.gscope_flag = THREAD_GSCOPE_FLAG_USED;	\
		atomic_write_barrier();						\
	} while (0)
#define THREAD_GSCOPE_WAIT() \
	GL(dl_wait_lookup_done)()

// Get the thread descriptor definition.
#include <nptl/descr.h>

#endif /* __ASSEMBLER__ */

#endif	/* _PU32_TLS_H */
