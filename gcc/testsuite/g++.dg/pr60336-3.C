// { dg-do compile }
// { dg-options "-O2" }

struct dummy { struct{}__attribute__((aligned (4))) a[7]; };

extern void test1 (struct dummy, ...);
extern void (*test2) (struct dummy, ...);

void
foo ()
{
  struct dummy a0;
  test1 (a0); // { dg-message "note: the ABI of passing empty record has changed in GCC 6" }
  test2 (a0); // { dg-message "note: the ABI of passing empty record has changed in GCC 6" }
}
