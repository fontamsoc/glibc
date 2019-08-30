#ifndef _SYS_IOCTL_H
#include <misc/sys/ioctl.h>

# ifndef _ISOMAC

/* Now define the internal interfaces.  */
extern int __ioctl (int __fd, unsigned long __request, void *__arg);
#define __IOCTL_VARIANT(_1,_2,_3,_X,...) _X
#define __IOCTL2(fd,req) __ioctl((fd),(req),(void*)0)
#define __IOCTL3(fd,req,arg) __ioctl((fd),(req),(void*)(arg))
#define ioctl(...) __IOCTL_VARIANT(__VA_ARGS__, __IOCTL3, __IOCTL2)(__VA_ARGS__)
libc_hidden_proto (__ioctl)

# endif /* !_ISOMAC */
#endif
