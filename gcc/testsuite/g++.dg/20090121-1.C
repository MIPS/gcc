// { dg-do compile }
// { dg-options "-fwhopr -Wuninitialized -O2" }
class A
{
private:
  int y;

public:
  A () { int x; y = x + 1; } /* { dg-warning "'x' is used uninitialized in this function" }  */
  int get_y () { return y; }
};

int foo()
{
  A a;
  return a.get_y ();
}

