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

#ifndef __SYS_STAT_H__
#define __SYS_STAT_H__

#include <__cdefs.h>

/* Included for off_t */
#include <sys/types.h>
#include <sys/time.h>

__BEGIN_EXTERN_C

struct stat {
    dev_t st_dev;			/* Device.  */
    ino_t st_ino;			/* 32bit file serial number.	*/
    
    mode_t st_mode;			/* File mode.  */
    nlink_t st_nlink;			/* Link count.  */
    uid_t st_uid;			/* User ID of the file's owner.	*/
    gid_t st_gid;			/* Group ID of the file's group.*/
    dev_t st_rdev;			/* Device number, if device.  */
    
    off_t st_size;			/* Size of file, in bytes.  */
    blksize_t st_blksize;		/* Optimal block size for I/O.  */
    blkcnt_t st_blocks; 		/* Number 512-byte blocks allocated. */

    time_t st_atime;		        /* Time of last access.  */
    time_t st_mtime;		        /* Time of last modification.  */
    time_t st_ctime;		        /* Time of last status change.  */
};

/* File types.  */
#define S_IFMT	  0170000	/* These bits determine file type.  */
#define	S_IFDIR   0040000	/* Directory.  */
#define	S_IFCHR   0020000	/* Character device.  */
#define	S_IFBLK   0060000	/* Block device.  */
#define	S_IFREG   0100000	/* Regular file.  */
#define	S_IFIFO   0010000	/* FIFO.  */
#define	S_IFLNK	  0120000	/* Symbolic link.  */
#define	S_IFSOCK  0140000	/* Socket.  */


LIBSTD_LPROTO(int, stat, const char *, struct stat *);
LIBSTD_LPROTO(int, fstat, int, struct stat *);

__END_EXTERN_C

#endif /*__SYS_STAT_H__*/
