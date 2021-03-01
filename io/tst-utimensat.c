/* utimensat basic tests.
   Copyright (C) 2021 Free Software Foundation, Inc.
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

#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <support/check.h>
#include <support/support.h>
#include <support/temp_file.h>
#include <support/xunistd.h>

#ifndef struct_stat
# define struct_stat struct stat64
#endif

static int temp_fd = -1;
static char *testfile;
static char *testlink;

/* struct timeval array with Y2038 threshold minus 2 and 1 seconds.  */
const static struct timespec t1[2] = { { 0x7FFFFFFE, 0 },
				       { 0x7FFFFFFF, 0 } };

/* struct timeval array with Y2038 threshold plus 1 and 2 seconds.  */
const static struct timespec t2[2] = { { 0x80000001ULL, 0 },
				       { 0x80000002ULL, 0 } };

/* struct timeval array around Y2038 threshold.  */
const static struct timespec t3[2] = { { 0x7FFFFFFE, 0 },
				       { 0x80000002ULL, 0 } };

static void
do_prepare (int argc, char *argv[])
{
  temp_fd = create_temp_file ("tst-utimensat", &testfile);
  TEST_VERIFY_EXIT (temp_fd > 0);

  testlink = xasprintf ("%s-symlink", testfile);
  xsymlink (testfile, testlink);
  add_temp_file (testlink);
}
#define PREPARE do_prepare

static void
test_utimensat_helper (const struct timespec ts[2])
{
  /* Check if we run on port with 32 bit time_t size */
  {
    time_t t;
    if (__builtin_add_overflow (ts->tv_sec, 0, &t))
      return;
  }

  {
    TEST_VERIFY_EXIT (utimensat (temp_fd, testfile, ts, 0) == 0);

    struct_stat st;
    xfstat (temp_fd, &st);

    /* Check if seconds for atime match */
    TEST_COMPARE (st.st_atime, ts[0].tv_sec);

    /* Check if seconds for mtime match */
    TEST_COMPARE (st.st_mtime, ts[1].tv_sec);
  }

  {
    struct_stat stfile_orig;
    xlstat (testfile, &stfile_orig);

    TEST_VERIFY_EXIT (utimensat (0 /* ignored  */, testlink, ts,
				 AT_SYMLINK_NOFOLLOW)
		       == 0);
    struct_stat stlink;
    xlstat (testlink, &stlink);

    TEST_COMPARE (stlink.st_atime, ts[0].tv_sec);
    TEST_COMPARE (stlink.st_mtime, ts[1].tv_sec);

    /* Check if the timestamp from original file is not changed.  */
    struct_stat stfile;
    xlstat (testfile, &stfile);

    TEST_COMPARE (stfile_orig.st_atime, stfile.st_atime);
    TEST_COMPARE (stfile_orig.st_mtime, stfile.st_mtime);
  }
}

static int
do_test (void)
{
  test_utimensat_helper (t1);
  test_utimensat_helper (t2);
  test_utimensat_helper (t3);

  return 0;
}

#include <support/test-driver.c>
