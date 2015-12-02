/* { dg-options "(-mips16) isa_rev<=5" } */

MIPS16 __attribute__((code_readable ("magic"))) int foo () {} /* { dg-error "code_readable argument is not one of 'yes', 'no' or 'pcrel'" } */

MIPS16 __attribute__((code_readable (1))) int * bar () {} /* { dg-error "code_readable argument is not one of 'yes', 'no' or 'pcrel'" } */
