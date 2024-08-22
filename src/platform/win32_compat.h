#ifndef WIN32_COMPAT_H
#define WIN32_COMPAT_H

/* Compatibility definitions for Windows NT targets. */

#include <sys/fcntl.h>
#include <winsock2.h>
#include <wspiapi.h>
#define close closesocket
#define ioctl ioctlsocket

#endif
