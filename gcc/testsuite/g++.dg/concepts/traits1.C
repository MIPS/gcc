// { dg-options "-std=c++1z" }

template<typename T>
  concept bool Nothrow_assignable() { return __has_nothrow_assign(T); }

template<typename T>
  concept bool Nothrow_constructible() { return __has_nothrow_constructor(T); }

template<typename T>
  concept bool Nothrow_copyable() { return __has_nothrow_copy(T); }

template<typename T>
  concept bool Trivially_assignable() { return __has_trivial_assign(T); }

template<typename T>
  concept bool Trivially_constructible() { return __has_trivial_constructor(T); }

template<typename T>
  concept bool Trivially_copyable() { return __has_trivial_copy(T); }

template<typename T>
  concept bool Trivially_destructible() { return __has_trivial_destructor(T); }

template<typename T>
  concept bool Dynamically_destructible() { return __has_virtual_destructor(T); }

template<typename T>
  concept bool Abstract() { return __is_abstract(T); }

template<typename T>
  concept bool Polymorphic() { return __is_polymorphic(T); }

template<typename T>
  concept bool Class() { return __is_class(T); }

template<typename T>
  concept bool Empty() { return __is_empty(T); }

template<typename T>
  concept bool Enum() { return __is_enum(T); }

template<typename T>
  concept bool Final() { return __is_final(T); }

template<typename T>
  concept bool Literal_type() { return __is_literal_type(T); }

template<typename T>
  concept bool Pod() { return __is_pod(T); }

template<typename T>
  concept bool Standard_layout() { return __is_standard_layout(T); }

template<typename T>
  concept bool Trivial() { return __is_trivial(T); }

template<typename T>
  concept bool Union() { return __is_union(T); }


void f1() requires Nothrow_assignable<void>();
void f2() requires Nothrow_copyable<void>();
void f3() requires Nothrow_constructible<void>();
void f4() requires Trivially_assignable<void>();
void f5() requires Trivially_copyable<void>();
void f6() requires Trivially_constructible<void>();
void f7() requires Trivially_destructible<void>();
void f8() requires Dynamically_destructible<void>();
void f9() requires Class<void>();
void f10() requires Empty<void>();
void f11() requires Standard_layout<void>();
void f12() requires Pod<void>();
void f13() requires Trivial<void>();
void f14() requires Polymorphic<void>();
void f15() requires Abstract<void>();
void f16() requires Final<void>();
void f17() requires Union<void>();
void f18() requires Enum<void>();


int main() { 
  f1(); // { dg-error "cannot call" }
  f2(); // { dg-error "cannot call" }
  f3(); // { dg-error "cannot call" }
  f4(); // { dg-error "cannot call" }
  f5(); // { dg-error "cannot call" }
  f6(); // { dg-error "cannot call" }
  f7(); // { dg-error "cannot call" }
  f8(); // { dg-error "cannot call" }
  f9(); // { dg-error "cannot call" }
  f10(); // { dg-error "cannot call" }
  f11(); // { dg-error "cannot call" }
  f12(); // { dg-error "cannot call" }
  f13(); // { dg-error "cannot call" }
  f14(); // { dg-error "cannot call" }
  f15(); // { dg-error "cannot call" }
  f16(); // { dg-error "cannot call" }
  f17(); // { dg-error "cannot call" }
}
