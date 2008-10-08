#include <memory>

std::auto_ptr<int> foo(bool input)
{
  if (input)
    {
      std::auto_ptr<int> x(new int);
      return x;
    }
  else
    {
      std::auto_ptr<int> y(new int);
      return y;
    }
}
