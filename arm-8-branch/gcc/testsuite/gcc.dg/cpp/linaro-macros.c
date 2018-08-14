/* Test __LINARO_RELEASE__ and __LINARO_SPIN__.  */
/* { dg-do compile } */

#if defined(__LINARO_RELEASE__) && __LINARO_RELEASE__ >= 201808
int i;
#else
#error "Bad __LINARO_RELEASE__."
#endif

#if defined(__LINARO_SPIN__) && __LINARO_SPIN__ >= 0
int j;
#else
#error "Bad __LINARO_SPIN__."
#endif
