struct a {
  int x;
  int y;
  char z;
};

struct a k;
struct a *p;

p = &k;

p->z = 'q';

