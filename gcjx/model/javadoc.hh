// Represent a javadoc comment.

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

#ifndef GCJX_MODEL_JAVADOC_HH
#define GCJX_MODEL_JAVADOC_HH

/// Represents a javadoc comment.  The only state this holds is
/// whether or not '@deprecated' was seen.  Note that javadoc comment
/// objects do not live past parsing -- they are created by the lexer
/// but the parser simply marks their referent as deprecated, when
/// appropriate.
class model_javadoc : public model_element
{
  bool deprecated;

public:

  model_javadoc (const location &w)
    : model_element (w),
      deprecated (false)
  {
  }

  void set_deprecated ()
  {
    deprecated = true;
  }

  bool deprecated_p () const
  {
    return deprecated;
  }

  void visit (visitor *);
};

#endif // GCJX_MODEL_JAVADOC_HH
