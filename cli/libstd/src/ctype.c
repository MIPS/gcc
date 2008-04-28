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

/* If we are compiling this, we are sure this is to build C-runtime*/
#define __LIBSTD_CRT_BUILD


#include <ctype.h>

#include "__ctype.h"

/* ASCII table from /usr/table/ascii */
/* when we compile native (for static compilation) this symbol needs to be
 * weak. Since it is global and initialized, the .NET assembly will contain
 * an init function for it. Therefore the static compiler will generate a
 * symbol in the symbol table. This will clash with the one in the library
 * if we don't define it as weak.
 */
#if !defined(__GNU_CIL__)
#pragma weak __Libstd__ctype__
#endif
const unsigned short __Libstd__ctype__ [256] = {

/*| 00 nul */   __ISCONTROL,
/*| 01 soh */   __ISCONTROL,
/*| 02 stx */   __ISCONTROL,
/*| 03 etx */   __ISCONTROL,
/*| 04 eot */   __ISCONTROL,
/*| 05 enq */   __ISCONTROL,
/*| 06 ack */   __ISCONTROL,
/*| 07 bel */   __ISCONTROL,
/*| */
/*| 08 bs  */   __ISCONTROL,
/*| 09 ht  */   __ISCONTROL | __ISSPACE | __ISSTDBLANK ,
/*| 0a nl  */   __ISCONTROL | __ISSPACE,
/*| 0b vt  */   __ISCONTROL | __ISSPACE,
/*| 0c np  */   __ISCONTROL | __ISSPACE,
/*| 0d cr  */   __ISCONTROL | __ISSPACE,
/*| 0e so  */   __ISCONTROL,
/*| 0f si  */   __ISCONTROL,
/*| */
/*| 10 dle */   __ISCONTROL,
/*| 11 dc1 */   __ISCONTROL,
/*| 12 dc2 */   __ISCONTROL,
/*| 13 dc3 */   __ISCONTROL,
/*| 14 dc4 */   __ISCONTROL,
/*| 15 nak */   __ISCONTROL,
/*| 16 syn */   __ISCONTROL,
/*| 17 etb */   __ISCONTROL,
/*| */
/*| 18 can */   __ISCONTROL,
/*| 19 em  */   __ISCONTROL,
/*| 1a sub */   __ISCONTROL,
/*| 1b esc */   __ISCONTROL,
/*| 1c fs  */   __ISCONTROL,
/*| 1d gs  */   __ISCONTROL,
/*| 1e rs  */   __ISCONTROL,
/*| 1f us  */   __ISCONTROL,
/*| */
/*| 20 sp  */   __ISSPACE | __ISSTDBLANK | __ISBLANK,
/*| 21  !  */   __ISPUNCT,
/*| 22  "  */   __ISPUNCT,
/*| 23  #  */   __ISPUNCT,
/*| 24  $  */   __ISPUNCT,
/*| 25  %  */   __ISPUNCT,
/*| 26  &  */   __ISPUNCT,
/*| 27  '  */   __ISPUNCT,
/*| */
/*| 28  (  */   __ISPUNCT,
/*| 29  )  */   __ISPUNCT,
/*| 2a  *  */   __ISPUNCT,
/*| 2b  +  */   __ISPUNCT,
/*| 2c  ,  */   __ISPUNCT,
/*| 2d  -  */   __ISPUNCT,
/*| 2e  .  */   __ISPUNCT,
/*| 2f  /  */   __ISPUNCT,
/*| */
/*| 30  0  */   __ISDIGIT | __ISHEX,
/*| 31  1  */   __ISDIGIT | __ISHEX,
/*| 32  2  */   __ISDIGIT | __ISHEX,
/*| 33  3  */   __ISDIGIT | __ISHEX,
/*| 34  4  */   __ISDIGIT | __ISHEX,
/*| 35  5  */   __ISDIGIT | __ISHEX,
/*| 36  6  */   __ISDIGIT | __ISHEX,
/*| 37  7  */   __ISDIGIT | __ISHEX,
/*| */
/*| 38  8  */   __ISDIGIT | __ISHEX,
/*| 39  9  */   __ISDIGIT | __ISHEX,
/*| 3a  :  */   __ISPUNCT,
/*| 3b  ;  */   __ISPUNCT,
/*| 3c  <  */   __ISPUNCT,
/*| 3d  =  */   __ISPUNCT,
/*| 3e  >  */   __ISPUNCT,
/*| 3f  ?  */   __ISPUNCT,
/*| */
/*| 40  @  */   __ISPUNCT,
/*| 41  A  */   __ISUPPER | __ISHEX,
/*| 42  B  */   __ISUPPER | __ISHEX,
/*| 43  C  */   __ISUPPER | __ISHEX,
/*| 44  D  */   __ISUPPER | __ISHEX,
/*| 45  E  */   __ISUPPER | __ISHEX,
/*| 46  F  */   __ISUPPER | __ISHEX,
/*| 47  G  */   __ISUPPER,
/*| */
/*| 48  H  */   __ISUPPER,
/*| 49  I  */   __ISUPPER,
/*| 4a  J  */   __ISUPPER,
/*| 4b  K  */   __ISUPPER,
/*| 4c  L  */   __ISUPPER,
/*| 4d  M  */   __ISUPPER,
/*| 4e  N  */   __ISUPPER,
/*| 4f  O  */   __ISUPPER,
/*| */
/*| 50  P  */   __ISUPPER,
/*| 51  Q  */   __ISUPPER,
/*| 52  R  */   __ISUPPER,
/*| 53  S  */   __ISUPPER,
/*| 54  T  */   __ISUPPER,
/*| 55  U  */   __ISUPPER,
/*| 56  V  */   __ISUPPER,
/*| 57  W  */   __ISUPPER,
/*| */
/*| 58  X  */   __ISUPPER,
/*| 59  Y  */   __ISUPPER,
/*| 5a  Z  */   __ISUPPER,
/*| 5b  [  */   __ISPUNCT,
/*| 5c  \  */   __ISPUNCT,
/*| 5d  ]  */   __ISPUNCT,
/*| 5e  ^  */   __ISPUNCT,
/*| 5f  _  */   __ISPUNCT,
/*| */
/*| 60  `  */   __ISPUNCT,
/*| 61  a  */   __ISLOWER | __ISHEX,
/*| 62  b  */   __ISLOWER | __ISHEX,
/*| 63  c  */   __ISLOWER | __ISHEX,
/*| 64  d  */   __ISLOWER | __ISHEX,
/*| 65  e  */   __ISLOWER | __ISHEX,
/*| 66  f  */   __ISLOWER | __ISHEX,
/*| 67  g  */   __ISLOWER,
/*| */
/*| 68  h  */   __ISLOWER,
/*| 69  i  */   __ISLOWER,
/*| 6a  j  */   __ISLOWER,
/*| 6b  k  */   __ISLOWER,
/*| 6c  l  */   __ISLOWER,
/*| 6d  m  */   __ISLOWER,
/*| 6e  n  */   __ISLOWER,
/*| 6f  o  */   __ISLOWER,
/*| */
/*| 70  p  */   __ISLOWER,
/*| 71  q  */   __ISLOWER,
/*| 72  r  */   __ISLOWER,
/*| 73  s  */   __ISLOWER,
/*| 74  t  */   __ISLOWER,
/*| 75  u  */   __ISLOWER,
/*| 76  v  */   __ISLOWER,
/*| 77  w  */   __ISLOWER,
/*| */
/*| 78  x  */   __ISLOWER,
/*| 79  y  */   __ISLOWER,
/*| 7a  z  */   __ISLOWER,
/*| 7b  {  */   __ISPUNCT,
/*| 7c  |  */   __ISPUNCT,
/*| 7d  }  */   __ISPUNCT,
/*| 7e  ~  */   __ISPUNCT,
/*| 7f del */   __ISCONTROL
/*| */
/* Remaining is initialized to zero */
};

