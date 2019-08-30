// SPDX-License-Identifier: GPL-2.0-only
// (c) William Fonkou Tambe

#ifndef _PU32_DL_TLS_H
#define _PU32_DL_TLS_H 1

// We have no tls_index type.
#define DONT_USE_TLS_INDEX 1

// On PU32 the __tls_get_addr function takes
// the module ID and the offset as parameters.
#define GET_ADDR_ARGS           size_t tls_pu32_m, size_t tls_pu32_offset
#define GET_ADDR_PARAM          tls_pu32_m, tls_pu32_offset
#define GET_ADDR_MODULE         tls_pu32_m
#define GET_ADDR_OFFSET         tls_pu32_offset

extern void *__tls_get_addr (size_t m, size_t offset);

#endif /* _PU32_DL_TLS_H */
