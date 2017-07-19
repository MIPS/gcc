/* { dg-options "-Wmissing-noreturn -fdiagnostics-show-caret -fblt" } */

struct s1
{
  void method_1 () const;
};

void s1::method_1 ()
{
}


struct s2
{
  void method_2 ();
};

void s2::method_2 () const
{
}
