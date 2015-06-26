/* { dg-compile } */
/* { dg-options "-mips32r6 -mabi=32 -mnan=2008 -mfpxx" } */

__attribute__ ((target("arch=mips32")))
void
foo ()
{ /* { dg-error "mixing R6 and pre-R6 code not supported" } */
}

__attribute__ ((target("arch=mips32r2")))
void
foo2 ()
{ /* { dg-error "mixing R6 and pre-R6 code not supported" } */
}

void
foo3 ()
{ /* no error */
}

__attribute__ ((target("arch=mips32r6")))
void
foo4 ()
{ /* no error */
}

__attribute__ ((target("arch=mips64r2")))
void
foo5 ()
{ /* { dg-error "mixing R6 and pre-R6 code not supported" } */
}

__attribute__ ((target("arch=mips64r6")))
void
foo6 ()
{ /* no error */
}
