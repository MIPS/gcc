// "member" interface.

// Copyright (C) 2004 Free Software Foundation, Inc.
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

#ifndef GCJX_MODEL_IMEMBER_HH
#define GCJX_MODEL_IMEMBER_HH

class IMember
{
protected:

  model_class *declaring_class;

  IMember ()
    : declaring_class (NULL)
  {
  }

  IMember (model_class *d)
    : declaring_class (d)
  {
  }

public:

  virtual ~IMember ()
  {
  }

  void set_declaring_class (model_class *dc)
  {
    assert (declaring_class == NULL);
    declaring_class = dc;
  }

  model_class *get_declaring_class () const
  {
    return declaring_class;
  }
};

#endif // GCJX_MODEL_IMEMBER_HH
