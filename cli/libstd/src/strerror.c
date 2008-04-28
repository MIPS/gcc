/*

   Copyright (C) 2007-2008 Free Software Foundation, Inc.
   Contributed by STMicroelectronics

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 2, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to the Free
Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301, USA.  */

#define __LIBSTD_CRT_BUILD

#include <errno.h>
#include <string.h>

LIBSTD_LPROTO_IMPL(char *, strerror, int errnum)
{
    char* result;
    switch (errnum) {
    case ENOENT:  result = "No such file or directory"; break;
    case EIO:     result = "I/O error";                 break;
    case EBADF:   result = "Bad file number";           break;
    case EAGAIN:  result = "Try again";                 break;
    case ENOMEM:  result = "Out of memory";             break;
    case EACCES:  result = "Permission denied";         break;
    case EFAULT:  result = "Bad address";               break;
    case EEXIST:  result = "File exists";               break;
    case ENOTDIR: result = "Not a directory";           break;
    case EINVAL:  result = "Invalid argument";          break;
    case EMFILE:  result = "Too many open files";       break;
    case ESPIPE:  result = "Illegal seek";              break;
    case EDOM:    result = "Math arg out of domain of func"; break;
    case ERANGE:  result = "Math result not representable";  break;
    case ENAMETOOLONG: result = "File name too long";   break;
    case EILSEQ:  result = "Illegal byte sequence";     break;
    case ENOSYS:  result = "Feature not implemented";   break;
    default:
        result = "Unknown error";
        break;
    }
    return result;
}
