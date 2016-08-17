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


#pragma acc routine
void bind_f_1 (void)
{
}

#pragma acc routine
extern void bind_f_1 (void);

#pragma acc routine (bind_f_1)


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
