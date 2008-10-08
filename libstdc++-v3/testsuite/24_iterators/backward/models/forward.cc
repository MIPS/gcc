// 2005-08-21  Douglas Gregor <dgregor@osl.iu.edu>

// Copyright (C) 2005  Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 2, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING.  If not, write to the Free
// Software Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
// USA.

// Backward-compatibility models

#include <iterator>
#include <cstdlib>

namespace N {
  class counting_iterator
  {
  public:
    counting_iterator() : value(0) { }
    explicit counting_iterator(int value) : value(value) { }

    const int& operator*() const { return value; }
    const int* operator->() const { return &value; }
    
    counting_iterator& operator++() { ++value; return *this; }
    counting_iterator operator++(int) { return counting_iterator(value++); }

    friend bool 
    operator==(const counting_iterator& x, const counting_iterator& y)
    {
      return x.value == y.value;
    }

    friend bool 
    operator!=(const counting_iterator& x, const counting_iterator& y)
    {
      return x.value != y.value;
    }

  private:
    int value;
  };
}

namespace std {
  template<>
  struct iterator_traits<N::counting_iterator>
  {
    typedef std::forward_iterator_tag iterator_category;
    typedef int value_type;
    typedef int difference_type;
    typedef const int* pointer;
    typedef const int& reference;
  };
}

template<typename Iter> requires std::ForwardIterator<Iter>
void check(Iter, Iter) {}

void f()
{
  N::counting_iterator first(0), last(17);
  check(first, last);
}

int main()
{
  return 0;
}
