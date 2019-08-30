/* MT support function to get address of `errno' variable, non-threaded
   version.
   Copyright (C) 1996-2020 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.  */

#include <errno.h>
#include <tls.h>
#include <libc-diag.h>

DIAG_PUSH_NEEDS_COMMENT;
DIAG_IGNORE_NEEDS_COMMENT (12, "-Winfinite-recursion");

int *
__errno_location (void)
{
  return &errno;
}
libc_hidden_def (__errno_location)

DIAG_POP_NEEDS_COMMENT;
