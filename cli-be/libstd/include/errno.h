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

#ifndef __ERRNO_H__
#define __ERRNO_H__

#include <__cdefs.h>

#include <bits/libstd_errno.h>
#include <bits/libstd_proto.h>

__BEGIN_EXTERN_C

#define ENOENT       __LIBSTD_ENOENT
#define EIO          __LIBSTD_EIO
#define EBADF        __LIBSTD_EBADF
#define EAGAIN       __LIBSTD_EAGAIN
#define ENOMEM       __LIBSTD_ENOMEM
#define EACCES       __LIBSTD_EACCES
#define EFAULT       __LIBSTD_EFAULT
#define EEXIST       __LIBSTD_EEXIST
#define ENOTDIR      __LIBSTD_ENOTDIR
#define EINVAL       __LIBSTD_EINVAL
#define EMFILE       __LIBSTD_EMFILE
#define ESPIPE       __LIBSTD_ESPIPE
#define EDOM         __LIBSTD_EDOM
#define ERANGE       __LIBSTD_ERANGE
#define ENAMETOOLONG __LIBSTD_ENAMETOOLONG
#define EILSEQ       __LIBSTD_EILSEQ
#define ENOTSUP      __LIBSTD_ENOTSUP
#define ENOSYS       __LIBSTD_ENOSYS

#define errno (*LIBSTD_LNAME(__errno__get_ptr)())

__END_EXTERN_C

#endif /*__ERRNO_H__*/
