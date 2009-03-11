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

#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

#include "__ctype.h"
#include "__io.h"


enum __pconv_mod {
    __MOD_hh = 1,
    __MOD_h,
    __MOD_l,
    __MOD_ll,
    __MOD_j,
    __MOD_z,
    __MOD_t,
    __MOD_L
};

typedef struct __pconv {
    int      width_;
    int      precision_;
    enum __pconv_mod mod_;
    char     conv_;
    /* Flags */
    unsigned plus_:1;
    unsigned ljustify:1;
    unsigned space_:1;
    unsigned sharp_:1;
    unsigned zeropad:1;
} __pconv;

LIBSTD_SPROTO(__pconv, __io_pconv_flags, char **, va_list);

LIBSTD_SPROTO(int, __io_pconv_do_numeric, const __pconv *, va_list *, char *);

LIBSTD_SPROTO(char *, __io_pconv_format_integral, const __pconv *, va_list *, char *);

LIBSTD_SPROTO(char *, __io_pconv_format_floating, const __pconv *, va_list *, char *);

LIBSTD_SPROTO(char *, __io_pconv_format_f, long double, const __pconv *, char *);
LIBSTD_SPROTO(char *, __io_pconv_format_e, long double, const __pconv *, char *);
LIBSTD_SPROTO(char *, __io_pconv_format_g, long double, const __pconv *, char *);

LIBSTD_SPROTO(char *, __io_ldtoefstr, long double, int, char *, int *);
LIBSTD_SPROTO(char *, __io_ldtogstr, long double, int, char *, int *);
LIBSTD_SPROTO(int, __io_itoa, int, char *);

LIBSTD_SPROTO(char *, __io_copyfrac, const char *, int, int, int, char *);
LIBSTD_SPROTO(char *, __io_copymant, const char *, int, int, char *);
LIBSTD_SPROTO(char *, __io_copyexp, int, char, char *);


enum {
    __IO_MAX_BUFF            = 512,
    __IO_MAX_FIELD_WIDTH     = 16,
    __IO_MAX_FIELD_PRECISION = 16,
    __IO_MAX_FLOATING_BUFF   = 128
};

LIBSTD_SPROTO_IMPL(__pconv, __io_pconv_flags, char **format, va_list va)
{
    __pconv conv = { 0, -1 };

    /* Skip % */
    ++(*format);

    /* Process flags */
    {
        int done = 0;
        while (done == 0) {
            switch (**format) {
            case '+': ++(*format); conv.plus_  = 1; break;
            case '-': ++(*format); conv.ljustify = 1; break;
            case '#': ++(*format); conv.sharp_ = 1; break;
            case '0': ++(*format); conv.zeropad  = 1; break;
            case ' ': ++(*format); conv.space_ = 1; break;
            default : done = 1; break;
            }
        }
    }

    /* Process field width */
    if (**format == '*') {
        conv.width_ = (int)va_arg(va, int);
        if (conv.width_ < 0) {
            conv.width_ = -conv.width_;
            conv.ljustify = 1;
        }
        ++(*format);
    } else {
        char s_[__IO_MAX_FIELD_WIDTH];
        char *ps_ = s_;
        char *eos_ = s_ + __IO_MAX_FIELD_WIDTH;
        while (__isdigit(**format) && (ps_<eos_))
            *ps_++ = *((*format)++);
        if (ps_ < eos_) {
            *ps_= '\0';
            conv.width_ = LIBSTD_LNAME(atoi)(s_);
        }
    }

    /* Process field precision */
    if (**format == '.') {
        ++(*format);
        if (**format=='*') {
            conv.precision_ = (int)va_arg(va, int);
            ++(*format);
        } else {
            char s_[__IO_MAX_FIELD_PRECISION];
            char *ps_ = s_;
            char *eos_ = s_ + __IO_MAX_FIELD_PRECISION;
            while (__isdigit(**format) && (ps_<eos_))
                *ps_++ = *((*format)++);
            if (ps_ < eos_) {
                *ps_= '\0';
                conv.precision_ = atoi(s_);
            } else
                conv.precision_ = 0;
        }
    }

    /* Process length modifiers */
    switch (**format) {
    case 'l': ++(*format); conv.mod_ = __MOD_l; if (**format=='l') { ++(*format); conv.mod_ = __MOD_ll; } break;
    case 'L': ++(*format); conv.mod_ = __MOD_L; break;
    case 'h': ++(*format); conv.mod_ = __MOD_h; if (**format=='h') { ++(*format); conv.mod_ = __MOD_hh; } break;
    case 'j': ++(*format); conv.mod_ = __MOD_j; break;
    case 'z': ++(*format); conv.mod_ = __MOD_z; break;
    case 't': ++(*format); conv.mod_ = __MOD_t; break;
    default : break;
    }

    /* The conversion character follows */
    conv.conv_ = *((*format)++);

    return conv;
}

