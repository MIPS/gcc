/* { dg-do compile } */
/* { dg-options "-mips16 -mcode-readable=yes -mplt" } */

int
frob (void)
{
  return 0x12345678;
}

/* Expect assembly like:

	lw	$2,$L3
						# Anything goes here.
	.type	__pool_frob_3, @object		# Symbol # must match label.
__pool_frob_3:					# The symbol must match.
	.align	2
$L3:						# The label must match.
	.word	305419896

   that is `__pool_*' symbol is inserted before a constant pool.

   This code is built with `-mplt' to prevent the special `__gnu_local_gp'
   symbol from being placed in the constant pool at `-O0' for SVR4 code
   and consequently interfering with test expectations.  */

/* { dg-final { scan-assembler "\tlw\t\\\$\[0-9\]+,(.L(\[0-9\]+))\n.*\t\\.type\t(__pool_frob_\\2), @object\n\\3:\n\t\\.align\t2\n\\1:\n\t\\.word\t305419896\n" } } */
