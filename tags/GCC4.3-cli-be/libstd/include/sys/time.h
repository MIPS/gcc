/*
 * Copyright (C) 2008, STMicroelectronics NV, All Rights Reserved.
 *
 */

#ifndef _SYS_TIME_H
#define _SYS_TIME_H

#include <__cdefs.h>

__BEGIN_EXTERN_C

#include <time.h>

typedef unsigned long suseconds_t;

struct timeval {
    time_t      tv_sec;     /* seconds */
    suseconds_t tv_usec;    /* microseconds */
};

struct timezone {
    int tz_minuteswest;     /* minutes west of Greenwich */
    int tz_dsttime;         /* type of DST correction */
};

LIBSTD_LPROTO(int, gettimeofday, struct timeval *tv, struct timezone *tz);
LIBSTD_LPROTO(int, settimeofday, const struct timeval *tv , const struct timezone *tz);

__END_EXTERN_C

#endif

