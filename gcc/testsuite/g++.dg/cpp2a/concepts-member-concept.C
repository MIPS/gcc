// { dg-options "-std=c++2a" }

// FIXME: Diagnostics should be better.

struct Base {
  template<typename T>
    static concept D = __is_same_as(T, int); // { dg-error "does not name a type" }

  template<typename T, typename U>
    static concept E = __is_same_as(T, U); // { dg-error "does not name a type" }
};

// FIXME: Actually trap these.

// { dg-excess-errors "deprecated" }
