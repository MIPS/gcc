// Build don't link:

struct A {
  ~A();
};

struct B {
  ~B();
};

int main()
{
  A a;
  a.~B();			// ERROR - wrong name
}
