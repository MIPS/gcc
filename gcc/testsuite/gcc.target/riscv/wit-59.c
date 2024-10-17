/* { dg-do compile } */
/* { dg-options "-O2" } */
/* { dg-skip-if "" { *-*-* } { "-O0" "-Og" "-Os" "-g" "-flto" } } */
typedef short mtype;
mtype bug_test(mtype *myval) {
    mtype retval = 0;
    retval = ((*myval) >> 8) & 0x3F;
    return retval;
}
/* check for the following code snippet
 * lb      a0,1(a0)
 * andi    a0,a0,63
 */
/* { dg-final { scan-assembler "\\slb\\sa0,1\\(a0\\)\n\\sandi\\sa0,a0,63\n" } } */
