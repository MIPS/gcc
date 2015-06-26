/* { dg-compile } */
/* { dg-options "-mips64r2" } */

__attribute__ ((target("arch=mips4")))
void
foo ()
{ /* { dg-error "must be equal or greater than" } */
}

__attribute__ ((target("arch=mips32r2")))
void
foo2 ()
{ /* { dg-error "must be equal or greater than" } */
}

void
foo3 ()
{ /* no error */
}

__attribute__ ((target("arch=mips32r6")))
void
foo4 ()
{ /* { dg-error "mixing R6 and pre-R6 code not supported" } */
}

__attribute__ ((target("arch=mips64r2")))
void
foo5 ()
{ /* no error */
}

__attribute__ ((target("arch=mips64r6")))
void
foo6 ()
{ /* { dg-error "mixing R6 and pre-R6 code not supported" } */
}
