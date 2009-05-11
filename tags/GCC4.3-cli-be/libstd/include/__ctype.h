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

#ifndef ____CTYPE_H__
#define ____CTYPE_H__

#include <__cdefs.h>

__BEGIN_EXTERN_C

extern const unsigned short __Libstd__ctype__[];

enum {
    __ISNONE     = 0x000,  /* Not classified */
    __ISDIGIT    = 0x001,  /* Oneof 10 decimal digits */
    __ISHEX      = 0x002,  /* Oneof  6 hexadecimal digits not in __ISDIGIT */
    __ISUPPER    = 0x004,  /* Oneof 26 uppercase letter of the Latin alphabet */
    __ISLOWER    = 0x008,  /* Oneof 26 lowercase letter of the Latin alphabet */
    __ISSPACE    = 0x010,  /* TAB, CR, NL, VT, FF*/
    __ISSTDBLANK = 0x020,  /* ' ' and `\t` */
    __ISPUNCT    = 0x040,  /* Printing characters for which neither isspace nor isalnum is true */
    __ISCONTROL  = 0x080,  /* ASCII control characters*/
    __ISBLANK    = 0x100,  /* ' ' since isprint include this one and not '\t' */
    __ISALPHA    = (__ISUPPER|__ISLOWER),
    __ISALNUM    = (__ISUPPER|__ISLOWER|__ISDIGIT),
    __ISPRINT    = (__ISBLANK|__ISPUNCT|__ISUPPER|__ISLOWER|__ISDIGIT),
    __ISGRAPH    = (__ISUPPER|__ISLOWER|__ISDIGIT|__ISPUNCT)
};

#define __isctype(_c,_ctype) (__Libstd__ctype__[(unsigned char)(_c)] & (_ctype))

#define __isalnum(c)    __isctype(c,__ISALNUM)
#define __isalpha(c)    __isctype(c,__ISALPHA)
#define __isblank(c)    __isctype(c,__ISSTDBLANK)
#define __iscntrl(c)    __isctype(c,__ISCONTROL)
#define __isdigit(c)    __isctype(c,__ISDIGIT)
#define __isgraph(c)    __isctype(c,__ISGRAPH)
#define __islower(c)    __isctype(c,__ISLOWER)
#define __isprint(c)    __isctype(c,__ISPRINT)
#define __ispunct(c)    __isctype(c,__ISPUNCT)
#define __isspace(c)    __isctype(c,__ISSPACE)
#define __isupper(c)    __isctype(c,__ISUPPER)
#define __isxdigit(c)   __isctype(c,__ISHEX)

#define __toupper(c)    (__islower(c) ? (c) + ('A' - 'a') : (c) )
#define __tolower(c)    (__isupper(c) ? (c) + ('a' - 'A') : (c) )

__END_EXTERN_C

#endif /*____CTYPE_H__*/
