// Build don't link:

class X{
    unsigned int i;
  public:
    void f();
};

void X::f()
{
  union {
    int j;
    int foo[sizeof (i)]; // ERROR - reference to nonstatic member
  };
}
