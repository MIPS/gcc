/* { dg-do compile } */
/* { dg-options "-std=gnu99" } */

/* Check if fixed-point constants are ok.  */

_Accum k0 = 0k;
_Accum k1 = 9999.0e0k;
_Accum k2 = 9999.0k;
_Accum k3 = 9999k;
_Accum k4 = 9999e0k;
_Accum k5 = 09999k;
_Accum k6 = 09999e0k;
_Accum k7 = 09999.0e0k;
_Accum k8 = 09999.0k;
_Accum k9 = 0x270fp0k;
_Accum k10 = 0x270f.0p0k;
