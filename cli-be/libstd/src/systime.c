
#define __LIBSTD_CRT_BUILD

#include <__cdefs.h>

#include <__host.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>

LIBSTD_LPROTO_IMPL(int, gettimeofday, struct timeval *tv, struct timezone *tz)
{
    if (tz != NULL) {
        errno = ENOTSUP;
        return -1;
    }

    LIBSTD_HNAME(gettimeofday)(&(tv->tv_sec), &(tv->tv_usec));
    
    return 0;
}

LIBSTD_LPROTO_IMPL(int, settimeofday, const struct timeval *tv , const struct timezone *tz)
{
    errno = ENOTSUP;
    return -1;
}
