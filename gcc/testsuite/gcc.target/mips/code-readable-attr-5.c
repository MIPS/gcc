/* { dg-options "isa_rev<=5" } */

NOMIPS16 __attribute__((code_readable ("pcrel"))) int foo () {} /* { dg-error "cannot use 'pcrel' on non-compressed instruction sets" } */

NOMIPS16 __attribute__((code_readable ("pcrel"))) int * bar () {} /* { dg-error "cannot use 'pcrel' on non-compressed instruction sets" } */
