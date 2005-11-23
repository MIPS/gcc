// { dg-do compile { target i?86-*-* } }
// { dg-options -O }

typedef unsigned int u4;
typedef unsigned long long u8;
typedef u8 (*u8tou8)(u8);

struct C {
  static inline u8 a(u4 x, u4 y);
  static inline u8 b(unsigned char *p) { return c(*(u8 *)p); }
  static inline u8 c(u8 x) {	// { dg-warning "too few arguments" "too few" }
    return ((u8tou8)a)(x);
  }
};

inline u8 C::a(u4 x, u4 y) {
  return x + y;
}

u8 n = 0x123456789abcdef;

struct B {
  unsigned char *e;
  B() { e = (unsigned char *) &n; }
  u8 f() {
    return C::b(e);
  }
};

struct A {
  B *g;
  void foo ();
};

void A::foo ()
{
  g->f();
}
