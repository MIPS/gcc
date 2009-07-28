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

#ifndef __LIBSTD_ERRNO_H__
#define __LIBSTD_ERRNO_H__

#if !defined __ERRNO_H__ && !defined __LIBSTD_CRT_BUILD
#  error "Never use <bits/libstd_errno.h> directly; include <errno.h> instead."
#endif

/* Remember to update values in src/MSCorlibWrapper.cs */

__BEGIN_EXTERN_C

enum {
    __LIBSTD_ENOENT       = 2,   /* No such file or directory */
    __LIBSTD_EIO          = 5,   /* I/O error */
    __LIBSTD_EBADF        = 9,   /* Bad file number */
    __LIBSTD_EAGAIN       = 11,  /* Try again */
    __LIBSTD_ENOMEM       = 12,  /* Out of memory */
    __LIBSTD_EACCES       = 13,  /* Permission denied */
    __LIBSTD_EFAULT       = 14,  /* Bad address */
    __LIBSTD_EEXIST       = 17,  /* File exists */
    __LIBSTD_ENOTDIR      = 20,  /* Not a directory */
    __LIBSTD_EINVAL       = 22,  /* Invalid argument */
    __LIBSTD_EMFILE       = 24,  /* Too many open files */
    __LIBSTD_ESPIPE       = 29,  /* Illegal seek */
    __LIBSTD_EDOM         = 33,  /* Math arg out of domain of func */
    __LIBSTD_ERANGE       = 34,  /* Math result not representable */
    __LIBSTD_ENAMETOOLONG = 36,  /* File name too long */
    __LIBSTD_EILSEQ       = 88,  /* Illegal byte sequence */
    __LIBSTD_ENOTSUP      = 134, /* Not Supported */
    __LIBSTD_ENOSYS       = 333  /* Feature not implemented */
};

__END_EXTERN_C

#endif /*__LIBSTD_ERRNO_H__*/
