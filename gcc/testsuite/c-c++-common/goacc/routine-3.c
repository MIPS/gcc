/* Test valid use of clauses with routine.  */
/* { dg-do compile } */

#pragma acc routine gang
void
f1 (void)
{
}

#pragma acc routine worker
void
f2 (void)
{
}

#pragma acc routine vector
void
f3 (void)
{
}

#pragma acc routine seq
void
f4 (void)
{
}

#pragma acc routine bind (f4a)
void
f5 (void)
{
}

typedef int T;

#pragma acc routine bind (T)
void
f6 (void)
{
}

#pragma acc routine bind ("f7a")
void
f7 (void)
{
}

#pragma acc routine nohost
void
f8 (void)
{
}
