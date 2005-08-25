/* { dg-do compile } */
/* { dg-options "-std=gnu99 -Wextra" } */

#if 0
     /* Not sure if these tests are important or not.  GCC does not
	currently emit warnings for FP overflow.  */

void 
f ()
{
  _Decimal32 d32 = __DEC32_MAX__ + __DEC32_DEN__; /* { dg-warning_ "overflow in expression" } */
  _Decimal64 d64 = __DEC64_MAX__ + __DEC64_DEN__; /* { dg-warning_ "overflow in expression" } */
  _Decimal64 d128 = __DEC128_MAX__ + __DEC128_DEN__; /* { dg-warning_ "overflow in expression" } */
}

static _Decimal32 sd32 = __DEC32_MAX__ + __DEC32_DEN__; /* { dg-warning_ "warning: integer overflow in expression" } */
static _Decimal64 sd64 = __DEC64_MAX__ + __DEC64_DEN__; /* { dg-warning_ "warning: integer overflow in expression" } */
static _Decimal128 sd128 = __DEC128_MAX__ + __DEC128_DEN__; /* { dg-warning_ "warning: integer overflow in expression" } */

_Decimal32 
h1 (void)
{
  return __DEC32_MAX__ + __DEC32_DEN__; /* { dg-warning_ "warning: integer overflow in expression" } */
}

_Decimal64
h2 (void)
{
  return __DEC64_MAX__ + __DEC64_DEN__; /* { dg-warning_ "warning: integer overflow in expression" } */
}

_Decimal128
h3 (void)
{
  return __DEC128_MAX__ + __DEC128_DEN__; /* { dg-warning_ "warning: integer overflow in expression" } */
}

#endif /* 0 */
