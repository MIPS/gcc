/* { dg-do compile } */

#include <stdlib.h>

int
main (int argc, char **argv)
{
#define N   2
    int a[N], b[N];
    int i;

    for (i = 0; i < N; i++)
    {
        a[i] = 3;
        b[i] = 0;
    }

#pragma acc parallel copyin (a[0:N]) copyout (b[0:N])
{
    int ii;

    for (ii = 0; ii < N; ii++)
    {
        const int idx = ii;
        int n = 1;
        const int len = n;

#pragma acc cache /* { dg-error "error: expected '\\(' before end of line" } */

#pragma acc cache (a) /* { dg-error "error: expected '\\\['" } */

#pragma acc cache (a[0:N]) copyin (a[0:N]) /* { dg-error "error: expected end of line before 'copyin'" } */

#pragma acc cache () /* { dg-error "error: expected unqualified-id before '\\)' token" } */

#pragma acc cache (a[0:N] b[0:N}) /* { dg-error "error: expected end of line before '\\\}' token|error: expected '\\)' before 'b'" } */

        b[ii] = a[ii];
    }
}


    for (i = 0; i < N; i++)
    {
        if (a[i] != b[i])
            abort ();
    }

    return 0;
}
