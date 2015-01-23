// { dg-do compile }
// { dg-options "-std=c++1z" }

// Check that empty expansions and required failures.

#define COMMA ,

#define MAKE_FN(name, op) \
  template<typename... Ts> \
    constexpr auto name (Ts... ts) { return (... op ts); } // { dg-error "empty" }

MAKE_FN (add, +);
MAKE_FN (sub, -);
MAKE_FN (mul, *);
MAKE_FN (div, /);
MAKE_FN (mod, %);
MAKE_FN (bxor, ^);
MAKE_FN (bor, |);
MAKE_FN (band, &);
MAKE_FN (lsh, <<);
MAKE_FN (rsh, >>);

MAKE_FN (assign, =);
MAKE_FN (addi, +=);
MAKE_FN (subi, -=);
MAKE_FN (muli, *=);
MAKE_FN (divi, /=);
MAKE_FN (modi, %=);
MAKE_FN (bxori, ^=);
MAKE_FN (bori, |=);
MAKE_FN (bandi, &=);
MAKE_FN (lshi, <<=);
MAKE_FN (rshi, >>=);

MAKE_FN (eq, ==);
MAKE_FN (ne, !=);
MAKE_FN (lt, <);
MAKE_FN (gt, >);
MAKE_FN (le, <);
MAKE_FN (ge, >);

MAKE_FN (land, &&);
MAKE_FN (lor, ||);

MAKE_FN (comma, COMMA);
MAKE_FN (dot_star, .*);
MAKE_FN (arrow_star, ->*);

int main() { 
  static_assert(add() == int(), "");
  static_assert(mul() == 1, "");
  static_assert(bxor() == int(), "");
  static_assert(bor() == int(), "");
  static_assert(band() == -1, "");
  static_assert(land() == true, "");
  static_assert(lor() == false, "");
  comma(); // No value to theck

  // These are all errors, but the error is emitted at the point
  // of instantiation (line 10).
  sub();
  div();
  mod();
  lsh();
  rsh();
  assign();
  addi();
  subi();
  muli();
  divi();
  modi();
  bxori();
  bori();
  bandi();
  lshi();
  rshi();
  eq();
  ne();
  lt();
  gt();
  le();
  ge();
  dot_star();
  arrow_star();
}
