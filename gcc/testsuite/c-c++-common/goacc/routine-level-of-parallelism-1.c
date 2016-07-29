/* Test various aspects of clauses specifying incompatible levels of
   parallelism with the OpenACC routine directive.  The Fortran counterpart is
   ../../gfortran.dg/goacc/routine-level-of-parallelism-1.f.  */

extern void g_1 (void);
#pragma acc routine (g_1) gang gang /* { dg-error "too many 'gang' clauses" } */

#pragma acc routine worker worker /* { dg-error "too many 'worker' clauses" } */
void w_1 (void)
{
}

#pragma acc routine vector vector /* { dg-error "too many 'vector' clauses" } */
void v_1 (void)
{
}

#pragma acc routine seq seq /* { dg-error "too many 'seq' clauses" } */
extern void s_1 (void);


#pragma acc routine gang gang gang /* { dg-error "too many 'gang' clauses" } */
void g_2 (void)
{
}

#pragma acc routine worker worker worker /* { dg-error "too many 'worker' clauses" } */
extern void w_2 (void);

extern void v_2 (void);
#pragma acc routine (v_2) vector vector vector /* { dg-error "too many 'vector' clauses" } */

#pragma acc routine seq seq seq /* { dg-error "too many 'seq' clauses" } */
void s_2 (void)
{
}


#pragma acc routine \
  gang \
  worker /* { dg-error ".worker. specifies a conflicting level of parallelism" } */
void g_3 (void)
{
}
#pragma acc routine (g_3) \
  gang \
  seq /* { dg-error ".seq. specifies a conflicting level of parallelism" } */
#pragma acc routine (g_3) \
  gang \
  vector /* { dg-error ".vector. specifies a conflicting level of parallelism" } */

extern void w_3 (void);
#pragma acc routine (w_3) \
  worker \
  vector /* { dg-error ".vector. specifies a conflicting level of parallelism" } */
#pragma acc routine (w_3) \
  worker \
  gang /* { dg-error ".gang. specifies a conflicting level of parallelism" } */
#pragma acc routine (w_3) \
  worker \
  seq /* { dg-error ".seq. specifies a conflicting level of parallelism" } */

#pragma acc routine \
  vector \
  seq /* { dg-error ".seq. specifies a conflicting level of parallelism" } */
void v_3 (void)
{
}
#pragma acc routine (v_3) \
  vector \
  worker /* { dg-error ".worker. specifies a conflicting level of parallelism" } */
#pragma acc routine (v_3) \
  vector \
  gang /* { dg-error ".gang. specifies a conflicting level of parallelism" } */

extern void s_3 (void);
#pragma acc routine (s_3) \
  seq \
  gang /* { dg-error ".gang. specifies a conflicting level of parallelism" } */
#pragma acc routine (s_3) \
  seq \
  vector /* { dg-error ".vector. specifies a conflicting level of parallelism" } */
#pragma acc routine (s_3) \
  seq \
  worker /* { dg-error ".worker. specifies a conflicting level of parallelism" } */


#pragma acc routine \
  gang gang gang /* { dg-error "too many 'gang' clauses" } */ \
  worker /* { dg-error ".worker. specifies a conflicting level of parallelism" } */ \
  vector /* { dg-error ".vector. specifies a conflicting level of parallelism" } */ \
  seq /* { dg-error ".seq. specifies a conflicting level of parallelism" } */
extern void g_4 (void);
#pragma acc routine (g_4) \
  gang gang gang /* { dg-error "too many 'gang' clauses" } */ \
  vector /* { dg-error ".vector. specifies a conflicting level of parallelism" } */ \
  worker /* { dg-error ".worker. specifies a conflicting level of parallelism" } */ \
  seq /* { dg-error ".seq. specifies a conflicting level of parallelism" } */
#pragma acc routine (g_4) \
  gang gang gang /* { dg-error "too many 'gang' clauses" } */ \
  seq /* { dg-error ".seq. specifies a conflicting level of parallelism" } */ \
  vector /* { dg-error ".vector. specifies a conflicting level of parallelism" } */ \
  worker /* { dg-error ".worker. specifies a conflicting level of parallelism" } */

