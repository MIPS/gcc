struct foo {
  int bar [10];
};

struct foo *k = (void *) malloc (2 * sizeof(int));
k->bar[5] = 9;
free (k);