LIBSTD_LPROTO_IMPL(int, isalnum, int c)
{
    return __isalnum(c);
}

LIBSTD_LPROTO_IMPL(int, isalpha, int c)
{
    return __isalpha(c);
}

LIBSTD_LPROTO_IMPL(int, isblank, int c)
{
    return __isblank(c);
}

LIBSTD_LPROTO_IMPL(int, iscntrl, int c)
{
    return __iscntrl(c);
}

LIBSTD_LPROTO_IMPL(int, isdigit, int c)
{
    return __isdigit(c);
}

LIBSTD_LPROTO_IMPL(int, isgraph, int c)
{
    return __isgraph(c);
}

LIBSTD_LPROTO_IMPL(int, islower, int c)
{
    return __islower(c);
}

LIBSTD_LPROTO_IMPL(int, isprint, int c)
{
    return __isprint(c);
}

LIBSTD_LPROTO_IMPL(int, ispunct, int c)
{
    return __ispunct(c);
}

LIBSTD_LPROTO_IMPL(int, isspace, int c)
{
    return __isspace(c);
}

LIBSTD_LPROTO_IMPL(int, isupper, int c)
{
    return __isupper(c);
}

LIBSTD_LPROTO_IMPL(int, isxdigit, int c)
{
    return __isxdigit(c);
}

LIBSTD_LPROTO_IMPL(int, tolower, int c)
{
    return  __tolower(c);
}

LIBSTD_LPROTO_IMPL(int, toupper, int c)
{
    return  __toupper(c);
}

