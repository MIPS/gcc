/* { dg-do "link" } */

class C {
 public:
  virtual ~C();
  virtual void foo();
};
void bar() {
  new C();
}
