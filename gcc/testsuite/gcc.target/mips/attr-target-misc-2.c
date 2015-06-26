/* { dg-compile } */
/* { dg-options "-mips32r2 -mabi=32" } */

__attribute__ ((target("arch=mips32")))
void
foo ()
{ /* { dg-error "must be equal or greater than" } */
}

__attribute__ ((target("arch=mips32r2")))
void
foo2 ()
{ /* no error */
}

void
foo3 ()
{ /* no error */
}

__attribute__ ((target("arch=mips32r5")))
void
foo4 ()
{ /* no error */
}

__attribute__ ((target("arch=mips32r6")))
void
foo5 ()
{ /* { dg-error "mixing R6 and pre-R6 code not supported" } */
}

__attribute__ ((target("arch=mips64r2")))
void
foo6 ()
{ /* no error */
}

__attribute__ ((target("arch=mips64r6")))
void
foo7 ()
{ /* { dg-error "mixing R6 and pre-R6 code not supported" } */
}
