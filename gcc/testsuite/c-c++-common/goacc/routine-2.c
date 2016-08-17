/* Test invalid use of clauses with OpenACC routine.  */

extern float F;
#pragma acc routine bind (F) /* { dg-error ".F. does not refer to a function" } */
extern void F_1 (void);

typedef int T;
#pragma acc routine bind (T) /* { dg-error ".T. does not refer to a function" } */
extern void T_1 (void);

#pragma acc routine (nothing) gang /* { dg-error ".nothing. has not been declared" } */

#pragma acc routine bind (bind_0) /* { dg-error ".bind_0. has not been declared" }*/
extern void bind_0 (void);

extern void a(void), b(void);

#pragma acc routine bind(a) bind(b) /* { dg-error "too many .bind. clauses" } */
extern void bind_1 (void);

#pragma acc routine bind(a) bind("b") /* { dg-error "too many .bind. clauses" } */
extern void bind_2 (void);

#pragma acc routine bind("a") bind("b") /* { dg-error "too many .bind. clauses" } */
extern void bind_3 (void);

#pragma acc routine nohost nohost /* { dg-error "too many .nohost. clauses" } */
extern void nohost (void);


/* bind clause on first OpenACC routine directive but not on following.  */

extern void a_bind_f_1 (void);
#pragma acc routine (a_bind_f_1)


#pragma acc routine \
  bind (a_bind_f_1)
void a_bind_f_1_1 (void)
{
}

#pragma acc routine /* { dg-error "missing .bind. clause when applying .#pragma acc routine. to .\[void \]*a_bind_f_1_1\[\\(\\)\]*., which has already been marked as an accelerator routine" } */
extern void a_bind_f_1_1 (void);

#pragma acc routine (a_bind_f_1_1) /* { dg-error "missing .bind. clause when applying .#pragma acc routine. to .\[void \]*a_bind_f_1_1\[\\(\\)\]*., which has already been marked as an accelerator routine" } */


/* Non-sensical bind clause, but permitted.  */
#pragma acc routine \
  bind ("a_bind_f_2")
void a_bind_f_2 (void)
{
}

#pragma acc routine /* { dg-error "missing .bind. clause when applying .#pragma acc routine. to .\[void \]*a_bind_f_2\[\\(\\)\]*., which has already been marked as an accelerator routine" } */
extern void a_bind_f_2 (void);

#pragma acc routine (a_bind_f_2) /* { dg-error "missing .bind. clause when applying .#pragma acc routine. to .\[void \]*a_bind_f_2\[\\(\\)\]*., which has already been marked as an accelerator routine" } */


#pragma acc routine \
  bind ("a_bind_f_2")
void a_bind_f_2_1 (void)
{
}

#pragma acc routine /* { dg-error "missing .bind. clause when applying .#pragma acc routine. to .\[void \]*a_bind_f_2_1\[\\(\\)\]*., which has already been marked as an accelerator routine" } */
extern void a_bind_f_2_1 (void);

#pragma acc routine (a_bind_f_2_1) /* { dg-error "missing .bind. clause when applying .#pragma acc routine. to .\[void \]*a_bind_f_2_1\[\\(\\)\]*., which has already been marked as an accelerator routine" } */


/* No bind clause on first OpenACC routine directive, but on following.  */

#pragma acc routine
extern void b_bind_f_1 (void);


#pragma acc routine
void b_bind_f_1_1 (void)
{
}

#pragma acc routine \
  bind (b_bind_f_1) /* { dg-error "incompatible .bind. clause when applying .#pragma acc routine. to .\[void \]*b_bind_f_1_1\[\\(\\)\]*., which has already been marked as an accelerator routine" } */
extern void b_bind_f_1_1 (void);

#pragma acc routine (b_bind_f_1_1) \
  bind (b_bind_f_1) /* { dg-error "incompatible .bind. clause when applying .#pragma acc routine. to .\[void \]*b_bind_f_1_1\[\\(\\)\]*., which has already been marked as an accelerator routine" } */


/* Non-sensical bind clause, but permitted.  */
#pragma acc routine
void b_bind_f_2 (void)
{
}

#pragma acc routine \
  bind ("b_bind_f_2") /* { dg-error "incompatible .bind. clause when applying .#pragma acc routine. to .\[void \]*b_bind_f_2\[\\(\\)\]*., which has already been marked as an accelerator routine" } */
extern void b_bind_f_2 (void);

#pragma acc routine (b_bind_f_2) \
  bind ("b_bind_f_2") /* { dg-error "incompatible .bind. clause when applying .#pragma acc routine. to .\[void \]*b_bind_f_2\[\\(\\)\]*., which has already been marked as an accelerator routine" } */


#pragma acc routine
void b_bind_f_2_1 (void)
{
}

#pragma acc routine \
  bind ("b_bind_f_2") /* { dg-error "incompatible .bind. clause when applying .#pragma acc routine. to .\[void \]*b_bind_f_2_1\[\\(\\)\]*., which has already been marked as an accelerator routine" } */
extern void b_bind_f_2_1 (void);

#pragma acc routine (b_bind_f_2_1) \
  bind ("b_bind_f_2") /* { dg-error "incompatible .bind. clause when applying .#pragma acc routine. to .\[void \]*b_bind_f_2_1\[\\(\\)\]*., which has already been marked as an accelerator routine" } */


/* Non-matching bind clauses.  */

#pragma acc routine
void c_bind_f_1a (void)
{
}

#pragma acc routine
extern void c_bind_f_1b (void);


#pragma acc routine \
  bind (c_bind_f_1a)
void c_bind_f_1_1 (void)
{
}

#pragma acc routine \
  bind (c_bind_f_1b) /* { dg-error "incompatible .bind. clause when applying .#pragma acc routine. to .\[void \]*c_bind_f_1_1\[\\(\\)\]*., which has already been marked as an accelerator routine" } */
extern void c_bind_f_1_1 (void);

#pragma acc routine (c_bind_f_1_1) \
  bind (c_bind_f_1b) /* { dg-error "incompatible .bind. clause when applying .#pragma acc routine. to .\[void \]*c_bind_f_1_1\[\\(\\)\]*., which has already been marked as an accelerator routine" } */


/* Non-sensical bind clause, but permitted.  */
#pragma acc routine \
  bind ("c_bind_f_2")
void c_bind_f_2 (void)
{
}

#pragma acc routine \
  bind ("C_BIND_F_2") /* { dg-error "incompatible .bind. clause when applying .#pragma acc routine. to .\[void \]*c_bind_f_2\[\\(\\)\]*., which has already been marked as an accelerator routine" } */
extern void c_bind_f_2 (void);

#pragma acc routine (c_bind_f_2) \
  bind ("C_BIND_F_2") /* { dg-error "incompatible .bind. clause when applying .#pragma acc routine. to .\[void \]*c_bind_f_2\[\\(\\)\]*., which has already been marked as an accelerator routine" } */


#pragma acc routine \
  bind ("c_bind_f_2")
void c_bind_f_2_1 (void)
{
}

#pragma acc routine \
  bind ("C_BIND_F_2") /* { dg-error "incompatible .bind. clause when applying .#pragma acc routine. to .\[void \]*c_bind_f_2_1\[\\(\\)\]*., which has already been marked as an accelerator routine" } */
extern void c_bind_f_2_1 (void);

#pragma acc routine (c_bind_f_2_1) \
  bind ("C_BIND_F_2") /* { dg-error "incompatible .bind. clause when applying .#pragma acc routine. to .\[void \]*c_bind_f_2_1\[\\(\\)\]*., which has already been marked as an accelerator routine" } */
