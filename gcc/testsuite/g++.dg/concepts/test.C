// { dg-options "-I/home/faculty/asutton/Code/origin -std=c++1z" }

#include <algorithm>

// #include <origin/range/stream.hpp>

// int main() {
//   std::string s = "1 2 3 4 5";
//   std::istringstream ss(s);
//   auto is = origin::make_istream<int>(ss);

//   for(int x : is) {
//      std::cout << x << '\n';
//   }

//   static_assert(not origin::Range<decltype(is)>(), "");
// }
