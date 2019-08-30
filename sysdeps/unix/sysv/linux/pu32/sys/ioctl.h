// SPDX-License-Identifier: GPL-2.0-only
// (c) William Fonkou Tambe

#ifndef	_SYS_IOCTL_H
#define	_SYS_IOCTL_H 1

#include <features.h>

__BEGIN_DECLS

/* Get the list of `ioctl' requests and related constants.  */
#include <bits/ioctls.h>

/* Define some types used by `ioctl' requests.  */
#include <bits/ioctl-types.h>

/* On a Unix system, the system <sys/ioctl.h> probably defines some of
   the symbols we define in <sys/ttydefaults.h> (usually with the same
   values).  The code to generate <bits/ioctls.h> has omitted these
   symbols to avoid the conflict, but a Unix program expects <sys/ioctl.h>
   to define them, so we must include <sys/ttydefaults.h> here.  */
#include <sys/ttydefaults.h>

/* Perform the I/O control operation specified by REQUEST on FD.
   One argument may follow; its presence and type depend on REQUEST.
   Return value depends on REQUEST.  Usually -1 indicates error.  */
extern int __ioctl (int __fd, unsigned long int __request, ...);
#define ioctl __ioctl

__END_DECLS

#endif /* sys/ioctl.h */
