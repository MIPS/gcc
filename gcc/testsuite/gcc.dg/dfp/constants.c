/* { dg-do compile } */
/* { dg-options "-std=gnu99" } */

/* Various decimal floating point constants.  */
_Decimal32 a32 = 1.1df;
_Decimal32 A32 = 10.1DF;
_Decimal64 a64 = 2.2dd;
_Decimal64 A64 = -20.2DD;
_Decimal128 a128 = -3.3dl;
_Decimal128 A128 = 30.3DL;

_Decimal32 b32 = -11e-1df;
_Decimal32 B32 = .101e2DF;
_Decimal64 b64 = 2.2e-0dd;
_Decimal64 B64 = .00000202e2dd;
_Decimal128 b128 = 000.3e0dl;
_Decimal128 B128 = 3000300030003e0DL;

/* N1107 6.4.4.2: The df, dd, dl, DF, DD and DL suffixes shall not be
   used in a hexadecimal-floating-constant.  */

_Decimal32 x32 = 0x1.0p1df;  /* { dg-error "invalid suffix" "hexadecimal floating constant" } */
_Decimal32 X32 = 0x1.0p1DF;  /* { dg-error "invalid suffix" "hexadecimal floating constant" } */
_Decimal64 x64 = 0x2.0p-2dd; /* { dg-error "invalid suffix" "hexadecimal floating constant" } */
_Decimal64 X64 = 0x2.0p2DD;  /* { dg-error "invalid suffix" "hexadecimal floating constant" } */
_Decimal128 x128 = 0x3.0p3dl;  /* { dg-error "invalid suffix" "hexadecimal floating constant" } */
_Decimal128 X128 = 0x3.0p3DL;  /* { dg-error "invalid suffix" "hexadecimal floating constant" } */