extern void w_4 (void);
#pragma acc routine (w_4) \
  worker worker worker /* { dg-error "too many 'worker' clauses" } */ \
  seq /* { dg-error ".seq. specifies a conflicting level of parallelism" } */ \
  vector /* { dg-error ".vector. specifies a conflicting level of parallelism" } */ \
  gang /* { dg-error ".gang. specifies a conflicting level of parallelism" } */
#pragma acc routine (w_4) \
  worker worker worker /* { dg-error "too many 'worker' clauses" } */ \
  vector /* { dg-error ".vector. specifies a conflicting level of parallelism" } */ \
  seq /* { dg-error ".seq. specifies a conflicting level of parallelism" } */ \
  gang /* { dg-error ".gang. specifies a conflicting level of parallelism" } */
#pragma acc routine (w_4) \
  worker worker worker /* { dg-error "too many 'worker' clauses" } */ \
  seq /* { dg-error ".seq. specifies a conflicting level of parallelism" } */ \
  gang /* { dg-error ".gang. specifies a conflicting level of parallelism" } */ \
  vector /* { dg-error ".vector. specifies a conflicting level of parallelism" } */

#pragma acc routine \
  vector vector vector /* { dg-error "too many 'vector' clauses" } */ \
  worker /* { dg-error ".worker. specifies a conflicting level of parallelism" } */ \
  seq /* { dg-error ".seq. specifies a conflicting level of parallelism" } */ \
  gang /* { dg-error ".gang. specifies a conflicting level of parallelism" } */
void v_4 (void)
{
}
#pragma acc routine (v_4) \
  vector vector vector /* { dg-error "too many 'vector' clauses" } */ \
  seq /* { dg-error ".seq. specifies a conflicting level of parallelism" } */ \
  worker /* { dg-error ".worker. specifies a conflicting level of parallelism" } */ \
  gang /* { dg-error ".gang. specifies a conflicting level of parallelism" } */
#pragma acc routine (v_4) \
  vector vector vector /* { dg-error "too many 'vector' clauses" } */ \
  gang /* { dg-error ".gang. specifies a conflicting level of parallelism" } */ \
  seq /* { dg-error ".seq. specifies a conflicting level of parallelism" } */ \
  worker /* { dg-error ".worker. specifies a conflicting level of parallelism" } */

#pragma acc routine \
  seq seq seq /* { dg-error "too many 'seq' clauses" } */ \
  vector /* { dg-error ".vector. specifies a conflicting level of parallelism" } */ \
  worker /* { dg-error ".worker. specifies a conflicting level of parallelism" } */ \
  gang /* { dg-error ".gang. specifies a conflicting level of parallelism" } */
void s_4 (void)
{
}
#pragma acc routine (s_4) \
  seq seq seq /* { dg-error "too many 'seq' clauses" } */ \
  vector /* { dg-error ".vector. specifies a conflicting level of parallelism" } */ \
  gang /* { dg-error ".gang. specifies a conflicting level of parallelism" } */ \
  worker /* { dg-error ".worker. specifies a conflicting level of parallelism" } */
#pragma acc routine (s_4) \
  seq seq seq /* { dg-error "too many 'seq' clauses" } */ \
  worker /* { dg-error ".worker. specifies a conflicting level of parallelism" } */ \
  vector /* { dg-error ".vector. specifies a conflicting level of parallelism" } */ \
  gang /* { dg-error ".gang. specifies a conflicting level of parallelism" } */


#pragma acc routine \
  gang gang gang /* { dg-error "too many 'gang' clauses" } */ \
  worker worker /* { dg-error "too many 'worker' clauses" } */ \
  /* { dg-error ".worker. specifies a conflicting level of parallelism" "" { target *-*-* } 163 } */ \
  vector vector vector /* { dg-error "too many 'vector' clauses" } */ \
  /* { dg-error ".vector. specifies a conflicting level of parallelism" "" { target *-*-* } 165 } */ \
  seq seq seq seq seq /* { dg-error "too many 'seq' clauses" } */ \
  /* { dg-error ".seq. specifies a conflicting level of parallelism" "" { target *-*-* } 167 } */
