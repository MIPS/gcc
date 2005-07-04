/* { dg-do compile } */
/* { dg-options "-std=gnu99" } */

/* Section 6 of WG14 N1107: constants.  */

_Decimal32 arr1[] =  { 1.1df, 10.1DF, -11e-1df, .101e2DF };
_Decimal64 arr2[] =  { 2.2dd, -20.2DD, -3.3dl, 30.3DL };
_Decimal128 arr3[] = { 000.3e0dl, 3000300030003e0DL };

/* N1107 6.4.4.2: The df, dd, dl, DF, DD and DL suffixes shall not be
   used in a hexadecimal-floating-constant.  */

_Decimal32 bad1 = 0x1.0p1df;   /* { dg-error "invalid suffix" "hexadecimal floating constant" } */
_Decimal32 bad2 = 0x1.0p1DF;   /* { dg-error "invalid suffix" "hexadecimal floating constant" } */
_Decimal64 bad3 = 0x2.0p-2dd;  /* { dg-error "invalid suffix" "hexadecimal floating constant" } */
_Decimal64 bad4 = 0x2.0p2DD;   /* { dg-error "invalid suffix" "hexadecimal floating constant" } */
_Decimal128 bad5 = 0x3.0p3dl;  /* { dg-error "invalid suffix" "hexadecimal floating constant" } */
_Decimal128 bad6 = 0x3.0p3DL;  /* { dg-error "invalid suffix" "hexadecimal floating constant" } */
