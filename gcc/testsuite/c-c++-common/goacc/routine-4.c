/* Test invalid use of clauses with routine.  */
/* { dg-do compile } */

#pragma acc routine gang worker /* { dg-error "invalid combination" } */
void
f1 (void)
{
}

#pragma acc routine worker gang /* { dg-error "invalid combination" } */
void
f1a (void)
{
}

#pragma acc routine gang vector /* { dg-error "invalid combination" } */
void
f2 (void)
{
}

#pragma acc routine vector gang /* { dg-error "invalid combination" } */
void
f2a (void)
{
}

#pragma acc routine gang seq /* { dg-error "invalid combination" } */
void
f3 (void)
{
}

#pragma acc routine seq gang /* { dg-error "invalid combination" } */
void
f3a (void)
{
}

#pragma acc routine worker vector /* { dg-error "invalid combination" } */
void
f4 (void)
{
}

#pragma acc routine vector worker /* { dg-error "invalid combination" } */
void
f4a (void)
{
}

#pragma acc routine worker seq /* { dg-error "invalid combination" } */
void
f5 (void)
{
}

#pragma acc routine seq worker /* { dg-error "invalid combination" } */
void
f5a (void)
{
}

#pragma acc routine vector seq /* { dg-error "invalid combination" } */
void
f6 (void)
{
}

#pragma acc routine seq vector /* { dg-error "invalid combination" } */
void
f6a (void)
{
}

#pragma acc routine (g1) gang worker /* { dg-error "invalid combination" } */
#pragma acc routine (g2) worker gang /* { dg-error "invalid combination" } */
#pragma acc routine (g3) gang vector /* { dg-error "invalid combination" } */
#pragma acc routine (g4) vector gang /* { dg-error "invalid combination" } */
#pragma acc routine (g5) gang seq /* { dg-error "invalid combination" } */
#pragma acc routine (g6) seq gang /* { dg-error "invalid combination" } */
#pragma acc routine (g7) worker vector /* { dg-error "invalid combination" } */
#pragma acc routine (g8) vector worker /* { dg-error "invalid combination" } */
#pragma acc routine (g9) worker seq /* { dg-error "invalid combination" } */
#pragma acc routine (g10) seq worker /* { dg-error "invalid combination" } */
#pragma acc routine (g11) vector seq /* { dg-error "invalid combination" } */
#pragma acc routine (g12) seq vector /* { dg-error "invalid combination" } */