LIBSTD_LPROTO_IMPL(int, __io_fout, char **format, va_list va, __out_proxy *oproxy)
{
    size_t count = 0;
    char *format_end = *format + LIBSTD_LNAME(strlen)(*format);

    char out_buff[__IO_MAX_BUFF];
    const char *peout_buff = (const char *)out_buff + __IO_MAX_BUFF - 1;

    while (*format < format_end) {
        char *pout_buff = out_buff;

        while (**format && (**format != '%') && (pout_buff<peout_buff)) {
            *pout_buff++ = *((*format)++);
            ++count;
        }
        if (pout_buff > out_buff) {
            *pout_buff = '\0';
            oproxy->os_(oproxy, out_buff);
        }

        if (**format) {

            const __pconv conv = LIBSTD_SNAME(__io_pconv_flags)(format, va);

            switch (conv.conv_) {
            case 'c':
                /* This way to support va_arg(va, int)==0 */
                oproxy->oc_(oproxy, va_arg(va, int));
                ++count;
                break;

            case 'n':
                switch (conv.mod_) {
                case __MOD_hh:
                    *(short*)va_arg(va, short *)         = (short)count;
                    break;
                case __MOD_h:
                    *(short*)va_arg(va, short *)         = (short)count;
                    break;
                case __MOD_l:
                    *(long*)va_arg(va, long *)           = (long)count;
                    break;
                case __MOD_ll:
                    *(long long*)va_arg(va, long long *) = (long long)count;
                    break;
                default:
                    *(int*)va_arg(va, int *)             = (int)count;
                    break;
                }
                break;

            case 's':
                {
                    char *str = (char *)va_arg(va, char *);

                    if (str) {
                        size_t str_len = LIBSTD_LNAME(strlen)(str);
                        size_t bl_ = (conv.precision_ >= 0)
                                     ? MIN((unsigned)conv.precision_, str_len)
                                     : str_len;
                        size_t new_len = MAX((unsigned)conv.width_, bl_);
                        if (new_len > 0) {
                            size_t f_ = new_len - bl_;
                            LIBSTD_LNAME(memset)(out_buff, ' ', new_len);
                            LIBSTD_LNAME(strncpy)(&out_buff[conv.ljustify ? 0 : f_], str, bl_ );
                            out_buff[new_len] = '\0';
                            oproxy->os_(oproxy, out_buff);
                            count += new_len;
                        }
                    } else {
                        oproxy->os_(oproxy, "(nil)");
                        count += 5;
                    }
                }
                break;

            case 'e': case 'E': case 'f': case 'F': case 'g': case 'G':
            case 'd': case 'i': case 'o': case 'u': case 'x': case 'X':
            case 'p':
                {
                    int written;
                    written = LIBSTD_SNAME(__io_pconv_do_numeric)(&conv, &va, out_buff);
                    if (written > 0) {
                        oproxy->os_(oproxy, out_buff);
                        count += written;
                    }
                }
                break;

            case '%':
                oproxy->os_(oproxy, "%");
                ++count;
                break;

                /* Not recognized */
            default:
                oproxy->os_(oproxy, "<%unknown>");
                count += 10;
                break;
            }
        }
    }

    return (int)count;
}

