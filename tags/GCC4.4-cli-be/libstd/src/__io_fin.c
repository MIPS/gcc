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


#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>

#include "__ctype.h"
#include "__io.h"

enum {
    __IO_SCONV_SCANSET_SIZE = 64,
    __IO_MAX_SCAN_BUFF      = 1024
};

enum __sconv_mod {
    __MOD_undefined = 0,
    __MOD_hh,
    __MOD_h,
    __MOD_l,
    __MOD_ll,
    __MOD_j,
    __MOD_z,
    __MOD_t,
    __MOD_L
};

typedef struct __sconv __sconv;

struct __sconv {
    enum __sconv_mod mod_;
    int              width_;
    char             sset[__IO_SCONV_SCANSET_SIZE];
    char             conv_;
    unsigned         star:1;
    unsigned         circ:1;
};

/* Process format flags */
LIBSTD_SPROTO(__sconv, __io_sconv_flags, char **);

/*
 * These are the specific parsing functions for specific types
*/
LIBSTD_SPROTO(int, __io_fin_do_integer, __sconv *, __in_proxy *, char *, size_t *);
LIBSTD_SPROTO(int, __io_fin_do_string, __sconv *, __in_proxy *, char *, size_t *);
LIBSTD_SPROTO(int, __io_fin_do_set, __sconv *, __in_proxy *, char *, size_t *);
LIBSTD_SPROTO(int, __io_fin_do_floating, __sconv *, __in_proxy *, char *, size_t *);

