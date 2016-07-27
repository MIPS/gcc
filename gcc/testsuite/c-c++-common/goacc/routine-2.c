/* Test invalid use of clauses with OpenACC routine.  */

#pragma acc routine (nothing) gang /* { dg-error "not been declared" } */
