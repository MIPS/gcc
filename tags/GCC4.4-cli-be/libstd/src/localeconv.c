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

#include <locale.h>
#include <limits.h>

/* We only support the "C" locale */
static char lconv_d_[] = ".";
static char lconv_n_[] = "";

static struct lconv lconv__c_ = {
    lconv_d_,    /* decimal_point       "."      */
    lconv_n_,    /* thousands_sep       ""       */
    lconv_n_,    /* grouping            ""       */
    lconv_n_,    /* int_curr_symbol     ""       */
    lconv_n_,    /* currency_symbol     ""       */
    lconv_n_,    /* mon_decimal_point   ""       */
    lconv_n_,    /* mon_thousands_sep   ""       */
    lconv_n_,    /* mon_grouping        ""       */
    lconv_n_,    /* positive_sign       ""       */
    lconv_n_,    /* negative_sign       ""       */
    CHAR_MAX,    /* int_frac_digits     CHAR_MAX */
    CHAR_MAX,    /* frac_digits         CHAR_MAX */
    CHAR_MAX,    /* p_cs_precedes       CHAR_MAX */
    CHAR_MAX,    /* p_sep_by_space      CHAR_MAX */
    CHAR_MAX,    /* n_cs_precedes       CHAR_MAX */
    CHAR_MAX,    /* n_sep_by_space      CHAR_MAX */
    CHAR_MAX,    /* p_sign_posn         CHAR_MAX */
    CHAR_MAX     /* n_sign_posn         CHAR_MAX */
};

LIBSTD_LPROTO_IMPL(struct lconv *,localeconv,void)
{
    return &lconv__c_;
}
