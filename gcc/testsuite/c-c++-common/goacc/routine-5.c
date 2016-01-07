/* { dg-do compile } */

#pragma acc routine /* { dg-error "not followed by" } */
int a;

#pragma acc routine /* { dg-error "not followed by" } */
void fn1 (void), fn1b (void);

#pragma acc routine /* { dg-error "not followed by" } */
int b, fn2 (void);

#pragma acc routine /* { dg-error "not followed by" } */
int fn3 (void), b2;

#pragma acc routine /* { dg-error "not followed by" } */
typedef struct c c;

#pragma acc routine /* { dg-error "not followed by" } */
struct d {} d;

#pragma acc routine /* { dg-error "not followed by" } */
#pragma acc routine
int fn4 (void);

int fn5a (void);

#pragma acc routine /* { dg-error "not followed by" } */
#pragma acc routine (fn5a)
int fn5 (void);

#ifdef __cplusplus

#pragma acc routine /* { dg-error "not followed by" "" { target c++ } } */
namespace f {}

namespace g {}

#pragma acc routine /* { dg-error "not followed by" "" { target c++ } } */
using namespace g;

#pragma acc routine (g) /* { dg-error "does not refer to" "" { target c++ } } */

#endif

#pragma acc routine (a) /* { dg-error "does not refer to" } */
  
#pragma acc routine (c) /* { dg-error "does not refer to" } */


void Bar ();

void Foo ()
{
  Bar ();
}

#pragma acc routine (Bar) // { dg-error "must be applied before use" }

#pragma acc routine (Foo) gang // { dg-error "must be applied before definition" }

#pragma acc routine (Baz) // { dg-error "not been declared" }

float vb1;

#pragma acc routine
int
func1 (int a)
{
  vb1 = a + 1; /* { dg-error "invalid use in" } */

  return vb1; /* { dg-error "invalid use in" } */
}

#pragma acc routine
int
func2 (int a)
{
  static int vb2;

  vb2 = a + 1; /* { dg-error "invalid use in" } */

  return vb2; /* { dg-error "invalid use in" } */
}

float vb3;
#pragma acc declare link (vb3)

#pragma acc routine
int
func3 (int a)
{
  vb3 = a + 1; /* { dg-error "invalid use in" } */

  return vb3; /* { dg-error "invalid use in" } */
}

float vb4;
#pragma acc declare create (vb4)

#pragma acc routine
int
func4 (int a)
{
  vb4 = a + 1;

  return vb4;
}
