/* Test invalid usage of the nohost clause for OpenACC routine directive.
   Exercising different variants for declaring routines.  */

#pragma acc routine seq
int THREE_1(void)
{
  return 3;
}

#pragma acc routine (THREE_1) \
  nohost seq /* { dg-error "incompatible .nohost. clause when applying .#pragma acc routine. to .\[int \]*THREE_1\[\\(\\)\]*., which has already been marked as an accelerator routine" } */

#pragma acc routine \
  nohost seq /* { dg-error "incompatible .nohost. clause when applying .#pragma acc routine. to .\[int \]*THREE_1\[\\(\\)\]*., which has already been marked as an accelerator routine" } */
extern int THREE_1(void);


#pragma acc routine seq
extern void NOTHING_1(void);

#pragma acc routine (NOTHING_1) \
  nohost seq /* { dg-error "incompatible .nohost. clause when applying .#pragma acc routine. to .\[void \]*NOTHING_1\[\\(\\)\]*., which has already been marked as an accelerator routine" } */

void NOTHING_1(void)
{
}

#pragma acc routine \
  nohost seq /* { dg-error "incompatible .nohost. clause when applying .#pragma acc routine. to .\[void \]*NOTHING_1\[\\(\\)\]*., which has already been marked as an accelerator routine" } */
extern void NOTHING_1(void);

#pragma acc routine (NOTHING_1) \
  nohost seq /* { dg-error "incompatible .nohost. clause when applying .#pragma acc routine. to .\[void \]*NOTHING_1\[\\(\\)\]*., which has already been marked as an accelerator routine" } */


extern float ADD_1(float, float);

#pragma acc routine (ADD_1) seq

float ADD_1(float x, float y)
{
  return x + y;
}

#pragma acc routine \
  nohost seq /* { dg-error "incompatible .nohost. clause when applying .#pragma acc routine. to .\[float \]*ADD_1\[\\(float, \\)\]*., which has already been marked as an accelerator routine" } */
extern float ADD_1(float, float);

#pragma acc routine (ADD_1) \
  nohost seq /* { dg-error "incompatible .nohost. clause when applying .#pragma acc routine. to .\[float \]*ADD_1\[\\(float, \\)\]*., which has already been marked as an accelerator routine" } */


/* The same again, but with/without nohost reversed.  */

#pragma acc routine \
  nohost seq
int THREE_2(void)
{
  return 3;
}

#pragma acc routine (THREE_2) seq /* { dg-error "missing .nohost. clause when applying .#pragma acc routine. to .\[int \]*THREE_2\[\\(\\)\]*., which has already been marked as an accelerator routine" } */

#pragma acc routine seq /* { dg-error "missing .nohost. clause when applying .#pragma acc routine. to .\[int \]*THREE_2\[\\(\\)\]*., which has already been marked as an accelerator routine" } */
extern int THREE_2(void);


#pragma acc routine \
  nohost seq
extern void NOTHING_2(void);

#pragma acc routine (NOTHING_2) seq /* { dg-error "missing .nohost. clause when applying .#pragma acc routine. to .\[void \]*NOTHING_2\[\\(\\)\]*., which has already been marked as an accelerator routine" } */

void NOTHING_2(void)
{
}

#pragma acc routine seq /* { dg-error "missing .nohost. clause when applying .#pragma acc routine. to .\[void \]*NOTHING_2\[\\(\\)\]*., which has already been marked as an accelerator routine" } */
extern void NOTHING_2(void);

#pragma acc routine (NOTHING_2) seq /* { dg-error "missing .nohost. clause when applying .#pragma acc routine. to .\[void \]*NOTHING_2\[\\(\\)\]*., which has already been marked as an accelerator routine" } */


extern float ADD_2(float, float);

#pragma acc routine (ADD_2) \
  nohost seq

float ADD_2(float x, float y)
{
  return x + y;
}

#pragma acc routine seq /* { dg-error "missing .nohost. clause when applying .#pragma acc routine. to .\[float \]*ADD_2\[\\(float, \\)\]*., which has already been marked as an accelerator routine" } */
extern float ADD_2(float, float);

#pragma acc routine (ADD_2) seq /* { dg-error "missing .nohost. clause when applying .#pragma acc routine. to .\[float \]*ADD_2\[\\(float, \\)\]*., which has already been marked as an accelerator routine" } */
