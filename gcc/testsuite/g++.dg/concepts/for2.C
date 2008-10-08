// { dg-do "run" }
#include <iterator>
#include <cassert>
#include <list>
#include <utility>

int main()
{
  std::list<int> values;
  for (int i = 1; i <= 10; ++i)
    values.push_back(i);

  std::pair<std::list<int>::iterator, std::list<int>::iterator>
    p(values.begin(), values.end());

  int sum = 0;
  for (int i : p)
    sum += i;

  assert(sum == 55);

  sum = 0;
  ++p.first;
  --p.second;
  for (int i : p)
    sum += i;
  assert(sum == 44);

  sum = 0;
  const std::pair<std::list<int>::iterator, std::list<int>::iterator> pc(p);
  for (int i : pc)
    sum += i;
  assert(sum == 44);

  return 0;
}