/*
 * Treating numerical format
*/
LIBSTD_SPROTO_IMPL(int, __io_pconv_do_numeric, const __pconv *pconv, va_list *va, char *ostring)
{
    int minus_ = 0;
    int plus_  = pconv->plus_;
    int zeropad   = pconv->zeropad;
    char *pendw_ = ostring + pconv->width_;
    char *posend_= ostring + __IO_MAX_BUFF - 1;
    char *pos_   = posend_;
    char *pwhere_= NULL;

    /*
     * Set up output buffer. Working from left to right, since
     * this is the natural way we produce digit output
     */
    LIBSTD_LNAME(memset)(ostring, ' ', __IO_MAX_BUFF);
    *(pos_--) = '\0';

    switch (pconv->conv_) {

    case 'o': case 'u': case 'x': case 'X':
        plus_ = 0;
        /* fall through */
    case 'p': case 'd': case 'i':
        if (pconv->precision_ >= 0)
            zeropad = 0;
        pos_ = LIBSTD_SNAME(__io_pconv_format_integral)(pconv, va, pos_);
        break;

    case 'e': case 'E':
    case 'f': case 'F':
    case 'g': case 'G':
        pos_ = LIBSTD_SNAME(__io_pconv_format_floating)(pconv, va, pos_);
        break;

    default:
        /* We should not be there */
        break;
    }

    minus_ = (pos_[1]=='-');

    if (!minus_) {
        if (plus_)
            *(pos_--) = '+';
        else if (pconv->space_)
            *(pos_--) = ' ';
    }

    /*
     * Treating justification issues and filling. Right-justify, except
     * if - flags used and resulting string overflows
     */
    pwhere_ = pconv->ljustify || ((posend_ - pos_ ) > pconv->width_ )
              ? ostring
              : pendw_ - (posend_ - pos_ ) + 1;

    /*pos_ = (char *)memccpy(pwhere_, pos_ + 1 , '\0', __IO_MAX_BUFF);*/
    /* We shall assume that ostrinsize is > 0 */
    {
        size_t s = __IO_MAX_BUFF;
        char *ps1_ = pwhere_;
        const char *ps2_ = pos_ + 1;
        char *memccpy_ = NULL;
        do {
            if ((*ps1_++ = *ps2_++) == '\0') {
                memccpy_ = ps1_;
                break;
            }
        } while (--s);
        pos_ = memccpy_;
    }
    /* Move eos to end of justified string in case output is smaller than width */
    if (pos_ <= pendw_) {
        LIBSTD_LNAME(memset)(pos_ - 1, ' ', pendw_ - pos_ + 1);
        *pendw_ = '\0';
    }
    /* Treat 0 flags and right-justification*/
    if (!zeropad)
        LIBSTD_LNAME(memset)(ostring, ' ', pwhere_ - ostring);
    else {
        LIBSTD_LNAME(memset)(ostring, '0', pwhere_ - ostring);
        if ((pwhere_ != ostring) && (minus_ || plus_ || pconv->space_)) {
            *ostring = *pwhere_;
            *pwhere_ = '0';
        }
    }

    /* =TBD= In most cases we should be able not to recompute it */
    return (int)LIBSTD_LNAME(strlen)(ostring);
}

