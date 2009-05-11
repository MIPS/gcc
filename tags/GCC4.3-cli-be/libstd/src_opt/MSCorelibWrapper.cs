//
// Copyright (C) 2008, Free Software Foundation, Inc.
// Contributed by STMicroelectronics
//

using System.Runtime.InteropServices;

public class MSCorelibWrapper
{
    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__open")]
    unsafe public static extern int open(sbyte* path, int oflag);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__write")]
    unsafe public static extern int write(int filedes, void* buf, int nbytes);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__read")]
    unsafe public static extern int read(int filedes, void* buf, int nbytes);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__close")]
    unsafe public static extern int close(int filedes);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__filesize")]
    unsafe public static extern int filesize(sbyte* path);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__lseek")]
    unsafe public static extern int lseek(int filedes, int offset, int whence);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__fcntl")]
    unsafe public static extern int fcntl(int filedes, int cmd, int arg);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__getenv")]
    unsafe public static extern sbyte* getenv(sbyte* name);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__malloc")]
    unsafe public static extern void* malloc(uint size);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__free")]
    unsafe public static extern void free(void* ptr);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__realloc")]
    unsafe public static extern void* realloc(void* ptr, uint size);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__fpclassify")]
    unsafe public static extern int fpclassify(double d);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__clock")]
    unsafe public static extern int clock();

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__time")]
    unsafe public static extern uint time(void* timer);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__gmtime")]
    unsafe public static extern void gmtime(void* timer,
                                     int* tm_sec,  int* tm_min,  int* tm_hour,
                                     int* tm_mday, int* tm_mon,  int* tm_year,
                                     int* tm_wday, int* tm_yday, int* tm_isdst);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__gettimeofday")]
    unsafe public static extern void gettimeofday (void *_tv_sec, void *_tv_usec);


    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__exit")]
    unsafe public static extern void exit(int status);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__rename")]
    unsafe public static extern int rename(sbyte* oldname, sbyte* newname);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__remove")]
    unsafe public static extern int remove(sbyte* filename);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__link")]
    unsafe public static extern int link(sbyte* oldname, sbyte* newname);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__unlink")]
    unsafe public static extern int unlink(sbyte* name);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__errno")]
    unsafe public static extern int* errno();

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__dbl_epsilon")]
    unsafe public static extern double dbl_epsilon();

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__dbl_minval")]
    unsafe public static extern double dbl_minval();

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__dbl_maxval")]
    unsafe public static extern double dbl_maxval();

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__dbl_hugeval")]
    unsafe public static extern double dbl_hugeval();

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__flt_epsilon")]
    unsafe public static extern float flt_epsilon();

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__flt_minval")]
    unsafe public static extern float flt_minval();

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__flt_maxval")]
    unsafe public static extern float flt_maxval();

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__flt_hugeval")]
    unsafe public static extern float flt_hugeval();

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__flt_nan")]
    unsafe public static extern float flt_nan();

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__flt_infinity")]
    unsafe public static extern float flt_infinity();

    ///////////////////////////////////////////////////////////////////////////////////////
    ///  MATH SECTION /////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////
    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__acos")]
    unsafe public static extern double acos(double d);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__acosf")]
    unsafe public static extern float acosf(float f);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__asin")]
    unsafe public static extern double asin(double d);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__asinf")]
    unsafe public static extern float asinf(float f);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__atan")]
    unsafe public static extern double atan(double d);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__atanf")]
    unsafe public static extern float atanf(float f);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__atan2")]
    unsafe public static extern double atan2(double p0, double p1);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__atan2f")]
    unsafe public static extern float atan2f(float p0, float p1);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__cos")]
    unsafe public static extern double cos(double d);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__cosf")]
    unsafe public static extern float cosf(float f);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__sin")]
    unsafe public static extern double sin(double d);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__sinf")]
    unsafe public static extern float sinf(float f);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__tan")]
    unsafe public static extern double tan(double d);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__tanf")]
    unsafe public static extern float tanf(float f);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__cosh")]
    unsafe public static extern double cosh(double d);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__coshf")]
    unsafe public static extern float coshf(float f);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__sinh")]
    unsafe public static extern double sinh(double d);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__sinhf")]
    unsafe public static extern float sinhf(float f);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__tanh")]
    unsafe public static extern double tanh(double d);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__tanhf")]
    unsafe public static extern float tanhf(float f);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__exp")]
    unsafe public static extern double exp(double d);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__expf")]
    unsafe public static extern float expf(float f);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__exp2")]
    unsafe public static extern double exp2(double d);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__exp2f")]
    unsafe public static extern float exp2f(float f);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__expm1")]
    unsafe public static extern double expm1(double d);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__expm1f")]
    unsafe public static extern float expm1f(float f);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__log")]
    unsafe public static extern double log(double d);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__logf")]
    unsafe public static extern float logf(float f);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__log10")]
    unsafe public static extern double log10(double d);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__log10f")]
    unsafe public static extern float log10f(float f);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__log1p")]
    unsafe public static extern double log1p(double d);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__log1pf")]
    unsafe public static extern float log1pf(float f);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__log2")]
    unsafe public static extern double log2(double d);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__log2f")]
    unsafe public static extern float log2f(float f);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__fabs")]
    unsafe public static extern double fabs(double d);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__fabsf")]
    unsafe public static extern float fabsf(float f);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__pow")]
    unsafe public static extern double pow(double p0, double p1);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__powf")]
    unsafe public static extern float powf(float f0, float f1);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__ceil")]
    unsafe public static extern double ceil(double d);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__ceilf")]
    unsafe public static extern float ceilf(float f);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__cbrt")]
    unsafe public static extern double cbrt(double d);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__cbrtf")]
    unsafe public static extern float cbrtf(float f);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__sqrt")]
    unsafe public static extern double sqrt(double d);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__sqrtf")]
    unsafe public static extern float sqrtf(float f);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__floor")]
    unsafe public static extern double floor(double d);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__floorf")]
    unsafe public static extern float floorf(float f);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__lround")]
    unsafe public static extern int lround(double d);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__lroundf")]
    unsafe public static extern int lroundf(float f);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__ldexp")]
    unsafe public static extern double ldexp(double p0, int p1);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__ldexpf")]
    unsafe public static extern float ldexpf(float p0, int p1);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__scalbn")]
    unsafe public static extern double scalbn(double p0, int p1);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__scalbnf")]
    unsafe public static extern float scalbnf(float p0, int p1);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__scalbln")]
    unsafe public static extern double scalbln(double p0, int p1);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__scalblnf")]
    unsafe public static extern float scalblnf(float p0, int p1);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__hypot")]
    unsafe public static extern double hypot(double p0, double p1);

    [DllImport("MSCorelibWrapper_support.so", EntryPoint="__host__hypotf")]
    unsafe public static extern float hypotf(float p0, float p1);

};

