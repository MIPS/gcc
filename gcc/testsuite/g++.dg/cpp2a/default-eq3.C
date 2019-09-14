// { dg-do compile { target c++2a } }

struct A {
  bool operator==(const A&) const; // { dg-message "non-.constexpr" }
};

struct D
{
  A i;
  bool operator==(const D& x) const = default; // { dg-message "not usable" }
  bool operator!=(const D& z) const = default; // { dg-message "not usable" }
};

constexpr D d{A()};
static_assert (d == d);		// { dg-error "non-constant|constexpr" }
static_assert (!(d != d));	// { dg-error "non-constant|constexpr" }
