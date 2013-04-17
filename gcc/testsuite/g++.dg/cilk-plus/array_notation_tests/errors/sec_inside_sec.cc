/* { dg-do compile } */
/* { dg-options "-fcilkplus" } */

int main ()
{
    int a;
      a = __sec_reduce_add(__sec_reduce_add(1)); /* { dg-error "parameter with zero rank" } */
      a = __sec_reduce_mul(__sec_reduce_add(1)); /* { dg-error "parameter with zero rank" } */
      a = __sec_reduce_max(__sec_reduce_add(1)); /* { dg-error "parameter with zero rank" } */
      a = __sec_reduce_min(__sec_reduce_add(1)); /* { dg-error "parameter with zero rank" } */
      a = __sec_reduce_any_zero(__sec_reduce_add(1)); /* { dg-error "parameter with zero rank" } */
      a = __sec_reduce_any_nonzero(__sec_reduce_add(1)); /* { dg-error "parameter with zero rank" } */
      a = __sec_reduce_all_zero(__sec_reduce_add(1)); /* { dg-error "parameter with zero rank" } */
      a = __sec_reduce_all_nonzero(__sec_reduce_add(1)); /* { dg-error "parameter with zero rank" } */
      a = __sec_reduce_max_ind(__sec_reduce_add(1)); /* { dg-error "parameter with zero rank" } */
      a = __sec_reduce_min_ind(__sec_reduce_add(1)); /* { dg-error "parameter with zero rank" } */
      a = __sec_reduce(0, __sec_reduce_add(1), __sec_reduce_add); /* { dg-error "parameter with zero rank" } */
      __sec_reduce_mutating (&a, __sec_reduce_add(1), __sec_reduce_add); /* { dg-error "parameter with zero rank" } */
        return 0;
}


