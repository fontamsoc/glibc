// SPDX-License-Identifier: GPL-2.0-only
// (c) William Fonkou Tambe

#ifndef _ATOMIC_MACHINE_H
#define _ATOMIC_MACHINE_H 1

#define __HAVE_64B_ATOMICS 0
#define USE_ATOMIC_COMPILER_BUILTINS 0
#define ATOMIC_EXCHANGE_USES_CAS 0

#define atomic_exchange_acq(p, n) \
({ \
	__typeof__(p) _p_ = (p); \
	__typeof__(n) _n_ = (n); \
	switch (sizeof(*(p))) { \
		case 1: \
			__asm__ __volatile__ ( \
				"ldst8 %0, %1" \
				: "+r" (_n_) \
				: "r" (_p_)); \
			break; \
		case 2: \
			__asm__ __volatile__ ( \
				"ldst16 %0, %1" \
				: "+r" (_n_) \
				: "r" (_p_)); \
			break; \
		case 4: \
			__asm__ __volatile__ ( \
				"ldst32 %0, %1" \
				: "+r" (_n_) \
				: "r" (_p_)); \
			break; \
		default: \
			abort(); \
	} \
	_n_; \
})

#define atomic_exchange_rel \
	atomic_exchange_acq

#define atomic_compare_and_exchange_val_acq(p, n, o) \
({ \
	__typeof__(p) _p_ = (p); \
	__typeof__(n) _n_ = (n); \
	__typeof__(o) _o_ = (o); \
	switch (sizeof(*(p))) { \
		case 1: \
			__asm__ __volatile__ ( \
				"cpy %%sr, %2; cldst8 %0, %1" \
				: "+r" (_n_) \
				: "r" (_p_), \
				  "r" (_o_)); \
			break; \
		case 2: \
			__asm__ __volatile__ ( \
				"cpy %%sr, %2; cldst16 %0, %1" \
				: "+r" (_n_) \
				: "r" (_p_), \
				  "r" (_o_)); \
			break; \
		case 4: \
			__asm__ __volatile__ ( \
				"cpy %%sr, %2; cldst32 %0, %1" \
				: "+r" (_n_) \
				: "r" (_p_), \
				  "r" (_o_)); \
			break; \
		default: \
			abort(); \
	} \
	_n_; \
})

#define atomic_compare_and_exchange_val_rel \
	atomic_compare_and_exchange_val_acq

#define __arch_compare_and_exchange_bool_acq(m, n, o) \
	((atomic_compare_and_exchange_val_acq(m, n, o) == (o)) ? 0 : 1)

#endif /* _ATOMIC_MACHINE_H */
