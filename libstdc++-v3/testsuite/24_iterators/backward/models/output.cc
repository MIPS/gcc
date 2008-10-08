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
  struct random_input_iterator
  {
    int operator*() const { return std::rand(); }
    int* operator->() const;
    
    random_input_iterator& operator++() { return *this; }
    random_input_iterator operator++(int) { return *this; }

    friend bool 
    operator==(const random_input_iterator&, const random_input_iterator&)
    {
      return false;
    }

    friend bool 
    operator!=(const random_input_iterator&, const random_input_iterator&)
    {
      return true;
    }
  };
}

namespace std {
  template<>
  struct iterator_traits<N::random_input_iterator>
  {
    typedef std::input_iterator_tag iterator_category;
    typedef int value_type;
    typedef int difference_type;
    typedef int* pointer;
    typedef int reference;
  };
}

template<typename Iter> requires std::InputIterator<Iter>
void check(Iter, Iter) {}

void f()
{
  N::random_input_iterator first, last;
  check(first, last);
}

int main()
{
  return 0;
}
