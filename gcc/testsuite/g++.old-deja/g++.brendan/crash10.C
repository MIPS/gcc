// Build don't link: 
// GROUPS passed old-abort
class word
{
  unsigned char b1, b2;
public:
  word (unsigned int i = 0) { b1 = i & 0xff; b2 = (i & 0xff00) >> 8; }
  operator unsigned int () { return (b2 <<  8) + b1; }
};

class just_another
{
  int foo;
  char bar[23];
};

int mumble(word w)
{
  just_another *jap;
  unsigned bar;

  bar = w;
  
  // The standard says that the expression giving the array bounds
  // must have integral type; it does not say that the expression must
  // be convertible to an integral type.  However, we believe that
  // this is an error, and that the same rules should be used as are
  // used for the value in a `switch' condition.
  jap = new just_another [w];
  
  return 0;
}  