void g_5 (void)
{
}
#pragma acc routine (g_5) \
  gang gang gang /* { dg-error "too many 'gang' clauses" } */ \
  vector vector vector vector /* { dg-error "too many 'vector' clauses" } */ \
  /* { dg-error ".vector. specifies a conflicting level of parallelism" "" { target *-*-* } 174 } */ \
  worker worker worker /* { dg-error "too many 'worker' clauses" } */ \
  /* { dg-error ".worker. specifies a conflicting level of parallelism" "" { target *-*-* } 176 } */ \
  seq seq seq seq seq seq /* { dg-error "too many 'seq' clauses" } */ \
  /* { dg-error ".seq. specifies a conflicting level of parallelism" "" { target *-*-* } 178 } */
#pragma acc routine (g_5) \
  gang gang gang /* { dg-error "too many 'gang' clauses" } */ \
  seq seq seq seq seq /* { dg-error "too many 'seq' clauses" } */ \
  /* { dg-error ".seq. specifies a conflicting level of parallelism" "" { target *-*-* } 182 } */ \
  vector vector vector vector /* { dg-error "too many 'vector' clauses" } */ \
  /* { dg-error ".vector. specifies a conflicting level of parallelism" "" { target *-*-* } 184 } */ \
  worker worker worker worker /* { dg-error "too many 'worker' clauses" } */ \
  /* { dg-error ".worker. specifies a conflicting level of parallelism" "" { target *-*-* } 186 } */

#pragma acc routine \
  worker worker worker /* { dg-error "too many 'worker' clauses" } */ \
  seq seq seq seq /* { dg-error "too many 'seq' clauses" } */ \
  /* { dg-error ".seq. specifies a conflicting level of parallelism" "" { target *-*-* } 191 } */ \
  vector vector vector vector vector /* { dg-error "too many 'vector' clauses" } */ \
  /* { dg-error ".vector. specifies a conflicting level of parallelism" "" { target *-*-* } 193 } */ \
  gang gang /* { dg-error "too many 'gang' clauses" } */ \
  /* { dg-error ".gang. specifies a conflicting level of parallelism" "" { target *-*-* } 195 } */
extern void w_5 (void);
#pragma acc routine (w_5) \
  worker worker worker /* { dg-error "too many 'worker' clauses" } */ \
  vector vector vector /* { dg-error "too many 'vector' clauses" } */ \
  /* { dg-error ".vector. specifies a conflicting level of parallelism" "" { target *-*-* } 200 } */ \
  seq seq seq /* { dg-error "too many 'seq' clauses" } */ \
  /* { dg-error ".seq. specifies a conflicting level of parallelism" "" { target *-*-* } 202 } */ \
  gang gang /* { dg-error "too many 'gang' clauses" } */ \
  /* { dg-error ".gang. specifies a conflicting level of parallelism" "" { target *-*-* } 204 } */
#pragma acc routine (w_5) \
  worker worker worker /* { dg-error "too many 'worker' clauses" } */ \
  seq seq /* { dg-error "too many 'seq' clauses" } */ \
  /* { dg-error ".seq. specifies a conflicting level of parallelism" "" { target *-*-* } 208 } */ \
  gang gang /* { dg-error "too many 'gang' clauses" } */ \
  /* { dg-error ".gang. specifies a conflicting level of parallelism" "" { target *-*-* } 210 } */ \
  vector vector vector vector /* { dg-error "too many 'vector' clauses" } */ \
  /* { dg-error ".vector. specifies a conflicting level of parallelism" "" { target *-*-* } 212 } */

#pragma acc routine \
  vector vector vector /* { dg-error "too many 'vector' clauses" } */ \
  worker worker worker /* { dg-error "too many 'worker' clauses" } */ \
  /* { dg-error ".worker. specifies a conflicting level of parallelism" "" { target *-*-* } 217 } */ \
  seq seq seq seq seq seq /* { dg-error "too many 'seq' clauses" } */ \
  /* { dg-error ".seq. specifies a conflicting level of parallelism" "" { target *-*-* } 219 } */ \
  gang gang /* { dg-error "too many 'gang' clauses" } */ \
  /* { dg-error ".gang. specifies a conflicting level of parallelism" "" { target *-*-* } 221 } */
extern void v_5 (void);
#pragma acc routine (v_5) \
  vector vector vector /* { dg-error "too many 'vector' clauses" } */ \
  seq seq seq seq seq seq /* { dg-error "too many 'seq' clauses" } */ \
  /* { dg-error ".seq. specifies a conflicting level of parallelism" "" { target *-*-* } 226 } */ \
  worker worker /* { dg-error "too many 'worker' clauses" } */ \
  /* { dg-error ".worker. specifies a conflicting level of parallelism" "" { target *-*-* } 228 } */ \
  gang gang /* { dg-error "too many 'gang' clauses" } */ \
  /* { dg-error ".gang. specifies a conflicting level of parallelism" "" { target *-*-* } 230 } */
