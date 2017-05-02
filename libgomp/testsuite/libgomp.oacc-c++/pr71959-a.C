// { dg-do compile }

struct Iter 
{
  int *cursor;

  void ctor (int *cursor_) asm("_ZN4IterC1EPi");
  int *point () const asm("_ZNK4Iter5pointEv");
};

#pragma acc routine seq
void  Iter::ctor (int *cursor_)
{
  cursor = cursor_;
}

#pragma acc routine seq
int *Iter::point () const
{
  return cursor;
}

void apply (int (*fn)(), Iter out) asm ("_ZN5Apply5applyEPFivE4Iter");

#pragma acc routine seq
void apply (int (*fn)(), struct Iter out)
{ *out.point() = fn (); }

extern "C" void __gxx_personality_v0 ()
{
}
