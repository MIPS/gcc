// { dg-options "-std=c++1z" }

typedef concept int CINT; // { dg-error "'concept' cannot appear in a typedef declaration" }

void f(concept int); // { dg-error "a parameter cannot be declared 'concept'" }

concept int f2(); // { dg-error "result must be bool" }
concept bool f3();

struct X
{
  concept int f4(); // { dg-error "result must be bool|declared with function parameters" }
  concept bool f5(); // { dg-error "declared with function parameters" }
  static concept bool f6();
  static concept bool x; // { dg-error "declared 'concept'" }
  concept int x2; // { dg-error "declared 'concept'" }
  concept ~X(); // { dg-error "a destructor cannot be 'concept'" }
  concept X(); // { dg-error "a constructor cannot be 'concept'" }
};

concept bool X2; // { dg-error "uninitialized" }