#pragma acc routine (v_5) \
  vector vector vector /* { dg-error "too many 'vector' clauses" } */ \
  gang gang /* { dg-error "too many 'gang' clauses" } */ \
  /* { dg-error ".gang. specifies a conflicting level of parallelism" "" { target *-*-* } 234 } */ \
  seq seq seq seq seq /* { dg-error "too many 'seq' clauses" } */ \
  /* { dg-error ".seq. specifies a conflicting level of parallelism" "" { target *-*-* } 236 } */ \
  worker worker /* { dg-error "too many 'worker' clauses" } */ \
  /* { dg-error ".worker. specifies a conflicting level of parallelism" "" { target *-*-* } 238 } */

extern void s_5 (void);
#pragma acc routine (s_5) \
  seq seq seq /* { dg-error "too many 'seq' clauses" } */ \
  vector vector vector vector /* { dg-error "too many 'vector' clauses" } */ \
  /* { dg-error ".vector. specifies a conflicting level of parallelism" "" { target *-*-* } 244 } */ \
  worker worker worker worker worker /* { dg-error "too many 'worker' clauses" } */ \
  /* { dg-error ".worker. specifies a conflicting level of parallelism" "" { target *-*-* } 246 } */ \
  gang gang /* { dg-error "too many 'gang' clauses" } */ \
  /* { dg-error ".gang. specifies a conflicting level of parallelism" "" { target *-*-* } 248 } */
#pragma acc routine (s_5) \
  seq seq seq /* { dg-error "too many 'seq' clauses" } */ \
  vector vector vector vector /* { dg-error "too many 'vector' clauses" } */ \
  /* { dg-error ".vector. specifies a conflicting level of parallelism" "" { target *-*-* } 252 } */ \
  gang gang /* { dg-error "too many 'gang' clauses" } */ \
  /* { dg-error ".gang. specifies a conflicting level of parallelism" "" { target *-*-* } 254 } */ \
  worker worker worker /* { dg-error "too many 'worker' clauses" } */ \
  /* { dg-error ".worker. specifies a conflicting level of parallelism" "" { target *-*-* } 256 } */
#pragma acc routine (s_5) \
  seq seq seq /* { dg-error "too many 'seq' clauses" } */ \
  worker worker /* { dg-error "too many 'worker' clauses" } */ \
  /* { dg-error ".worker. specifies a conflicting level of parallelism" "" { target *-*-* } 260 } */ \
  vector vector vector vector /* { dg-error "too many 'vector' clauses" } */ \
  /* { dg-error ".vector. specifies a conflicting level of parallelism" "" { target *-*-* } 262 } */ \
  gang gang /* { dg-error "too many 'gang' clauses" } */ \
  /* { dg-error ".gang. specifies a conflicting level of parallelism" "" { target *-*-* } 264 } */


/* Like the *_5 tests, but with the order of clauses changed in the second and
   following routine directives for the specific *_5 function.  */

#pragma acc routine \
  gang gang gang /* { dg-error "too many 'gang' clauses" } */ \
  worker worker /* { dg-error "too many 'worker' clauses" } */ \
  /* { dg-error ".worker. specifies a conflicting level of parallelism" "" { target *-*-* } 273 } */ \
  vector vector vector /* { dg-error "too many 'vector' clauses" } */ \
  /* { dg-error ".vector. specifies a conflicting level of parallelism" "" { target *-*-* } 275 } */ \
  seq seq seq seq seq /* { dg-error "too many 'seq' clauses" } */ \
  /* { dg-error ".seq. specifies a conflicting level of parallelism" "" { target *-*-* } 277 } */
