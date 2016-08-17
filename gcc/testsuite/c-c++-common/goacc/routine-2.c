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
