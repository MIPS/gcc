/* { dg-skip-if "not yet" { c++ } } */

#include <stdlib.h>

int
main (int argc, char **argv)
{
    int N = 64;
    float *a, *b;
    int i;

    a = (float *) malloc (N * sizeof (float));
    b = (float *) malloc (N * sizeof (float));

    for (i = 0; i < N; i++)
    {
        a[i] = 3.0;
        b[i] = 0.0;
    }

#pragma acc parallel copyin (a[0:N]) copy (b[0:N]) async (1 2) /* { dg-error "expected '\\)' before numeric constant" } */
    {
        int ii;

        for (ii = 0; ii < N; ii++)
            b[ii] = a[ii];
    }

#pragma acc parallel copyin (a[0:N]) copy (b[0:N]) async (1,) /* { dg-error "expected (primary-|)expression before" } */
    {
        int ii;

        for (ii = 0; ii < N; ii++)
            b[ii] = a[ii];
    }

#pragma acc parallel copyin (a[0:N]) copy (b[0:N]) async (,1) /* { dg-error "expected (primary-|)expression before" } */
    {
        int ii;

        for (ii = 0; ii < N; ii++)
            b[ii] = a[ii];
    }

#pragma acc parallel copyin (a[0:N]) copy (b[0:N]) async (1,2,) /* { dg-error "expected (primary-|)expression before" } */
    {
        int ii;

        for (ii = 0; ii < N; ii++)
            b[ii] = a[ii];
    }

#pragma acc parallel copyin (a[0:N]) copy (b[0:N]) async (1,2 3) /* { dg-error "expected '\\)' before numeric constant" } */
    {
        int ii;

        for (ii = 0; ii < N; ii++)
            b[ii] = a[ii];
    }

#pragma acc parallel copyin (a[0:N]) copy (b[0:N]) async (1,2,,) /* { dg-error "expected (primary-|)expression before" } */
    {
        int ii;

        for (ii = 0; ii < N; ii++)
            b[ii] = a[ii];
    }

#pragma acc parallel copyin (a[0:N]) copy (b[0:N]) async (1 /* { dg-error "expected '\\)' before end of line" } */
    {
        int ii;

        for (ii = 0; ii < N; ii++)
            b[ii] = a[ii];
    }

#pragma acc parallel copyin (a[0:N]) copy (b[0:N]) async (*) /* { dg-error "expected (primary-|)expression before" } */
    {
        int ii;

        for (ii = 0; ii < N; ii++)
            b[ii] = a[ii];
    }

#pragma acc parallel copyin (a[0:N]) copy (b[0:N]) async (a)
	/* { dg-error "expected integer expression before" "" { target c } 85 } */
	/* { dg-error "'async' expression must be integral" "" { target c++ } 85 } */
    {
        int ii;

        for (ii = 0; ii < N; ii++)
            b[ii] = a[ii];
    }

#pragma acc parallel copyin (a[0:N]) copy (b[0:N]) async (1.0)
	/* { dg-error "expected integer expression before" "" { target c } 95 } */
	/* { dg-error "'async' expression must be integral" "" { target c++ } 95 } */
    {
        int ii;

        for (ii = 0; ii < N; ii++)
            b[ii] = a[ii];
    }

#pragma acc parallel copyin (a[0:N]) copy (b[0:N]) async () /* { dg-error "expected (primary-|)expression before" } */
    {
        int ii;

        for (ii = 0; ii < N; ii++)
            b[ii] = a[ii];
    }

#pragma acc parallel copyin (a[0:N]) copy (b[0:N]) async
    {
        int ii;

        for (ii = 0; ii < N; ii++)
            b[ii] = a[ii];
    }

#pragma acc parallel copyin (a[0:N]) copy (b[0:N]) wait (1 2) /* { dg-error "expected '\\)' before numeric constant" } */
    {
        int ii;

        for (ii = 0; ii < N; ii++)
            b[ii] = a[ii];
    }

#pragma acc parallel copyin (a[0:N]) copy (b[0:N]) wait (1,) /* { dg-error "expected (primary-|)expression before" } */
    {
        int ii;

        for (ii = 0; ii < N; ii++)
            b[ii] = a[ii];
    }

#pragma acc parallel copyin (a[0:N]) copy (b[0:N]) wait (,1) /* { dg-error "expected (primary-|)expression before" } */
    {
        int ii;

        for (ii = 0; ii < N; ii++)
            b[ii] = a[ii];
    }

#pragma acc parallel copyin (a[0:N]) copy (b[0:N]) wait (1,2,) /* { dg-error "expected (primary-|)expression before" } */
    {
        int ii;

        for (ii = 0; ii < N; ii++)
            b[ii] = a[ii];
    }

#pragma acc parallel copyin (a[0:N]) copy (b[0:N]) wait (1,2 3) /* { dg-error "expected '\\)' before numeric constant" } */
    {
        int ii;

        for (ii = 0; ii < N; ii++)
            b[ii] = a[ii];
    }

#pragma acc parallel copyin (a[0:N]) copy (b[0:N]) wait (1,2,,) /* { dg-error "expected (primary-|)expression before" } */
    {
        int ii;

        for (ii = 0; ii < N; ii++)
            b[ii] = a[ii];
    }

#pragma acc parallel copyin (a[0:N]) copy (b[0:N]) wait (1 /* { dg-error "expected '\\\)' before end of line" } */
    {
	/* { dg-error "expected integer expression list before" "" { target c++ } 169 } */
        int ii;

        for (ii = 0; ii < N; ii++)
            b[ii] = a[ii];
    }

#pragma acc parallel copyin (a[0:N]) copy (b[0:N]) wait (1,*) /* { dg-error "expected (primary-|)expression before" } */
    {
        int ii;

        for (ii = 0; ii < N; ii++)
            b[ii] = a[ii];
    }

#pragma acc parallel copyin (a[0:N]) copy (b[0:N]) wait (1,a) /*{ dg-error "must be integral" } */
    {
        int ii;

        for (ii = 0; ii < N; ii++)
            b[ii] = a[ii];
    }

#pragma acc parallel copyin (a[0:N]) copy (b[0:N]) wait (a) /* { dg-error "must be integral" } */
    {
        int ii;

        for (ii = 0; ii < N; ii++)
            b[ii] = a[ii];
    }

#pragma acc parallel copyin (a[0:N]) copy (b[0:N]) wait (1.0) /* { dg-error "must be integral" } */
    {
        int ii;

        for (ii = 0; ii < N; ii++)
            b[ii] = a[ii];
    }

#pragma acc parallel copyin (a[0:N]) copy (b[0:N]) wait () /* { dg-error "expected (integer |)expression (list |)before" } */
    {
        int ii;

        for (ii = 0; ii < N; ii++)
            b[ii] = a[ii];
    }

#pragma acc parallel copyin (a[0:N]) copy (b[0:N]) wait
    {
        int ii;

        for (ii = 0; ii < N; ii++)
            b[ii] = a[ii];
    }

#pragma acc wait (1 2) /* { dg-error "expected '\\)' before numeric constant" } */

#pragma acc wait (1,) /* { dg-error "expected (primary-|)expression before" } */

#pragma acc wait (,1) /* { dg-error "expected (primary-|)expression before" } */

#pragma acc wait (1,2,) /* { dg-error "expected (primary-|)expression before" } */

#pragma acc wait (1,2 3) /* { dg-error "expected '\\)' before numeric constant" } */

#pragma acc wait (1,2,,) /* { dg-error "expected (primary-|)expression before" } */

#pragma acc wait (1 /* { dg-error "expected '\\\)' before end of line" } */
    /* { dg-error "expected integer expression list before" "" { target c++ } 238 } */

#pragma acc wait (1,*) /* { dg-error "expected (primary-|)expression before" } */

#pragma acc wait (1,a) /* { dg-error "expression must be integral" } */

#pragma acc wait (a) /* { dg-error "expression must be integral" } */

#pragma acc wait (1.0) /* { dg-error "expression must be integral" } */

#pragma acc wait 1 /* { dg-error "expected clause before numeric constant" } */

#pragma acc wait N /* { dg-error "expected clause before 'N'" } */

#pragma acc wait async (1 2) /* { dg-error "expected '\\)' before numeric constant" } */

#pragma acc wait async (1 2) /* { dg-error "expected '\\)' before numeric constant" } */

#pragma acc wait async (1,) /* { dg-error "expected (primary-|)expression before" } */

#pragma acc wait async (,1) /* { dg-error "expected (primary-|)expression before" } */

#pragma acc wait async (1,2,) /* { dg-error "expected (primary-|)expression before" } */

#pragma acc wait async (1,2 3) /* { dg-error "expected '\\)' before numeric constant" } */

#pragma acc wait async (1,2,,) /* { dg-error "expected (primary-|)expression before" } */

#pragma acc wait async (1 /* { dg-error "expected '\\)' before end of line" } */

#pragma acc wait async (*) /* { dg-error "expected (primary-|)expression before " } */

#pragma acc wait async (a)
    /* { dg-error "expected integer expression before" "" { target c } 271 } */
    /* { dg-error "expression must be integral" "" { target c++ } 271 } */

#pragma acc wait async (1.0)
   /* { dg-error "expected integer expression before" "" { target c } 275 } */
   /* { dg-error "expression must be integral" "" { target c++ } 275 } */

    return 0;
}