LIBSTD_LPROTO_IMPL(int, __io_fin, const char *format, va_list va, __in_proxy *iproxy)
{
    char *lformat = (char *)format;
    size_t assign = 0;
    size_t lch_read = 0;
    int inchar;

    if (__isspace(*lformat)) {
        while (__isspace(*lformat))
            ++lformat;

        do {
            inchar = iproxy->ic_(iproxy);
            ++lch_read;
        } while (__isspace(inchar));

        if (inchar == EOF)
            return EOF;

        iproxy->uc_(iproxy, (char)inchar);
        --lch_read;
    }

    for (;;) {
        __sconv sconv_;
        char out_[__IO_MAX_SCAN_BUFF];

        switch (iproxy->ma_(iproxy, &lformat, &lch_read)) {
        case 0:
            return (int)assign;
            break;
        case EOF:
            return EOF;
            break;
        case 1:
            sconv_ = LIBSTD_SNAME(__io_sconv_flags)(&lformat);
            break;
        default:
            /* Should not be there */
/*             assert(0); */
            break;
        }

        inchar = iproxy->ic_(iproxy);
        ++lch_read;
        if (   sconv_.conv_ != '['
            && sconv_.conv_ != 'c'
            && sconv_.conv_ != 'n') {
            while (__isspace(inchar)) {
                inchar = iproxy->ic_(iproxy);
                ++lch_read;
            }
        }
        iproxy->uc_(iproxy, (char)inchar);
        --lch_read;

        if (inchar== EOF && lch_read==0)
            return EOF;

        switch (sconv_.conv_) {
        case 'd': case 'i': case 'o': case 'u': case 'x': case 'X':
        case 'p':
            if (LIBSTD_SNAME(__io_fin_do_integer)(&sconv_, iproxy, out_, &lch_read) == EOF)
                return EOF;
            if (*out_ && !sconv_.star) {
                int base_;

                switch (sconv_.conv_) {

                case 'p':
                    *(va_arg(va, void **)) = (void *)LIBSTD_LNAME(strtoul)(out_, NULL, 16);
                    break;

                case 'd': case 'i':
                    base_ = sconv_.conv_ == 'd' ? 10 : 0;
                    switch (sconv_.mod_) {
                    case __MOD_l:
                        *(va_arg(va, long *))    = (long)LIBSTD_LNAME(strtol)(out_, NULL, base_);
                        break;
                    case __MOD_ll:
                        *(va_arg(va, int64_t *)) = (int64_t)LIBSTD_LNAME(strtoll)(out_, NULL, base_);
                        break;
                    case __MOD_h:
                        *(va_arg(va, short *))   = (short)LIBSTD_LNAME(strtol)(out_, NULL, base_);
                        break;
                    default:
                        *(va_arg(va, int *))     = (int)LIBSTD_LNAME(strtol)(out_, NULL, base_);
                        break;
                    }
                    break;

                case 'o': case 'u': case 'x': case 'X':
                    base_ = sconv_.conv_ == 'o' ? 8 : sconv_.conv_ == 'x' || sconv_.conv_ == 'X' ? 16 : 10;
                    switch (sconv_.mod_) {
                    case __MOD_l:
                        *(va_arg(va, unsigned long *))  = (unsigned long)LIBSTD_LNAME(strtoul)(out_, NULL, base_);
                        break;
                    case __MOD_ll:
                        *(va_arg(va, uint64_t *))       = (uint64_t)LIBSTD_LNAME(strtoull)(out_, NULL, base_);
                        break;
                    case __MOD_h:
                        *(va_arg(va, unsigned short *)) = (unsigned short)LIBSTD_LNAME(strtoul)(out_, NULL, base_);
                        break;
                    default:
                        *(va_arg(va, unsigned int *))   = (unsigned int)LIBSTD_LNAME(strtoul)(out_, NULL, base_);
                        break;
                    }
                    break;
                }
                ++assign;
            }
            break;

/*         case 'a' : case 'A': */
/*             break; */

        case 'e': case 'E': case 'f': case 'F': case 'g': case 'G':
            if (LIBSTD_SNAME(__io_fin_do_floating)(&sconv_, iproxy, out_, &lch_read) == EOF)
                return EOF;
            if (*out_ && !sconv_.star) {
                switch (sconv_.mod_) {
                case __MOD_l:
                    *(va_arg(va, double*))      = LIBSTD_LNAME(strtod)(out_, NULL);
                    break;
                case __MOD_L:
                    *(va_arg(va, long double*)) = LIBSTD_LNAME(strtold)(out_, NULL);
                    break;
                default:
                    *(va_arg(va, float*))       = LIBSTD_LNAME(strtof)(out_, NULL);
                    break;
                }
                ++assign;
            }
            break;

        case 'c':
            if (LIBSTD_SNAME(__io_fin_do_string)(&sconv_, iproxy, out_, &lch_read) == EOF)
                return EOF;
            if (*out_ && !sconv_.star) {
                char *pv_ = va_arg(va, char*);
                /* There may be many chars to copy depending on witdh,
                 * but no terminating null
                 * Default lenght is one.
                 */
                if (pv_)
                    LIBSTD_LNAME(memcpy)(pv_, out_, sconv_.width_ < 0 ? 1 : sconv_.width_);
                ++assign;
            }
            break;
        case 's':
            if (LIBSTD_SNAME(__io_fin_do_string)(&sconv_, iproxy, out_, &lch_read) == EOF)
                return EOF;
            if (*out_ && !sconv_.star) {
                char *pv_ = va_arg(va, char*);
                if (pv_)
                    LIBSTD_LNAME(strcpy)(pv_, out_);
                ++assign;
            }
            break;

        case '[':
            if (LIBSTD_SNAME(__io_fin_do_set)(&sconv_, iproxy, out_, &lch_read) == EOF)
                return EOF;
            if (*out_ && !sconv_.star) {
                char *pv_ = va_arg(va, char*);
                if (pv_)
                    LIBSTD_LNAME(strcpy)(pv_, out_);
                ++assign;
            }
            break;

        case '%':
            inchar = iproxy->ic_(iproxy);
            ++lch_read;
            if (inchar!= '%')
                return EOF;
            break;

        case 'n':
            if (sconv_.star) {
                switch (sconv_.mod_) {
                case __MOD_l:
                    *(va_arg(va, long *))    = (long)lch_read;
                    break;
                case __MOD_ll:
                    *(va_arg(va, int64_t *)) = (int64_t)lch_read;
                    break;
                case __MOD_h:
                    *(va_arg(va, short *))   = (short)lch_read;
                    break;
                default:
                    *(va_arg(va, int *))     = (int)lch_read;
                    break;
                }
            }
            break;

        default:
            /* Unkown conversion format, no way to cry */
            break;
        }
    }

    return EOF;
}


