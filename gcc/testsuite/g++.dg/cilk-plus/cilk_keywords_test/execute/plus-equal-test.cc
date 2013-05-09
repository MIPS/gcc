/* { dg-do run } */
/* { dg-options "-fcilkplus" } */

#define NUMBER_OF_ELEMENTS 1000

#include <cstdlib>

#if HAVE_IO
#include <cstdio>
#endif

class my_class {
private:
  int value;
public:

  my_class ();
  my_class (const my_class &val);
  my_class (my_class &val);
  my_class (int val);
  ~my_class ();
  int getValue();
  my_class &operator= (my_class &new_value);
  my_class &operator+= (int val)
  {
    value += val;
    return *this;
  }
  bool operator< (const my_class &val)
  {
    return (value < val.value);
  }
};


my_class::my_class ()
{
  value = 0;
}

my_class::my_class(int val)
{
  value = val;
}

my_class::my_class (my_class &val)
{
  value = val.value;
}

my_class::my_class (const my_class &val)
{
  value = val.value;
}

my_class::~my_class ()
{
  value = -1;
}

int my_class::getValue ()
{
  return value;
}

my_class & my_class::operator= (my_class &new_value)
{
  value = new_value.value;
  return *this;
}

int operator- (my_class x, my_class y)
{
  int val_x = x.getValue ();
  int val_y = y.getValue ();
  return (val_x - val_y);
}


int main (int argc, char **argv)
{
  int n, *array_parallel, *array_serial;
  my_class length (argc == 2 ? atoi (argv[1]) : NUMBER_OF_ELEMENTS);
  
  if (argc == 2)
    n = atoi (argv[1]);
  else
    n = NUMBER_OF_ELEMENTS;
  
  array_parallel = new int[n];
  array_serial = new int[n];

  _Cilk_for (my_class ii (0); ii < length; ii += 1) {
#if HAVE_IO
    std::printf("ii.getValue() = %d\n", ii.getValue ());
#endif
    array_parallel [ii.getValue ()] = ii.getValue() * 2;
  }

  for (my_class ii (0); ii < length; ii += 1)
    array_serial [ii.getValue ()] = ii.getValue () * 2;
  
  for (int ii = 0; ii < NUMBER_OF_ELEMENTS; ii++)
    if (array_serial[ii] != array_parallel[ii]) {
#if HAVE_IO
      std::printf("array_serial[%3d] = %6d\tarray_parallel[%3d] = %6d\n", ii,
		  array_serial[ii], ii, array_parallel[ii]);
#endif
      return 1;
    }

  return 0;
}
