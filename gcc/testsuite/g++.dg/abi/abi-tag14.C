// { dg-options "-Wabi-tag" }

inline namespace __cxx11 __attribute ((abi_tag ("cxx11"))) {
  struct A {};
};

// { dg-final { scan-assembler "_Z1aB5cxx11" } }
A a;				// { dg-warning "\"cxx11\"" }

// { dg-final { scan-assembler "_Z1fB5cxx11v" } }
A f() {}			// { dg-warning "\"cxx11\"" }