LIBSTD_SPROTO_IMPL(int, __io_fin_do_integer, __sconv *conv, __in_proxy *iproxy, char *out, size_t *read)
{
    size_t lch_read = *read;
    int width_ = conv->width_;
    int accept_hex = (conv->conv_=='i' || conv->conv_=='x' || conv->conv_=='X' || conv->conv_=='p');
    signed char inchar;

    inchar = iproxy->ic_(iproxy);
    ++(*read);

    if ((inchar == '+') || (inchar == '-') && (width_!=0)) {
        *out++ = inchar;
        inchar = iproxy->ic_(iproxy);
        ++(*read);
        --width_;
    }

    if (   width_ != 0
        && inchar== '0'
        && (conv->conv_=='o' || accept_hex != 0)) {
        *out++ = inchar;
        inchar = iproxy->ic_(iproxy);
        ++(*read);
        --width_;
        if (width_ != 0 && accept_hex != 0 && (inchar== 'x' || inchar== 'X' )) {
            *out++ = inchar;
            inchar = iproxy->ic_(iproxy);
            ++(*read);
            --width_;
        }
    }

    while (width_ != 0 && (__isdigit(inchar) || (accept_hex && __isxdigit(inchar)))) {
        *out++ = inchar;
        inchar = iproxy->ic_(iproxy);
        ++(*read);
        --width_;
    }

    iproxy->uc_(iproxy, (char)inchar);
    --(*read);
    *out = '\0';
    return (*read != lch_read) ? 1 : EOF;
}

LIBSTD_SPROTO_IMPL(int, __io_fin_do_string, __sconv *conv, __in_proxy *iproxy, char *out, size_t *read)
{
    size_t lch_read = *read;
    int width_ = conv->width_;
    signed char inchar;

    if ((conv->conv_ == 'c') && (width_ < 0) )
        width_ = 1;

    inchar = iproxy->ic_(iproxy);
    ++(*read);
    while (inchar && (inchar != EOF) && (!__isspace(inchar) || (conv->conv_ == 'c')) && width_) {
        *out++ = inchar;
        inchar = iproxy->ic_(iproxy);
        ++(*read);
        --width_;
    }

    if (conv->conv_ == 's')
        *out = '\0';
    iproxy->uc_(iproxy, (char)inchar);
    --(*read);
    return (*read != lch_read) ? 1 : EOF;
}

LIBSTD_SPROTO_IMPL(int, __io_fin_do_set, __sconv *conv, __in_proxy *iproxy, char *out, size_t *read)
{
    size_t lch_read = *read;
    int width_ = conv->width_;
    int found_ = 1;
    signed char inchar;

    inchar = iproxy->ic_(iproxy);
    ++(*read);

    while (width_ != 0 && found_ != 0 && inchar != EOF) {
        found_ = (LIBSTD_LNAME(strrchr)(conv->sset, inchar) != NULL);
        if (conv->circ)
            found_ = !found_;
        if (found_) {
            *out++ = inchar;
            inchar = iproxy->ic_(iproxy);
            ++(*read);
            --width_;
        }
    }
    iproxy->uc_(iproxy, (char)inchar);
    --(*read);
    *out = '\0';
    return (*read != lch_read) ? 1 : EOF;
}

