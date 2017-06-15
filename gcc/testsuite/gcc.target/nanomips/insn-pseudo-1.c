/* { dg-do compile } */

void
unreachable (int i)
{
  asm volatile goto ("b\t.\n\tbeqzc\t%0,%l1" : : "r" (i) : : punt);
punt:
  __builtin_unreachable ();
}

/* Expect assembly like:

	beqz	$4,$L2
				# Anything goes here.
$L2:				# The label must match.
	.insn
$L3 = .				# It's there, but we don't care.
	.end	unreachable

   that is .insn to be inserted if a code label is at function's end.  */

/* { dg-final { scan-assembler "\tbeqzc\t\\\$a\[0-7\]+,(.L\[0-9\]+)\n.*\n\\1:\n\t\\.insn\n(?:.L\[0-9\]+ = \\.\n)?\t\\.end\tunreachable\n" } } */
