struct foo {
  int zoo;
  int bar [10];
  float baz;
};

#define offsetof(S,F) (& (((S *) 0)->F))

struct foo *k = (void *) malloc (offsetof (struct foo, bar[4]));
k->bar[1] = 9;
free (k);
