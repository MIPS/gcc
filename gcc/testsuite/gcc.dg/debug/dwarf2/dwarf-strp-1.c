/* Test -fno-dwarf2-indirect-strings.  */
/* Origin: Joseph Myers <joseph@codesourcery.com> */
/* { dg-do compile } */
/* { dg-options "-gdwarf-2 -fno-dwarf2-indirect-strings -dA" } */
/* { dg-final { scan-assembler-not "DW_FORM_strp" } } */

struct abcdefghijklmnop { int abcdefghijklmnop; } x;
