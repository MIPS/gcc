/* Test invalid use of clauses with routine.  */
/* { dg-do compile } */

#pragma acc routine gang worker /* { dg-error "multiple loop axes" } */
void
f1 (void)
{
}

#pragma acc routine worker gang /* { dg-error "multiple loop axes" } */
void
f1a (void)
{
}

#pragma acc routine gang vector /* { dg-error "multiple loop axes" } */
void
f2 (void)
{
}

#pragma acc routine vector gang /* { dg-error "multiple loop axes" } */
void
f2a (void)
{
}

#pragma acc routine gang seq /* { dg-error "multiple loop axes" } */
void
f3 (void)
{
}

#pragma acc routine seq gang /* { dg-error "multiple loop axes" } */
void
f3a (void)
{
}

#pragma acc routine worker vector /* { dg-error "multiple loop axes" } */
void
f4 (void)
{
}

#pragma acc routine vector worker /* { dg-error "multiple loop axes" } */
void
f4a (void)
{
}

#pragma acc routine worker seq /* { dg-error "multiple loop axes" } */
void
f5 (void)
{
}

#pragma acc routine seq worker /* { dg-error "multiple loop axes" } */
void
f5a (void)
{
}

#pragma acc routine vector seq /* { dg-error "multiple loop axes" } */
void
f6 (void)
{
}

#pragma acc routine seq vector /* { dg-error "multiple loop axes" } */
void
f6a (void)
{
}
