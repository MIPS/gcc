// Build don't link:

struct A { // ERROR - inaccessible
  static int i; // ERROR - inaccessible
};

struct B : private A { };

struct C : public B {
  int f () { return A::i; } // ERROR - A is inaccessible
};