/*
 * Treating integral types: process conversion based on flags,
 * outputs in pos
*/
static const char bdigit_[]="0123456789abcdef0123456789ABCDEF";
LIBSTD_SPROTO_IMPL(char *, __io_pconv_format_integral, const __pconv *pconv, va_list *va, char *ostring)
{
    int neg = 0;
    int digits_ = 0;
    int precision = pconv->precision_;
    int64_t svalue_;

    /* In case no precision was given */
    if (precision < 0)
        precision = 1;

    switch (pconv->mod_) {
    case __MOD_h:
        if (pconv->conv_ == 'd' || pconv->conv_ == 'i')
            svalue_ = (int64_t)((short)va_arg(*va, int));
        else  /* 'o' 'u' 'x' 'X' */
            svalue_ = (int64_t)((int)va_arg(*va, int));
        break;
    case __MOD_l:
        if  (pconv->conv_ == 'd' || pconv->conv_ == 'i')
            svalue_ = (int64_t)((long)va_arg(*va, long));
        else /* 'o' 'u' 'x' 'X' */
            svalue_ = (uint64_t)((unsigned long)va_arg(*va, unsigned long));
        break;
    case __MOD_ll:
        if (pconv->conv_ == 'd' || pconv->conv_ == 'i')
            svalue_ = (int64_t) va_arg(*va, int64_t);
        else /* 'o' 'u' 'x' 'X' */
            svalue_ = (uint64_t)va_arg(*va, uint64_t);
        break;
    default:
        if (pconv->conv_ == 'd' || pconv->conv_ == 'i')
            svalue_ = (int64_t)((int)va_arg(*va, int));
        else if (pconv->conv_ == 'p')
            svalue_ = (int64_t)((uintptr_t)va_arg(*va, void *));
        else /* 'o' 'u' 'x' 'X' */
            svalue_ = (int64_t)((unsigned int)va_arg(*va, unsigned int));
        break;
    }

    /* If number is signed and negative, set negative flag */
    if ((pconv->conv_ == 'd' || pconv->conv_ == 'i') && (svalue_ < (int64_t)0)) {
        neg = 1;
        svalue_ = -(int64_t)svalue_;
    }

    /* Form the string */
    if (svalue_ == 0) {
        *ostring-- = '0';
        digits_ = 1;
    }
    else {
        uint64_t c = (uint64_t)svalue_;
        char *o = ostring;

        switch (pconv->conv_) {
        case 'o':
            /* case base == 8 */
            while (c) {
                *o-- = ((int)c & 7) + '0';
                c >>= 3;
            }
            break;
        case 'p':
        case 'x':
        case 'X':
            /* case base == 16 */
            {
                const char* bdigit = bdigit_ + ((pconv->conv_ == 'X') ? 16 : 0);
                while (c) {
                    *o-- = bdigit[(int)c & 15];
                    c >>= 4;
                }
            }
            break;
        default:
            /* case base == 10
             * when possible 32bit division
             */
            while (c) {
                *o-- = (unsigned int)c % 10 + '0';
                c = c < UINT_MAX
                    ? (unsigned int) c / 10
                    : c / 10;
            }
            break;
        }

        digits_ = (int)(ostring - o);
        ostring = o;
    }

    /* Fill till precision reached */
    while (digits_++ <  precision)
        *ostring-- = '0';

    if (pconv->sharp_) {
        if (((pconv->conv_ == 'x') || (pconv->conv_ == 'X' ))) {
            *ostring-- = pconv->conv_;
            *ostring-- = '0';
        }
        else if ((pconv->conv_ == 'o') && (ostring[1] != '0'))
            *ostring-- = '0';
    }
    if (neg)
        *ostring-- = '-';

    return ostring;
}

static const char naninf[] = "-nan\0-NAN\0-inf\0-INF";

