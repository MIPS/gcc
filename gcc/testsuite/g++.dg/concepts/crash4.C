template<typename U, typename T>
requires Convertible<T, U> // { dg-error "expected" }
U convert(const T& t) {
   return t;
}

int main() {
  convert<float>(42); // { dg-error "not declared" }
}

// { dg-error "primary-expression" "" { target *-*-* } 8 }
// { dg-error "before" "" { target *-*-* } 8 }
