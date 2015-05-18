/* { dg-do compile */
/* { dg-options "-O0 -gdwarf -dA" } */

/* Test that various DIEs are generated early.  */

/* We're looking for something like:

   # (DIE (0xff) DW_TAG_variable) (early)
   # DW_AT_name: "global"
   ...
   # (DIE (0xff) DW_TAG_subprogram) (early)
   # DW_AT_external
   # DW_AT_name: "function"
*/

/* { dg-final { scan-assembler "DW_TAG_variable. .early.\[\n\r]\[^\n\r]*DW_AT_name: .global" } } */
/* { dg-final { scan-assembler "DW_TAG_subprogram. .early.\[\n\r]\[^\n\r]*DW_AT_external\[\n\r]\[^\n\r]*DW_AT_name: .function" } } */

int global;
void function()
{
  global=6;
}

/* Test that nested functions get their appropriate early DIEs.

   We're looking for:

   # (DIE (0xff) DW_TAG_subprogram) (early)
   # DW_AT_external
   # DW_AT_name: "foo1"
   ...
   # (DIE (0xff) DW_TAG_subprogram) (early)
   # DW_AT_name: "foo2"
   ...
   # (DIE (0xff) DW_TAG_subprogram) (early)
   # DW_AT_name: "foo3"
 */
/* { dg-final { scan-assembler "DW_TAG_subprogram. .early.\[\n\r]\[^\n\r]*DW_AT_external\[\n\r]\[^\n\r]*DW_AT_name: .foo1" } } */
/* { dg-final { scan-assembler "DW_TAG_subprogram. .early.\[\n\r]\[^\n\r]*DW_AT_name: .foo2" } } */
/* { dg-final { scan-assembler "DW_TAG_subprogram. .early.\[\n\r]\[^\n\r]*DW_AT_name: .foo3" } } */

void foo1()
{
  void foo2 () {
      void foo3 () {
      }
  }
  foo2();
}
