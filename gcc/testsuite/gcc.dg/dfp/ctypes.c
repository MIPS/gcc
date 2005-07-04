/* { dg-do compile } */
/* { dg-options "-std=gnu99" } */

/* Tests for sections 3 and 4 of WG14 N1107.  */

/* Section 3: Test for the existence of the types.  */
_Decimal32 sd1;
_Decimal32 dd2;
_Decimal128 td3;

/* Section 4: Test sizes for these types.  */
int ssize[sizeof (_Decimal32) == 4 ? 1 : -1];
int dsize[sizeof (_Decimal64) == 8 ? 1 : -1];
int tsize[sizeof (_Decimal128) == 16 ? 1 : -1];

int salign = __alignof (_Decimal32);
int dalign = __alignof (_Decimal64);
int talign = __alignof (_Decimal128);
