// { dg-do compile }
// { dg-options "-std=c++2a" }

// Change in grammar for the expression trailing `requires`.
template<typename T>
  requires true != false // { dg-error "expected unqualified-id" }
void f1(T)
{ }

template<typename T>
void f3(T) requires true != false // { dg-error "expected initializer" }
{ }

template<typename T>
  requires requires {
    requires true == false; // OK: logical-or-expressions allowed here.
  }
void f3(T)
{ }

template<typename T> 
concept bool C1 = true; // { dg-warning "deprecated as a declaration specifier" }
template<typename T> 
bool concept C1 = true; // { dg-warning "deprecated as a declaration specifier" }

template<typename T>
concept C2 = true; // OK
template<typename T>
concept C2 = true; // { dg-error "redefinition" }
template<typename T, typename U>
concept C2 = true; // { dg-error "different template parameters" }
template<int>
concept C2 = true; // { dg-error "different template parameters" }
int C2 = 0; // { dg-error "different kind of entity" }

int C3 = 0;
template<typename T>
concept C3 = true; // { dg-error "different kind of entity" }

// Concepts as expressions

template<typename T>
concept True = true;

template<typename T>
concept False = false;

static_assert(True<int>);
static_assert(False<int>); // { dg-error "static assertion failed" }

void f4(True);// { dg-warning "using a concept" }
