// Build don't link:
// Origin: Mark Mitchell <mark@codesourcery.com>

struct S
{
  int j;
  int i[2];
};

void f ()
{
  sizeof (S::j); // ERROR - non-static data member
  sizeof (S::i[0]); //  ERROR - non-static data member
}

