/* { dg-do compile } */

#include <openacc.h>
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

#pragma acc cache (a) /* { dg-error "error: expected '\\\[' before '\\)' token" } */

#pragma acc cache (a[0:N]) copyin (a[0:N]) /* { dg-error "error: expected end of line before 'copyin'" } */

#pragma acc cache () /* { dg-error "error: expected identifier before '\\)' token" } */

#pragma acc cache (a[0:N] b[0:N}) /* { dg-error "error: expected '\\)' before 'b'" } */

#pragma acc cache (a[0:N] /* { dg-error "error: expected '\\)' before end of line" } */

#pragma acc cache (a[ii]) /* { dg-error "error: 'ii' is not a constant" } */

#pragma acc cache (a[idx:n]) /* { dg-error "error: 'n' is not a constant" } */

#pragma acc cache (a[0:N])

#pragma acc cache (a[0:N], b[0:N])

#pragma acc cache (a[0])

#pragma acc cache (a[0], a[1], b[0:N])

#pragma acc cache (a[idx])

#pragma acc cache (a[idx:len])

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
