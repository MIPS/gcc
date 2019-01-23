/* Test valid use of clauses with routine.  */

#pragma acc routine gang
void gang (void) /* { dg-warning "partitioned" 3 } */
{
}

#pragma acc routine worker
void worker (void) /* { dg-warning "partitioned" 2 } */
{
}

#pragma acc routine vector
void vector (void) /* { dg-warning "partitioned" 1 } */
{
}

#pragma acc routine seq
void seq (void)
{
}


#pragma acc routine /* { dg-warning "expecting one of" } */
void bind_f_1 (void)
{
}

#pragma acc routine /* { dg-warning "expecting one of" } */
extern void bind_f_1 (void);

#pragma acc routine (bind_f_1)/* { dg-warning "expecting one of" } */


#pragma acc routine \
  bind (bind_f_1)
void bind_f_1_1 (void)
{
}

#pragma acc routine \
  bind (bind_f_1)
extern void bind_f_1_1 (void);

#pragma acc routine (bind_f_1_1) \
  bind (bind_f_1)


/* Non-sensical bind clause, but permitted.  */
#pragma acc routine \
  bind ("bind_f_2")
void bind_f_2 (void)
{
}

#pragma acc routine \
  bind ("bind_f_2")
extern void bind_f_2 (void);

#pragma acc routine (bind_f_2) \
  bind ("bind_f_2")


#pragma acc routine \
  bind ("bind_f_2")
void bind_f_2_1 (void)
{
}

#pragma acc routine \
  bind ("bind_f_2")
extern void bind_f_2_1 (void);

#pragma acc routine (bind_f_2_1) \
  bind ("bind_f_2")


#pragma acc routine \
  nohost
void nohost (void)
{
}

#pragma acc routine \
  nohost
extern void nohost (void);

#pragma acc routine (nohost) \
  nohost


int main ()
{
#pragma acc kernels num_gangs (32) num_workers (32) vector_length (32) /* { dg-warning "region contains gang partitoned code but is not gang partitioned" } */
  {
    gang ();
    worker ();
    vector ();
    seq ();
    bind_f_1 ();
    bind_f_1_1 ();
    bind_f_2 ();
    bind_f_2_1 ();
    nohost ();
  }

#pragma acc parallel num_gangs (32) num_workers (32) vector_length (32)
  {
    gang ();
    worker ();
    vector ();
    seq ();
    bind_f_1 ();
    bind_f_1_1 ();
    bind_f_2 ();
    bind_f_2_1 ();
    nohost ();
  }

  return 0;
}

/* { dg-warning "expecting one of" "" { target *-*-* } 35 } */
/* { dg-warning "expecting one of" "" { target *-*-* } 41 } */
/* { dg-warning "expecting one of" "" { target *-*-* } 45 } */
/* { dg-warning "expecting one of" "" { target *-*-* } 50 } */
/* { dg-warning "expecting one of" "" { target *-*-* } 56 } */
/* { dg-warning "expecting one of" "" { target *-*-* } 60 } */
/* { dg-warning "expecting one of" "" { target *-*-* } 64 } */
/* { dg-warning "expecting one of" "" { target *-*-* } 70 } */
/* { dg-warning "expecting one of" "" { target *-*-* } 74 } */
/* { dg-warning "expecting one of" "" { target *-*-* } 78 } */
/* { dg-warning "expecting one of" "" { target *-*-* } 84 } */
/* { dg-warning "expecting one of" "" { target *-*-* } 88 } */
