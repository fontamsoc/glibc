#include <sys/types.h>
#include <linux/fcntl.h>

#ifdef __USE_GNU
# include <bits/types/struct_iovec.h>
#endif

#define F_OWNER_GID F_OWNER_PGRP

/* File handle structure.  */
struct file_handle
{
  unsigned int handle_bytes;
  int handle_type;
  /* File identifier.  */
  unsigned char f_handle[0];
};
