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

#ifndef __STDIO_H__
#define __STDIO_H__

#include <__cdefs.h>

/* Included for NULL, size_t */
#include <stddef.h>

/* Included for va_list */
#include <stdarg.h>

#include <bits/libstd_stdio.h>
#include <bits/libstd_types.h>
#include <bits/libstd_proto.h>

__BEGIN_EXTERN_C

#define EOF (-1)


#define FOPEN_MAX    __LIBSTD_FOPEN_MAX
#define FILENAME_MAX __LIBSTD_FILENAME_MAX

LIBSTD_LPROTO(FILE *, fopen, const char* restrict, const char *restrict);
LIBSTD_LPROTO(int, fclose, FILE *restrict);
LIBSTD_LPROTO(int, fflush, FILE *restrict);
LIBSTD_LPROTO(FILE *, freopen, const char *restrict, const char *restrict, FILE *restrict);


#define BUFSIZ __LIBSTD_BUFSIZ

#define _IOFBF  __LIBSTD_IOFBF
#define _IOLBF  __LIBSTD_IOLBF
#define _IONBF  __LIBSTD_IONBF

LIBSTD_LPROTO(int, setvbuf, FILE *restrict, char *restrict, int, size_t);
LIBSTD_LPROTO(void, setbuf, FILE *restrict, char *restrict);

#define stdin  (LIBSTD_LNAME(__io_ftable_get_entry)(__LIBSTD_STDIN_FILENO))
#define stdout (LIBSTD_LNAME(__io_ftable_get_entry)(__LIBSTD_STDOUT_FILENO))
#define stderr (LIBSTD_LNAME(__io_ftable_get_entry)(__LIBSTD_STDERR_FILENO))



#define SEEK_SET __LIBSTD_SEEK_SET
#define SEEK_CUR __LIBSTD_SEEK_CUR
#define SEEK_END __LIBSTD_SEEK_END

LIBSTD_LPROTO(int, fseek, FILE *restrict, long, int);
LIBSTD_LPROTO(long, ftell, FILE *restrict);
LIBSTD_LPROTO(void, rewind, FILE *restrict);


typedef int fpos_t;
LIBSTD_LPROTO(int, fgetpos, FILE *restrict, fpos_t *restrict);
LIBSTD_LPROTO(int, fsetpos, FILE *restrict, const fpos_t *);




LIBSTD_LPROTO(int, fgetc, FILE *);
LIBSTD_LPROTO(int, getc, FILE *);
LIBSTD_LPROTO(int, getchar, void);
LIBSTD_LPROTO(int, ungetc, int, FILE *);


LIBSTD_LPROTO(char *, fgets, char *restrict, int, FILE *restrict);
LIBSTD_LPROTO(char *, gets, char *);


LIBSTD_LPROTO(int, fscanf, FILE *restrict, const char *restrict, ...);
LIBSTD_LPROTO(int, scanf, const char *restrict, ...);
LIBSTD_LPROTO(int, sscanf, const char *restrict, const char *restrict, ...);


LIBSTD_LPROTO(int, fputc, int, FILE *);
LIBSTD_LPROTO(int, putc, int, FILE *);
LIBSTD_LPROTO(int, putchar, int);

LIBSTD_LPROTO(int, fputs, const char *restrict, FILE *restrict);
LIBSTD_LPROTO(int, puts, const char *);


LIBSTD_LPROTO(int, fprintf, FILE *restrict, const char *restrict, ...);
LIBSTD_LPROTO(int, printf, const char *restrict, ...);
LIBSTD_LPROTO(int, sprintf, char *restrict, const char *restrict, ...);
LIBSTD_LPROTO(int, snprintf, char *restrict, size_t, const char *restrict, ...);


LIBSTD_LPROTO(int, vfprintf, FILE *restrict, const char *restrict, va_list);
LIBSTD_LPROTO(int, vprintf, const char *restrict, va_list);
LIBSTD_LPROTO(int, vsprintf, char *restrict, const char *restrict, va_list);
LIBSTD_LPROTO(int, vsnprintf, char *restrict, size_t, const char *restrict, va_list);

LIBSTD_LPROTO(int, vfscanf, FILE *restrict, const char *restrict, va_list);
LIBSTD_LPROTO(int, vscanf, const char *restrict, va_list);
LIBSTD_LPROTO(int, vsscanf, const char *restrict, const char *restrict, va_list);


LIBSTD_LPROTO(size_t, fread, void *restrict, size_t, size_t, FILE *restrict);
LIBSTD_LPROTO(size_t, fwrite, const void * restrict, size_t , size_t, FILE *restrict);


LIBSTD_LPROTO(int, feof, FILE *);
LIBSTD_LPROTO(int, ferror, FILE *);
LIBSTD_LPROTO(void, clearerr, FILE *);


LIBSTD_LPROTO(int, rename, const char *, const char *);
LIBSTD_LPROTO(int, remove, const char *);



#define L_tmpnam __LIBSTD_L_tmpnam
#define TMP_MAX  __LIBSTD_TMP_MAX

LIBSTD_LPROTO(FILE *, tmpfile, void);
LIBSTD_LPROTO(char *, tmpnam, char *);

LIBSTD_LPROTO(void, perror, const char *);

__END_EXTERN_C

#endif /*__STDIO_H__*/