void g_6 (void)
{
}
#pragma acc routine (g_6) \
  vector vector vector vector /* { dg-error "too many 'vector' clauses" } */ \
  /* { dg-error "incompatible .vector. clause when applying .#pragma acc routine. to .\[void \]*g_6\[\\(\\)\]*., which has already been marked as an accelerator routine" "" { target *-*-* } 283 } */ \
  gang gang gang /* { dg-error "too many 'gang' clauses" } */ \
  /* { dg-error ".gang. specifies a conflicting level of parallelism" "" { target *-*-* } 285 } */ \
  worker worker worker /* { dg-error "too many 'worker' clauses" } */ \
  /* { dg-error ".worker. specifies a conflicting level of parallelism" "" { target *-*-* } 287 } */ \
  seq seq seq seq seq seq /* { dg-error "too many 'seq' clauses" } */ \
  /* { dg-error ".seq. specifies a conflicting level of parallelism" "" { target *-*-* } 289 } */
#pragma acc routine (g_6) \
  seq seq seq seq seq /* { dg-error "too many 'seq' clauses" } */ \
  /* { dg-error "incompatible .seq. clause when applying .#pragma acc routine. to .\[void \]*g_6\[\\(\\)\]*., which has already been marked as an accelerator routine" "" { target *-*-* } 292 } */ \
  gang gang gang /* { dg-error "too many 'gang' clauses" } */ \
  /* { dg-error ".gang. specifies a conflicting level of parallelism" "" { target *-*-* } 294 } */ \
  vector vector vector vector /* { dg-error "too many 'vector' clauses" } */ \
  /* { dg-error ".vector. specifies a conflicting level of parallelism" "" { target *-*-* } 296 } */ \
  worker worker worker worker /* { dg-error "too many 'worker' clauses" } */ \
  /* { dg-error ".worker. specifies a conflicting level of parallelism" "" { target *-*-* } 298 } */

#pragma acc routine \
  worker worker worker /* { dg-error "too many 'worker' clauses" } */ \
  seq seq seq seq /* { dg-error "too many 'seq' clauses" } */ \
  /* { dg-error ".seq. specifies a conflicting level of parallelism" "" { target *-*-* } 303 } */ \
  vector vector vector vector vector /* { dg-error "too many 'vector' clauses" } */ \
  /* { dg-error ".vector. specifies a conflicting level of parallelism" "" { target *-*-* } 305 } */ \
  gang gang /* { dg-error "too many 'gang' clauses" } */ \
  /* { dg-error ".gang. specifies a conflicting level of parallelism" "" { target *-*-* } 307 } */
extern void w_6 (void);
#pragma acc routine (w_6) \
  vector vector vector /* { dg-error "too many 'vector' clauses" } */ \
  /* { dg-error "incompatible .vector. clause when applying .#pragma acc routine. to .\[void \]*w_6\[\\(\\)\]*., which has already been marked as an accelerator routine" "" { target *-*-* } 311 } */ \
  worker worker worker /* { dg-error "too many 'worker' clauses" } */ \
  /* { dg-error ".worker. specifies a conflicting level of parallelism" "" { target *-*-* } 313 } */ \
  seq seq seq /* { dg-error "too many 'seq' clauses" } */ \
  /* { dg-error ".seq. specifies a conflicting level of parallelism" "" { target *-*-* } 315 } */ \
  gang gang /* { dg-error "too many 'gang' clauses" } */ \
  /* { dg-error ".gang. specifies a conflicting level of parallelism" "" { target *-*-* } 317 } */
#pragma acc routine (w_6) \
  seq seq /* { dg-error "too many 'seq' clauses" } */ \
  /* { dg-error "incompatible .seq. clause when applying .#pragma acc routine. to .\[void \]*w_6\[\\(\\)\]*., which has already been marked as an accelerator routine" "" { target *-*-* } 320 } */ \
  worker worker worker /* { dg-error "too many 'worker' clauses" } */ \
  /* { dg-error ".worker. specifies a conflicting level of parallelism" "" { target *-*-* } 322 } */ \
  gang gang /* { dg-error "too many 'gang' clauses" } */ \
  /* { dg-error ".gang. specifies a conflicting level of parallelism" "" { target *-*-* } 324 } */ \
  vector vector vector vector /* { dg-error "too many 'vector' clauses" } */ \
  /* { dg-error ".vector. specifies a conflicting level of parallelism" "" { target *-*-* } 326 } */

