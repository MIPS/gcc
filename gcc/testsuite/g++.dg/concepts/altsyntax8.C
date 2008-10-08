#include <concepts> 
#include <iterator>

auto concept Container<typename X> {
  std::InputIterator iterator = X::iterator;
  iterator X::begin(); iterator X::end();
}