LIBSTD_SPROTO_IMPL(int, __io_fin_do_floating, __sconv *conv, __in_proxy *iproxy, char *out, size_t *read)
{
    size_t lch_read = *read;
    int width_ = conv->width_;
    signed char inchar;

    inchar = iproxy->ic_(iproxy);
    ++(*read);

    if ((inchar == '+' || inchar == '-') && width_ != 0) {
        *out++ = inchar;
        inchar = iproxy->ic_(iproxy);
        ++(*read);
        --width_;
    }

    while (__isdigit(inchar) && width_ != 0) {
        *out++ = inchar;
        inchar = iproxy->ic_(iproxy);
        ++(*read);
        --width_;
    }


    if (inchar == '.' && width_ != 0) {
        *out++ = inchar;
        inchar = iproxy->ic_(iproxy);
        ++(*read);
        --width_;
        while (__isdigit(inchar) && width_ != 0) {
            *out++ = inchar;
            inchar = iproxy->ic_(iproxy);
            ++(*read);
            --width_;
        }
    }

    if ((inchar == 'e' || inchar == 'E') && width_ != 0) {
        *out++ = inchar;
        inchar = iproxy->ic_(iproxy);
        ++(*read);
        --width_;
        if ((inchar == '+' || inchar == '-') && width_ != 0) {
            *out++ = inchar;
            inchar = iproxy->ic_(iproxy);
            ++(*read);
            --width_;
        }
        while (__isdigit(inchar) && width_ != 0) {
            *out++ = inchar;
            inchar = iproxy->ic_(iproxy);
            ++(*read);
            --width_;
        }
    }
    iproxy->uc_(iproxy, (char)inchar);
    --(*read);
    *out = '\0';
    return (*read != lch_read) ? 1 : EOF;
}

LIBSTD_SPROTO_IMPL(__sconv , __io_sconv_flags, char **pf)
{
    __sconv conv = { __MOD_undefined };

    /* Skip the % */
    ++(*pf);

    /* Note star */
    if (**pf == '*') {
        conv.star = 1;
        ++(*pf);
    }

    /* Field width */
    {
        enum { __IO_FW_BUFFER = 16 };
        char b[__IO_FW_BUFFER];
        char *pb = b;
        int m = 0;
        while ( __isdigit(**pf) && (++m < __IO_FW_BUFFER))
            *pb++ = *((*pf)++);
        *pb = '\0';
        if (m)
            conv.width_ = atoi(b);
        else
            conv.width_ = -1;
    }

    /* Process length modifiers */
    switch (**pf) {
    case 'l': ++(*pf); conv.mod_ = __MOD_l; if (**pf=='l') { ++(*pf); conv.mod_ = __MOD_ll; } break;
    case 'L': ++(*pf); conv.mod_ = __MOD_L; break;
    case 'h': ++(*pf); conv.mod_ = __MOD_h; if (**pf=='h') { ++(*pf); conv.mod_ = __MOD_hh; } break;
    case 'j': ++(*pf); conv.mod_ = __MOD_j; break;
    case 'z': ++(*pf); conv.mod_ = __MOD_z; break;
    case 't': ++(*pf); conv.mod_ = __MOD_t; break;
    default : break;
    }

    /* Conversion specifier */
    conv.conv_ = **pf;
    ++(*pf);

    /* Scanset conversion */
    if (conv.conv_ == '[') {
        char *psset = conv.sset;
        int m = 0;
        /* Maybe tarting cirumflex */
        if (**pf == '^') {
            conv.circ = 1;
            ++(*pf);
        }
        /* Look for [']' wile copying */
        if (**pf == ']') {
            *psset++ = *((*pf)++);
            ++m;
        }
        while (**pf != ']' && **pf && (++m < __IO_SCONV_SCANSET_SIZE))
            *psset++ = *((*pf)++);
        *psset = '\0';
        if (**pf == ']')
            ++(*pf);
    }
    return conv;
}