#pragma acc routine \
  vector vector vector /* { dg-error "too many 'vector' clauses" } */ \
  worker worker worker /* { dg-error "too many 'worker' clauses" } */ \
  /* { dg-error ".worker. specifies a conflicting level of parallelism" "" { target *-*-* } 331 } */ \
  seq seq seq seq seq seq /* { dg-error "too many 'seq' clauses" } */ \
  /* { dg-error ".seq. specifies a conflicting level of parallelism" "" { target *-*-* } 333 } */ \
  gang gang /* { dg-error "too many 'gang' clauses" } */ \
  /* { dg-error ".gang. specifies a conflicting level of parallelism" "" { target *-*-* } 335 } */
extern void v_6 (void);
#pragma acc routine (v_6) \
  seq seq seq seq seq seq /* { dg-error "too many 'seq' clauses" } */ \
  /* { dg-error "incompatible .seq. clause when applying .#pragma acc routine. to .\[void \]*v_6\[\\(\\)\]*., which has already been marked as an accelerator routine" "" { target *-*-* } 339 } */ \
  vector vector vector /* { dg-error "too many 'vector' clauses" } */ \
  /* { dg-error ".vector. specifies a conflicting level of parallelism" "" { target *-*-* } 341 } */ \
  worker worker /* { dg-error "too many 'worker' clauses" } */ \
  /* { dg-error ".worker. specifies a conflicting level of parallelism" "" { target *-*-* } 343 } */ \
  gang gang /* { dg-error "too many 'gang' clauses" } */ \
  /* { dg-error ".gang. specifies a conflicting level of parallelism" "" { target *-*-* } 345 } */
#pragma acc routine (v_6) \
  gang gang /* { dg-error "too many 'gang' clauses" } */ \
  /* { dg-error "incompatible .gang. clause when applying .#pragma acc routine. to .\[void \]*v_6\[\\(\\)\]*., which has already been marked as an accelerator routine" "" { target *-*-* } 348 } */ \
  vector vector vector /* { dg-error "too many 'vector' clauses" } */ \
  /* { dg-error ".vector. specifies a conflicting level of parallelism" "" { target *-*-* } 350 } */ \
  seq seq seq seq seq /* { dg-error "too many 'seq' clauses" } */ \
  /* { dg-error ".seq. specifies a conflicting level of parallelism" "" { target *-*-* } 352 } */ \
  worker worker /* { dg-error "too many 'worker' clauses" } */ \
  /* { dg-error ".worker. specifies a conflicting level of parallelism" "" { target *-*-* } 354 } */

extern void s_6 (void);
#pragma acc routine (s_6) \
  seq seq seq /* { dg-error "too many 'seq' clauses" } */ \
  vector vector vector vector /* { dg-error "too many 'vector' clauses" } */ \
  /* { dg-error ".vector. specifies a conflicting level of parallelism" "" { target *-*-* } 360 } */ \
  worker worker worker worker worker /* { dg-error "too many 'worker' clauses" } */ \
  /* { dg-error ".worker. specifies a conflicting level of parallelism" "" { target *-*-* } 362 } */ \
  gang gang /* { dg-error "too many 'gang' clauses" } */ \
  /* { dg-error ".gang. specifies a conflicting level of parallelism" "" { target *-*-* } 364 } */
#pragma acc routine (s_6) \
  vector vector vector vector /* { dg-error "too many 'vector' clauses" } */ \
  /* { dg-error "incompatible .vector. clause when applying .#pragma acc routine. to .\[void \]*s_6\[\\(\\)\]*., which has already been marked as an accelerator routine" "" { target *-*-* } 367 } */ \
  seq seq seq /* { dg-error "too many 'seq' clauses" } */ \
  /* { dg-error ".seq. specifies a conflicting level of parallelism" "" { target *-*-* } 369 } */ \
  gang gang /* { dg-error "too many 'gang' clauses" } */ \
  /* { dg-error ".gang. specifies a conflicting level of parallelism" "" { target *-*-* } 371 } */ \
  worker worker worker /* { dg-error "too many 'worker' clauses" } */ \
  /* { dg-error ".worker. specifies a conflicting level of parallelism" "" { target *-*-* } 373 } */
