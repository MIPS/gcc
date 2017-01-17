/* { dg-do compile } */
/* { dg-options "-munique-sections=unique-sections.txt" } */
/* { dg-final { scan-assembler "\t.section\tfoo.f" } } */
/* { dg-final { scan-assembler "\t.section\tfoof" } } */
/* { dg-final { scan-assembler "\t.section\tbar.h" } } */
/* { dg-final { scan-assembler "\t.section\tbaz" } } */
/* { dg-final { scan-assembler "\t.section\tbar.k" } } */

int __attribute__((section("foo"))) f (void) { return 0; }
int __attribute__((section("foof"))) g (void) { return 0; }
int __attribute__((section("bar"))) h (void) { return 0; }
int __attribute__((section("baz"))) i (void) { return 0; }
int j (void) { return 0; }
int __attribute__((section("bar"))) k;
int l;
