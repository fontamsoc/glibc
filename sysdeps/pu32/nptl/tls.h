// SPDX-License-Identifier: GPL-2.0-only
// (c) William Fonkou Tambe

#ifndef _PU32_TLS_H
#define _PU32_TLS_H 1

#ifndef __ASSEMBLER__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <dl-dtv.h>
#include <sysdep.h>

# define TLS_DTV_AT_TP	1
# define TLS_TCB_AT_TP	0

// Get the thread descriptor definition.
#include <nptl/descr.h>

typedef struct {
	dtv_t *dtv;
	void *private;
} tcbhead_t;

// This is the size of the initial TCB.
# define TLS_INIT_TCB_SIZE  sizeof(tcbhead_t)

// This is the size of the TCB.
#define TLS_TCB_SIZE TLS_INIT_TCB_SIZE

// Alignment requirements for the initial TCB.
#define TLS_INIT_TCB_ALIGN __alignof__(tcbhead_t)

// Alignment requirements for the TCB.
#define TLS_TCB_ALIGN TLS_INIT_TCB_ALIGN

#define TLS_PRE_TCB_SIZE  sizeof(struct pthread)

#define TLS_TCB_OFFSET  0

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

// Value passed to 'clone' for initialization of the thread local storage pointer.
# define TLS_DEFINE_INIT_TP(tp, pd) void *tp = ((pd) + 1)

// Code to initially initialize the thread local storage pointer.
#define TLS_INIT_TP(tcbp) ({ \
	__pu32_syscall1 (__NR_settls, ((char *)(tcbp) + TLS_TCB_OFFSET)); \
	NULL; })

// Return the address of the dtv for the current thread.
#define THREAD_DTV() \
	((tcbhead_t *)((unsigned long)__pu32_syscall0(__NR_gettls) \
		- TLS_TCB_OFFSET))->dtv

// Return the thread descriptor for the current thread.
#undef THREAD_SELF
#define THREAD_SELF \
	((struct pthread *)((unsigned long)__pu32_syscall0(__NR_gettls) \
		- TLS_TCB_OFFSET - TLS_PRE_TCB_SIZE))

/* Magic for libthread_db to know how to do THREAD_SELF.  */
# define DB_THREAD_SELF \
	CONST_THREAD_AREA ((8*sizeof(unsigned long)), sizeof(struct pthread))

// Macros for accessing thread descriptor data.
#define THREAD_GETMEM(descr, member) \
	descr->member
#define THREAD_GETMEM_NC(descr, member, idx) \
	descr->member[idx]
#define THREAD_SETMEM(descr, member, value) \
	descr->member = (value)
#define THREAD_SETMEM_NC(descr, member, idx, value) \
	descr->member[idx] = (value)

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

#endif /* __ASSEMBLER__ */

#endif	/* _PU32_TLS_H */
