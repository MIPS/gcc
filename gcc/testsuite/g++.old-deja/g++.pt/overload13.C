// Build don't link:

struct A {
  template <class T> int f (T) { return 0; }
  int f (int) { return 1; }
};

int main ()
{
  A a;
  // The standard does not say that the `template' keyword causes
  // overload resolution to ignore non-templates, although that might
  // be sensible.
  return a.template f (0); // gets bogus error XFAIL *-*-*
}