LIBSTD_SPROTO_IMPL(char *, __io_pconv_format_floating, const __pconv *pconv, va_list *va, char *ostring)
{
    long double val = pconv->mod_ == __MOD_L
                      ? (long double)va_arg(*va, long double)
                      : (double)va_arg(*va, double);
    int classify_ = LIBSTD_LNAME(fpclassify)(val);

    if (classify_ == FP_NORMAL || classify_ == FP_ZERO || classify_ == FP_SUBNORMAL) {
        int sign = 0;
        if (LIBSTD_LNAME(signbit)(val) != 0) {
            sign = 1;
            val = -val;
        }

        switch (pconv->conv_) {
        case 'f': case 'F':
            ostring = LIBSTD_SNAME(__io_pconv_format_f)(val, pconv, ostring);
            break;

        case 'e': case 'E':
            ostring = LIBSTD_SNAME(__io_pconv_format_e)(val, pconv, ostring);
            break;

        case 'g': case 'G':
            ostring = LIBSTD_SNAME(__io_pconv_format_g)(val, pconv, ostring);
            break;
        }
        if (sign)
            *ostring-- = '-';
    } else {
        const char* str = naninf; /* assume nan */
        const char *tpstr;

        if (classify_==FP_INFINITE)
            str += 10;

        if (pconv->conv_ == 'E' || pconv->conv_ == 'F' || pconv->conv_ == 'G')
            str += 5;

        if (LIBSTD_LNAME(signbit)(val) == 0)
            ++str;

        tpstr = str + LIBSTD_LNAME(strlen)(str) - 1;
        while (tpstr >= str)
            *ostring-- = *tpstr--;
    }
    return ostring;
}

LIBSTD_SPROTO_IMPL(char *, __io_pconv_format_f, long double v, const __pconv *pconv, char *ostring)
{
    char fbuf[__IO_MAX_FLOATING_BUFF];
    char *pfbuf;
    int putdec;
    int decpoint;
    int precision = pconv->precision_;
    char *adv;

    /* If the precision is missing, it is taken as 6 (7.19.6.1 (8)) */
    if (precision < 0)
        precision = 6;

    /* Convert float to string, copy fractional, determine if decimal point needed and copy main part.*/
    pfbuf = LIBSTD_SNAME(__io_ldtoefstr)(v, precision, fbuf, &decpoint);
    adv = ostring;
    ostring = LIBSTD_SNAME(__io_copyfrac)(pfbuf, decpoint, 1, precision, ostring);
    putdec = (pconv->sharp_ || adv != ostring) ? 1 : 0;
    ostring = LIBSTD_SNAME(__io_copymant)(pfbuf, decpoint, putdec, ostring);
    return ostring;
}

LIBSTD_SPROTO_IMPL(char *, __io_pconv_format_e, long double v, const __pconv *pconv, char *ostring)
{
    char fbuf[__IO_MAX_FLOATING_BUFF];
    char *pfbuf;
    int putdec;
    int decpoint;
    int precision = pconv->precision_;
    int exp = 0;
    char *adv;

    /* If the precision is missing, it is taken as 6 (7.19.6.1 (8)) */
    if (precision < 0)
        precision = 6;

    if (v) {
        while (v < 1) {
            v*=10;
            --exp;
        }
        while (v >= 10) {
            v/=10;
            ++exp;
        }
    }
    /* Convert float to string, copy fractional, determine if decimal point needed and copy main part.*/
    pfbuf = LIBSTD_SNAME(__io_ldtoefstr)(v, precision, fbuf, &decpoint);

    /* Rounding issues */
    if (decpoint==2) {
        --decpoint;
        ++exp;
        pfbuf[LIBSTD_LNAME(strlen)(pfbuf)-1] = '\0';
    }
    ostring = LIBSTD_SNAME(__io_copyexp)(exp, pconv->conv_, ostring);
    adv = ostring;
    ostring = LIBSTD_SNAME(__io_copyfrac)(pfbuf, decpoint, 1, precision, ostring);
    putdec = (pconv->sharp_ || adv != ostring) ? 1 : 0;
    ostring = LIBSTD_SNAME(__io_copymant)(pfbuf, decpoint, putdec, ostring);
    return ostring;
}

