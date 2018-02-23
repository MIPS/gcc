// { dg-do compile { target c++11 } }
// { dg-require-cstdint "" }
// { dg-require-gthreads "" }

// Copyright (C) 2018 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING3.  If not see
// <http://www.gnu.org/licenses/>.

#include <future>

// PR libstdc++/84532

template<typename T, typename U>
  using is_wrapper_for = std::is_same<T, std::reference_wrapper<U>>;

auto f = [](auto x, auto y, int&) {
  static_assert(is_wrapper_for<decltype(x), int>::value, "");
  static_assert(is_wrapper_for<decltype(y), const int>::value, "");
};
int i = 0;
auto fut = std::async(f, std::ref(i), std::cref(i), std::ref(i));
