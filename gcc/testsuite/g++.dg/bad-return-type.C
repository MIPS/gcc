/* Verify that we can highlight the return type for various kinds
   of bogus return.  */

// { dg-options "-fdiagnostics-show-caret -fblt -Wreturn-type" }

void test_1 (void) // { dg-line return_line }
{
  return 42; // { dg-error "return-statement with a value, in function returning 'void'" }
  /* { dg-begin-multiline-output "" }
   return 42;
          ^~
     { dg-end-multiline-output "" } */
  // { dg-message "the return type was declared as 'void' here" "" { target *-*-* } return_line }
  /* { dg-begin-multiline-output "" }
 void test_1 (void)
 ^~~~
     { dg-end-multiline-output "" } */
}

/* As before, but with different whitespace.  */

void // { dg-line return_line_2 }
test_2 (void)
{
  return 42; // { dg-error "return-statement with a value, in function returning 'void'" }
  /* { dg-begin-multiline-output "" }
   return 42;
          ^~
     { dg-end-multiline-output "" } */
  // { dg-message "the return type was declared as 'void' here" "" { target *-*-* } return_line_2 }
  /* { dg-begin-multiline-output "" }
 void
 ^~~~
     { dg-end-multiline-output "" } */
}

/* As before, but with "extern".  */

extern void test_3 (void) // { dg-line return_line_3 }
{
  return 42; // { dg-error "return-statement with a value, in function returning 'void'" }
  /* { dg-begin-multiline-output "" }
   return 42;
          ^~
     { dg-end-multiline-output "" } */
  // { dg-message "the return type was declared as 'void' here" "" { target *-*-* } return_line_3 }
  /* { dg-begin-multiline-output "" }
 extern void test_3 (void)
        ^~~~
     { dg-end-multiline-output "" } */
}

/* Type mismatch for non-void return.  */

extern int test_4 (void) // { dg-line return_line_4 }
{
  return; // { dg-error "return-statement with no value, in function returning 'int'" }
  /* { dg-begin-multiline-output "" }
   return;
   ^~~~~~
     { dg-end-multiline-output "" } */
  // { dg-message "the return type was declared as 'int' here" "" { target *-*-* } return_line_4 }
  /* { dg-begin-multiline-output "" }
 extern int test_4 (void)
        ^~~
     { dg-end-multiline-output "" } */
}

/* Falling off the end of a non-void function.  */

extern int test_5 (void) // { dg-line return_line_5 }
{
} // { dg-warning "no return statement in function returning non-void" }
/* { dg-begin-multiline-output "" }
 }
 ^
   { dg-end-multiline-output "" } */
// { dg-message "the return type was declared as 'int' here" "" { target *-*-* } return_line_5 }
/* { dg-begin-multiline-output "" }
 extern int test_5 (void)
        ^~~
   { dg-end-multiline-output "" } */

/* Member function.  */

struct test_6
{
  void member (void) // { dg-line return_line_6 }
  {
    return 42; // { dg-error "return-statement with a value, in function returning 'void'" }
    /* { dg-begin-multiline-output "" }
     return 42;
            ^~
       { dg-end-multiline-output "" } */
    // { dg-message "the return type was declared as 'void' here" "" { target *-*-* } return_line_6 }
    /* { dg-begin-multiline-output "" }
   void member (void)
   ^~~~
       { dg-end-multiline-output "" } */
  }
};

/* Function template.  */
// TODO: highlight "void" for these
template <typename T>
void test_7 (T t)
{
  return 42; // { dg-error "return-statement with a value, in function returning 'void'" }
/* { dg-begin-multiline-output "" }
   return 42;
          ^~
   { dg-end-multiline-output "" } */
}

/* Class template.  */

template <typename T>
struct test_8
{
  void member (T t) // { dg-line return_line_8 }
  {
    return 42; // { dg-error "return-statement with a value, in function returning 'void'" }
    /* { dg-begin-multiline-output "" }
     return 42;
            ^~
       { dg-end-multiline-output "" } */
    // { dg-message "the return type was declared as 'void' here" "" { target *-*-* } return_line_8 }
    /* { dg-begin-multiline-output "" }
   void member (T t)
   ^~~~
       { dg-end-multiline-output "" } */
  }
};

// TODO: complex return type e.g. const char *
