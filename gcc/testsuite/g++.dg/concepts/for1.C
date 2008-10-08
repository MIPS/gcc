// { dg-do "run" }
#include <iterator>
#include <cassert>

int main()
{
  int values[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  int sum = 0;

  for (int i : values)
    sum += i;

  assert(sum == 55);
}
