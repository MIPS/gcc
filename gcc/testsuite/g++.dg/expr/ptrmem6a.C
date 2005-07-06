/* APPLE LOCAL begin mainline */
struct Z {
  int f();
};

int Z::f() { return 7; }

struct Z z;
int (Z::*m)() = &Z::f;
struct Z*p = &z;
/* APPLE LOCAL end mainline */
