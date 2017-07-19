// { dg-do compile }
// { dg-options "-O2 -Wsuggest-final-types -Wsuggest-final-methods -fdiagnostics-show-caret" }
int c;
struct A { // { dg-warning "final would enable devirtualization of 4 calls" }
/* { dg-begin-multiline-output "" }
 struct A {
        ^
   { dg-end-multiline-output "" } */

virtual void a() {} // { dg-warning "final would enable devirtualization of 2 calls" }
/* { dg-begin-multiline-output "" }
 virtual void a() {}
              ^
   { dg-end-multiline-output "" } */

 virtual void b() {c++;} // { dg-warning "final would enable devirtualization of 2 calls"  }
/* { dg-begin-multiline-output "" }
  virtual void b() {c++;}
               ^
   { dg-end-multiline-output "" } */
};
void
t(struct A *a)
{
  a->a();
  a->a();
  a->b();
  a->b();
}
