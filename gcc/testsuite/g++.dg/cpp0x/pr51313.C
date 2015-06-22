// PR c++/51313
// { dg-do compile { target c++11 } }

class ostream;

extern "C" {
  extern int isdigit (int);
}

ostream&
operator<<(ostream&, const unsigned char*);
ostream&
operator<<(ostream&, const int);

extern ostream cout;

int main()
{
  cout << isdigit(0);
}