LIBSTD_SPROTO_IMPL(char *, __io_pconv_format_g, long double v, const __pconv *pconv, char *ostring)
{
    char fbuf[__IO_MAX_FLOATING_BUFF];
    char *pfbuf;
    int putdec;
    int decpoint;
    int precision = pconv->precision_;
    char *adv;

    /* If the precision is missing, it is taken as 6 (7.19.6.1 (8)),
       if it is zero, set it to one.*/
    if (precision < 0)
        precision = 6;
    else if (precision == 0)
        precision = 1;

    pfbuf = LIBSTD_SNAME(__io_ldtogstr)(v, precision, fbuf, &decpoint);
    /* If the exponent is less than -4, or greater than or equal to the precision, %e, otherwise %f*/
    if (decpoint < -3 || decpoint > precision) {
        int exp = decpoint - 1;
        decpoint = 1;
        ostring = LIBSTD_SNAME(__io_copyexp)(exp, pconv->conv_-2, ostring);
    }
    adv = ostring;
    ostring = LIBSTD_SNAME(__io_copyfrac)(pfbuf, decpoint, pconv->sharp_, precision, ostring);
    putdec = (pconv->sharp_ || adv != ostring) ? 1 : 0;
    ostring = LIBSTD_SNAME(__io_copymant)(pfbuf, decpoint, putdec, ostring);
    return ostring;
}

/*
 * Common helpers to format floating point numbers
*/
LIBSTD_SPROTO_IMPL(char *, __io_ldtoefstr, long double v, int ndigit, char *os, int *decpoint)
{
    char *p_ = os;
    int digits_ = 0;
    int l_;

    /* For rounding */
    *p_++ = '0';
    ++ndigit;

    /* Prescale */
    while (v > INT_MAX) {
        v /= 10;
        ++digits_;
    }
    while (v && v < 1) {
        v *= 10;
        --digits_;
    }

    /* Write integral part */
    l_ = LIBSTD_SNAME(__io_itoa)((int)v, p_);
    p_ += l_;
    *decpoint = digits_ + l_;
    ndigit += digits_;


    /* Fractional part */
    if ( ndigit > 0 )
        do {
            v -= (int) v;
            v *= 10;
            *p_++  = (int)(v) + '0';
        } while (--ndigit);

    /* Rounding */
    --p_;
    if (*p_ >= '5') {
        char *pt_ = p_;
        while ((*--pt_ += 1) > '9')
            *pt_ = '0';
        if (pt_ == os)  {
            if (p_ > os)
                *p_ = '\0';
            else
                os[1] = '\0';
            *decpoint += 1;
            return os;
        }
    }

    *p_ = '\0';
    return os + 1;
}

LIBSTD_SPROTO_IMPL(char *, __io_ldtogstr, long double v, int ndigit, char *os, int *decpoint)
{
    char *p_ = os + 1;
    int digits_ = 0;
    int l_;

    /* For rounding */
    *os = '0';
    ++ndigit;

    /* Prescale */
    while (v > INT_MAX) {
        v /= 10;
        ++digits_;
    }
    while (v && v < 1) {
        v *= 10;
        --digits_;
    }

    /* Write integral part */
    l_ = LIBSTD_SNAME(__io_itoa)((int)v, p_);
    p_ += l_;
    *decpoint = digits_ + l_;

    /* Fractional part */
    if ( l_ >= ndigit )
        p_ = os + ndigit + 1;
    else if ( (ndigit -= l_) > 0 )
        do {
            v -= (int) v;
            v *= 10;
            *p_++  = (int)(v) + '0';
        } while (--ndigit);

    /* Rounding */
    if (*--p_ >= '5') {
        char *pt_ = p_;
        while ((*--pt_ += 1) > '9')
            *pt_ = '0';
        if (pt_ == os)  {
            *--p_ = '\0';
            *decpoint += 1;
            return os;
        }
    }
    *p_ = '\0';
    return os + 1;
}

