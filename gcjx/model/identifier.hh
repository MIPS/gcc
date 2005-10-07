// An identifier.

// Copyright (C) 2004, 2005 Free Software Foundation, Inc.
//
// This file is part of GCC.
//
// gcjx is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// gcjx is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with gcjx; see the file COPYING.LIB.  If
// not, write to the Free Software Foundation, Inc.,
// 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

#ifndef GCJX_MODEL_IDENTIFIER_HH
#define GCJX_MODEL_IDENTIFIER_HH

// This is just a small wrapper around a string.
class model_identifier : public model_element
{
  // The identifier.
  std::string identifier;

public:

  model_identifier (const location &w, const std::string &id)
    : model_element (w),
      identifier (id)
  {
  }

  std::string get_identifier () const
  {
    return identifier;
  }

  void visit (visitor *);
};

#endif // GCJX_MODEL_IDENTIFIER_HH
