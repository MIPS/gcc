/* Verify that we can highlight the return type for various kinds
   of bogus return.  */

/* { dg-options "-fdiagnostics-show-caret -fblt -Wreturn-type" } */

void test_1 (void) // { dg-line return_line }
{
  return 42; // { dg-warning "'return' with a value, in function returning void" }
  /* { dg-begin-multiline-output "" }
   return 42;
          ^~
     { dg-end-multiline-output "" } */
  /* { dg-message "the return type was declared as 'void' here" "" { target *-*-* } return_line } */
  /* { dg-begin-multiline-output "" }
 void test_1 (void)
 ^~~~
     { dg-end-multiline-output "" } */
}

/* As before, but with different whitespace.  */

void // { dg-line return_line_2 }
test_2 (void)
{
  return 42; // { dg-warning "'return' with a value, in function returning void" }
  /* { dg-begin-multiline-output "" }
   return 42;
          ^~
     { dg-end-multiline-output "" } */
  /* { dg-message "the return type was declared as 'void' here" "" { target *-*-* } return_line_2 } */
  /* { dg-begin-multiline-output "" }
 void
 ^~~~
     { dg-end-multiline-output "" } */
}

/* As before, but with "extern".  */

extern void test_3 (void) // { dg-line return_line_3 }
{
  return 42; // { dg-warning "'return' with a value, in function returning void" }
  /* { dg-begin-multiline-output "" }
   return 42;
          ^~
     { dg-end-multiline-output "" } */
  /* { dg-message "the return type was declared as 'void' here" "" { target *-*-* } return_line_3 } */
  /* { dg-begin-multiline-output "" }
 extern void test_3 (void)
        ^~~~
     { dg-end-multiline-output "" } */
}

/* Type mismatch for non-void return.  */

extern int test_4 (void) // { dg-line return_line_4 }
{
  return; // { dg-warning "'return' with no value, in function returning non-void" }
  /* { dg-begin-multiline-output "" }
   return;
   ^~~~~~
     { dg-end-multiline-output "" } */
  /* { dg-message "the return type was declared as 'int' here" "" { target *-*-* } return_line_4 } */
  /* { dg-begin-multiline-output "" }
 extern int test_4 (void)
        ^~~
     { dg-end-multiline-output "" } */
}