LIBSTD_SPROTO_IMPL(int, __io_itoa, int v, char *buf)
{
    char tbuf[__IO_MAX_FLOATING_BUFF];
    char* ptbuf = &tbuf[__IO_MAX_FLOATING_BUFF-1];
    int len;

/*     assert(v >= 0); */

    *ptbuf = '\0';

    do {
        *--ptbuf = (v % 10) + '0';
    } while (v /= 10);

    len = (int)((tbuf + __IO_MAX_FLOATING_BUFF) - ptbuf);
    LIBSTD_LNAME(memcpy)(buf, ptbuf, len);
    return len - 1;
}

/*
 * Produces fractional, mantissa or exponent part
*/
LIBSTD_SPROTO_IMPL(char *, __io_copyfrac, const char *buf, int decpoint, int trail, int precision, char *ostr)
{
   int  i;
   int lenbuf = (int)LIBSTD_LNAME(strlen)(buf);
   const char *pbuf = buf + lenbuf - 1;

   if (precision == 0)
       return ostr;

   /* Fill all unused precision spaces with zeros. */
   if (trail && decpoint > lenbuf)
       for (i = 0; i < precision; ++i)
           *ostr-- = '0';

   if (decpoint >= 0) {
       /* Copy the fractional part of the float into the string. */
       if (decpoint < lenbuf) {
           i = lenbuf - decpoint;
           /* Skip trailing zeros if TRAIL is not set. */
           if (!trail)
               for (; i > 0 && *pbuf == '0'; --i)
                   --pbuf;
           for (; i > 0; --i)
               *ostr-- = *pbuf--;
       }
   }
   else /* if (decpoint < 0) */ {
       /* Place any leading fractional zeros if necessary.*/
       if (lenbuf + (-decpoint) > precision) {
           /* Because of precision constrain we got to eliminate some digits */
           if (-decpoint >= precision) {
               /* We have only space for zeros */
               for (i = 0; i < precision; ++i)
                   *ostr-- = '0';
           }
           else {
               /* We have space for zeros ad for a part of buf */
               int num_ch_pbuf = precision - (-decpoint);
               if (num_ch_pbuf <= lenbuf) {
                   pbuf -= (lenbuf - num_ch_pbuf);
                   for (i = num_ch_pbuf; i > 0; --i)
                       *ostr-- = *pbuf--;
               }
               else {
                   for (i = lenbuf; i > 0; --i)
                       *ostr-- = *pbuf--;
                   for (i = num_ch_pbuf - lenbuf; i > 0; --pbuf, --i)
                       *ostr-- = '0';
               }
               for (i = 0; i < precision - num_ch_pbuf; ++i)
                   *ostr-- = '0';
           }
       }
       else {
           for (i = lenbuf; i > 0; --i)
               *ostr-- = *pbuf--;
           for (i = -decpoint; i > 0; --i)
               *ostr-- = '0';
       }
   }

   return ostr;
}

LIBSTD_SPROTO_IMPL(char *, __io_copymant, const char *buf, int decpoint, int putdec, char *ostr)
{
    int i;
    int lenbuf = (int)LIBSTD_LNAME(strlen)(buf);

    if (putdec)
        *ostr-- = '.';

    /* Place any whole number trailing zeros. */
    for (i = decpoint; i > lenbuf; --i)
        *ostr-- = '0';

    /* Copy the rest of the whole number. */
    if (i > 0) {
        const char *pbuf = NULL;
        /* If the number has both a whole number part and a fractional part,
         * position the last character of the whole number. */
        if (decpoint > 0 && decpoint <= lenbuf)
            pbuf = buf + decpoint - 1;

        while (pbuf >= buf)
            *ostr-- = *pbuf--;
    }
    else
        *ostr-- = '0';

    return ostr;
}

LIBSTD_SPROTO_IMPL(char *, __io_copyexp, int exp, char lexp, char *ostr)
{
    unsigned int c = ABS(exp);

    do {
        *ostr-- = (c % 10) + '0';
    } while (c /= 10);

    if (exp < 10 && exp > -10)
        *ostr-- = '0';

    *ostr-- = (exp < 0) ? '-' : '+';
    *ostr-- = lexp;

    return ostr;
}