#pragma acc routine (s_6) \
  worker worker /* { dg-error "too many 'worker' clauses" } */ \
  /* { dg-error "incompatible .worker. clause when applying .#pragma acc routine. to .\[void \]*s_6\[\\(\\)\]*., which has already been marked as an accelerator routine" "" { target *-*-* } 376 } */ \
  seq seq seq /* { dg-error "too many 'seq' clauses" } */ \
  /* { dg-error ".seq. specifies a conflicting level of parallelism" "" { target *-*-* } 378 } */ \
  vector vector vector vector /* { dg-error "too many 'vector' clauses" } */ \
  /* { dg-error ".vector. specifies a conflicting level of parallelism" "" { target *-*-* } 380 } */ \
  gang gang /* { dg-error "too many 'gang' clauses" } */ \
  /* { dg-error ".gang. specifies a conflicting level of parallelism" "" { target *-*-* } 382 } */


/* Like the *_6 tests, but without all the duplicate clauses, so that the
   routine directives are valid in isolation.  */

#pragma acc routine \
  gang
void g_7 (void)
{
}
#pragma acc routine (g_7) \
  vector /* { dg-error "incompatible .vector. clause when applying .#pragma acc routine. to .\[void \]*g_7\[\\(\\)\]*., which has already been marked as an accelerator routine" } */
#pragma acc routine (g_7) \
  seq /* { dg-error "incompatible .seq. clause when applying .#pragma acc routine. to .\[void \]*g_7\[\\(\\)\]*., which has already been marked as an accelerator routine" } */

#pragma acc routine \
  worker
extern void w_7 (void);
#pragma acc routine (w_7) \
  vector /* { dg-error "incompatible .vector. clause when applying .#pragma acc routine. to .\[void \]*w_7\[\\(\\)\]*., which has already been marked as an accelerator routine" } */
#pragma acc routine (w_7) \
  seq /* { dg-error "incompatible .seq. clause when applying .#pragma acc routine. to .\[void \]*w_7\[\\(\\)\]*., which has already been marked as an accelerator routine" } */

#pragma acc routine \
  vector
extern void v_7 (void);
#pragma acc routine (v_7) \
  seq /* { dg-error "incompatible .seq. clause when applying .#pragma acc routine. to .\[void \]*v_7\[\\(\\)\]*., which has already been marked as an accelerator routine" } */
#pragma acc routine (v_7) \
  gang /* { dg-error "incompatible .gang. clause when applying .#pragma acc routine. to .\[void \]*v_7\[\\(\\)\]*., which has already been marked as an accelerator routine" } */

extern void s_7 (void);
#pragma acc routine (s_7) \
  seq
#pragma acc routine (s_7) \
  vector /* { dg-error "incompatible .vector. clause when applying .#pragma acc routine. to .\[void \]*s_7\[\\(\\)\]*., which has already been marked as an accelerator routine" } */
#pragma acc routine (s_7) \
  worker /* { dg-error "incompatible .worker. clause when applying .#pragma acc routine. to .\[void \]*s_7\[\\(\\)\]*., which has already been marked as an accelerator routine" } */


/* Test cases for implicit seq clause.  */

#pragma acc routine \
  gang
void g_8 (void)
{
}
#pragma acc routine (g_8) /* { dg-error "incompatible .seq. clause when applying .#pragma acc routine. to .\[void \]*g_8\[\\(\\)\]*., which has already been marked as an accelerator routine" } */

#pragma acc routine \
  worker
extern void w_8 (void);
#pragma acc routine (w_8) /* { dg-error "incompatible .seq. clause when applying .#pragma acc routine. to .\[void \]*w_8\[\\(\\)\]*., which has already been marked as an accelerator routine" } */

#pragma acc routine \
  vector
extern void v_8 (void);
#pragma acc routine (v_8) /* { dg-error "incompatible .seq. clause when applying .#pragma acc routine. to .\[void \]*v_8\[\\(\\)\]*., which has already been marked as an accelerator routine" } */

extern void s_8 (void);
#pragma acc routine (s_8)
#pragma acc routine (s_8) \
  vector /* { dg-error "incompatible .vector. clause when applying .#pragma acc routine. to .\[void \]*s_8\[\\(\\)\]*., which has already been marked as an accelerator routine" } */
#pragma acc routine (s_8) \
  gang /* { dg-error "incompatible .gang. clause when applying .#pragma acc routine. to .\[void \]*s_8\[\\(\\)\]*., which has already been marked as an accelerator routine" } */
#pragma acc routine (s_8) \
  worker /* { dg-error "incompatible .worker. clause when applying .#pragma acc routine. to .\[void \]*s_8\[\\(\\)\]*., which has already been marked as an accelerator routine" } */
