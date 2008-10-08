// { dg-do "run" }
#include <iterator>
#include <cassert>
#include <list>
#include <vector>
#include <deque>
#include <map>
#include <set>

int main()
{
  std::list<int> values;
  for (int i = 1; i <= 10; ++i)
    values.push_back(i);

  int sum = 0;
  for (int i : values)
    sum += i;
  assert(sum == 55);

  const std::list<int>& values_list_c = values;
  sum = 0;
  for (int i : values_list_c)
    sum += i;
  assert(sum == 55);

  std::vector<int> values_vec(values.begin(), values.end());
  sum = 0;
  for (int i : values_vec)
    sum += i;
  assert(sum == 55);


  const std::vector<int>& values_vec_c = values_vec;
  sum = 0;
  for (int i : values_vec_c)
    sum += i;
  assert(sum == 55);

  std::deque<int> values_deq(values.begin(), values.end());
  sum = 0;
  for (int i : values_deq)
    sum += i;
  assert(sum == 55);

  const std::deque<int>& values_deq_c = values_deq;
  sum = 0;
  for (int i : values_deq_c)
    sum += i;
  assert(sum == 55);

  std::map<int, int> values_map;
  for (int i = 1; i <= 10; ++i)
    values_map[i] = i;
  sum = 0;
  for (std::pair<const int, int>& p : values_map)
    sum += p.second;
  assert(sum == 55);

  const std::map<int, int>& values_map_c = values_map;
  sum = 0;
  for (const std::pair<const int, int>& p : values_map_c)
    sum += p.second;
  assert(sum == 55);
  
  std::multimap<int, int> values_multimap(values_map.begin(), values_map.end());
  sum = 0;
  for (const std::pair<const int, int>& p : values_multimap)
    sum += p.second;
  assert(sum == 55);

  const std::multimap<int, int>& values_multimap_c = values_multimap;
  sum = 0;
  for (const std::pair<const int, int>& p : values_multimap_c)
    sum += p.second;
  assert(sum == 55);
  
  std::set<int> values_set(values.begin(), values.end());
  sum = 0;
  for (int i : values_set)
    sum += i;
  assert(sum == 55);

  const std::set<int>& values_set_c = values_set;
  sum = 0;
  for (int i : values_set_c)
    sum += i;
  assert(sum == 55);

  std::multiset<int> values_multiset(values.begin(), values.end());
  sum = 0;
  for (int i : values_multiset)
    sum += i;
  assert(sum == 55);

  const std::multiset<int>& values_multiset_c = values_multiset;
  sum = 0;
  for (int i : values_multiset_c)
    sum += i;
  assert(sum == 55);

  return 0;
}
