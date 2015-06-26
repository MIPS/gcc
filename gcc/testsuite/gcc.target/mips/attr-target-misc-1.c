/* { dg-compile } */

/* Test if parser rejects similarly named but unsupported options.  */
__attribute__ ((target("dspr100")))
void
f (void)
{ /* { dg-error {attribute\(target\(\"dspr100\"\)\) is unknown} } */
}

/* Test if negative forms are rejected for some options.  */
__attribute__ ((target("no-arch=mips32")))
void
f2 (void)
{ /* { dg-error {attribute\(target\(\"no-arch=mips32\"\)\) is unknown} } */
}

__attribute__ ((target("no-tune=mips32")))
void
f3 (void)
{ /* { dg-error {attribute\(target\(\"no-tune=mips32\"\)\) is unknown} } */
}

__attribute__ ((target("no-fp=xx")))
void
f4 (void)
{ /* { dg-error {attribute\(target\(\"no-fp=xx\"\)\) is unknown} } */
}
