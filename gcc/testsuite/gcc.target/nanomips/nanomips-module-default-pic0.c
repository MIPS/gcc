/* Default for bare metal is no pic directive.  */
/* { dg-do compile { target nanomips*-*-elf } }  */
/* { dg-options "-march=32r6" }  */
/* { dg-skip-if "" { *-*-* } { "-fpic" "-fPIC" } { "" } } */
/* { dg-final { scan-assembler-not "\\\.module\tpic\[012\]" } }  */

void foo (void) { }
