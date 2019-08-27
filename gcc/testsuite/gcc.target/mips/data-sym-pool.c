/* { dg-do compile } */
/* { dg-options "-mips16 -mcode-readable=yes" } */
/* { dg-skip-if "per-function expected output" { *-*-* } { "-flto" }  { "" } } */
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

   that is `__pool_*' symbol is inserted before a constant pool.  */

/* { dg-final { scan-assembler "\tl\[wd\]\t\\\$\[0-9\]+,(.L(\[0-9\]+))\n.*\t\\.type\t(__pool_frob_\\2), @object\n\\3:\n\t\\.align\t\[23\]\n\\1:\n\t\\.\[d\]?word\t305419896\n" } } */
