/* { dg-do compile { target thumb*-*-pe* } } */
/* { dg-final { scan-assembler dll-1.c "\.section\[ \t\]*.drectve\n\[^\n\]*-export:exp.*__imp_imp" } } */
