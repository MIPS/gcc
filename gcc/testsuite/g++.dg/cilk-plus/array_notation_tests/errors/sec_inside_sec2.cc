/* { dg-do compile } */
/* { dg-options "-fcilkplus" } */
int A[256];

int add( int x, int y)
{
  return x+y;
}

void
check () { 
    int a; 
    a = __sec_reduce_min_ind (__sec_reduce_min_ind (A[:])); /* { dg-error "cannot have array notation parameter with zero" } */
    a = __sec_reduce_max_ind (__sec_reduce_min_ind (A[:]));  /* { dg-error "cannot have array notation parameter with zero" } */
    a = __sec_reduce_any_zero (__sec_reduce_min_ind (A[:]));  /* { dg-error "cannot have array notation parameter with zero" } */
    a = __sec_reduce_any_nonzero (__sec_reduce_min_ind (A[:]));  /* { dg-error "cannot have array notation parameter with zero" } */
    a = __sec_reduce_all_nonzero (__sec_reduce_min_ind (A[:]));  /* { dg-error "cannot have array notation parameter with zero" } */
    a = __sec_reduce_max (__sec_reduce_min_ind (A[:])); /*  { dg-error "cannot have array notation parameter with zero" } */
    a = __sec_reduce_min (__sec_reduce_min_ind (A[:])); /*  { dg-error "cannot have array notation parameter with zero" } */
    a = __sec_reduce_mul (__sec_reduce_min_ind (A[:])); /*  { dg-error "cannot have array notation parameter with zero" } */
    a = __sec_reduce_add (__sec_reduce_min_ind (A[:])); /*  { dg-error "cannot have array notation parameter with zero" } */
    a = __sec_reduce (0, __sec_reduce_min_ind (A[:]), add); /* { dg-error "cannot have array notation parameter with zero" } */
    __sec_reduce_mutating (&a,  __sec_reduce_min_ind (A[:]), add);  /* { dg-error "cannot have array notation parameter with zero" } */
}

