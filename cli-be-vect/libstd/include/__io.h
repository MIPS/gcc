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

#ifndef ____IO_H__
#define ____IO_H__

#if !defined(__LIBSTD_CRT_BUILD)
#error "__io_device.h : Sorry : This header is intended for building C runtime only."
#endif

#include <stdarg.h>

#include <bits/libstd_types.h>

__BEGIN_EXTERN_C

enum {
    __iob_mask_buff  = 00003, /* Buffering mode : cf setvbuf                    */
    __iob_mask_unget = 00004, /* unget status                                   */
    __iob_mask_error = 00010, /* Stream error indicator                         */
    __iob_mask_alloc = 00020, /* Owner must free                                */
    __iob_mask_eof   = 00040, /* EOF reached                                    */
    __iob_mask_r     = 00100, /* Readable                                       */
    __iob_mask_w     = 00200, /* Writable                                       */
    __iob_mask_rw    = 00400, /* Readable and writable                          */
    __iob_mask_bin   = 01000, /* Binary mode, Makes no difference on unix hosts */
    __iob_mask_tmp   = 02000, /* Was created by tmpfile. Must remove on close   */
    __iob_mask_tty   = 04000  /* It is a tty                                    */
};

struct __iob {
    signed short   fd;        /* File descriptor                                   */
    unsigned short flags;     /* FILE flags (look at enum definition)              */
    unsigned char* base;      /* Pointer to the start of the buffer (NULL if none) */
    unsigned char* end;       /* Pointer to the end of the buffer   (NULL if none) */
    unsigned char* pos;       /* next character from/to in buffer                  */
    unsigned char* stop;      /* Last read char in buffer                          */
    char*          tmpnam;    /* name of temporary file if __iob_mask_tmp set      */
    unsigned char  unget_val; /* unget character if __iob_mask_unget set           */
};

#define _FILE_GET_unget(filep_)     ((filep_)->flags & __iob_mask_unget )
#define _FILE_SET_unget(filep_)     ((filep_)->flags |= __iob_mask_unget )
#define _FILE_UNSET_unget(filep_)   ((filep_)->flags &= ~__iob_mask_unget )

#define _FILE_GET_error(filep_)     ((filep_)->flags & __iob_mask_error )
#define _FILE_SET_error(filep_)     ((filep_)->flags |= __iob_mask_error )
#define _FILE_UNSET_error(filep_)   ((filep_)->flags &= ~__iob_mask_error )

#define _FILE_GET_buff(filep_)      ((filep_)->flags & __iob_mask_buff )
#define _FILE_SET_buff(filep_,val_) ((filep_)->flags = ((filep_)->flags & ~__iob_mask_buff) | (val_))

#define _FILE_GET_alloc(filep_)     ((filep_)->flags & __iob_mask_alloc )
#define _FILE_SET_alloc(filep_)     ((filep_)->flags |= __iob_mask_alloc )
#define _FILE_UNSET_alloc(filep_)   ((filep_)->flags &= ~__iob_mask_alloc )

#define _FILE_GET_eof(filep_)     ((filep_)->flags & __iob_mask_eof )
#define _FILE_SET_eof(filep_)     ((filep_)->flags |= __iob_mask_eof )
#define _FILE_UNSET_eof(filep_)   ((filep_)->flags &= ~__iob_mask_eof )

#define _FILE_GET_r(filep_)     ((filep_)->flags & __iob_mask_r )
#define _FILE_SET_r(filep_)     ((filep_)->flags |= __iob_mask_r )
#define _FILE_UNSET_r(filep_)   ((filep_)->flags &= ~__iob_mask_r )

#define _FILE_GET_w(filep_)     ((filep_)->flags & __iob_mask_w )
#define _FILE_SET_w(filep_)     ((filep_)->flags |= __iob_mask_w )
#define _FILE_UNSET_w(filep_)   ((filep_)->flags &= ~__iob_mask_w )

#define _FILE_GET_rw(filep_)     ((filep_)->flags & __iob_mask_rw )
#define _FILE_SET_rw(filep_)     ((filep_)->flags |= __iob_mask_rw )

#define _FILE_GET_bin(filep_)     ((filep_)->flags & __iob_mask_bin )
#define _FILE_SET_bin(filep_)     ((filep_)->flags |= __iob_mask_bin )

#define _FILE_GET_tmp(filep_)     ((filep_)->flags & __iob_mask_tmp )
#define _FILE_SET_tmp(filep_)     ((filep_)->flags |= __iob_mask_tmp )

#define _FILE_GET_tty(filep_)     ((filep_)->flags & __iob_mask_tty )


/*
 * These functions base their calls on the fileno.
 * Higher level functions should call only these.
 */
LIBSTD_LPROTO(int, __device_open, const char *, int);
LIBSTD_LPROTO(int, __device_close, int);
LIBSTD_LPROTO(int, __device_read, int, void *, int);
LIBSTD_LPROTO(int, __device_write, int, const void *, int);
LIBSTD_LPROTO(long, __device_lseek, int, long, int);


LIBSTD_LPROTO(FILE *, __io_ftable_new_file, void);
LIBSTD_LPROTO(void, __io_ftable_release_file, FILE *);
LIBSTD_LPROTO(FILE *, __io_ftable_get_entry, int);
LIBSTD_LPROTO(void, __io_ftable_close_all, void);
LIBSTD_LPROTO(int, __io_ftable_flush_all, void);


LIBSTD_LPROTO(FILE *, __io_fopen, FILE *, const char *, const char *);
LIBSTD_LPROTO(int, __io_fread, FILE *);
LIBSTD_LPROTO(int, __io_flush, FILE *);

LIBSTD_LPROTO(int, __io_perm_readable, FILE *);
LIBSTD_LPROTO(int, __io_perm_writable, FILE *);

/* Input handlers */

typedef struct __in_proxy __in_proxy;
typedef char (*__io_in_char)(__in_proxy *);
typedef void (*__io_unin_char)(__in_proxy *, char);
typedef int  (*__io_matcher)(__in_proxy *, char **, size_t *);
struct __in_proxy {
    union __in_proxy_where {
        FILE *f;              /* Input from this file descriptor */
        const char **s;       /* Input from this string */
    } where_;
    __io_in_char   ic_;
    __io_unin_char uc_;
    __io_matcher   ma_;
};

LIBSTD_LPROTO(int, __io_fin, const char *, va_list, __in_proxy *);


/* Output handlers */

typedef struct __out_proxy __out_proxy;
typedef int (*__io_out_char)(__out_proxy *, char);
typedef int (*__io_out_string)(__out_proxy *, char *);

struct __out_proxy {
    union __out_proxy_where {
        FILE *f;              /* Output goes to this file descriptor */
        char *s;              /* Output goes to this string */
    } where_;
    size_t maxlen;
    __io_out_char   oc_;
    __io_out_string os_;
};

LIBSTD_LPROTO(int, __io_fout, char **, va_list, __out_proxy *);

/* Helpful macros */

#ifndef MIN
#  define MIN(x,y) ((x) < (y) ? (x): (y))
#endif

#ifndef MAX
#  define MAX(x,y) ((x) > (y) ? (x): (y))
#endif

#ifndef ABS
#  define ABS(x) ((x) > 0 ? (x): -(x))
#endif

__END_EXTERN_C

#endif /*____IO_H__*/
