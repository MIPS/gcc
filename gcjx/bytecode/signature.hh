// Signature parsing.

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

#ifndef GCJX_BYTECODE_SIGNATURE_HH
#define GCJX_BYTECODE_SIGNATURE_HH

class signature_parser
{
  // This points to our current string location.
  std::string::const_iterator it;

  // End of the string.
  std::string::const_iterator done;

  // Location.
  location where;

  void error_if (bool, const char *);
  void require (char);
  std::string find_substring (char);

  ref_forwarding_type parse_one_type_variable ();
  ref_type_variable parse_one_parameter ();
  ref_forwarding_type parse_one_type ();
  ref_forwarding_type parse_ref_type ();
  ref_forwarding_type parse_actual_type_arguments (const ref_forwarding_type &);
  void parse_type_parameters (std::list<ref_type_variable> &);

public:

  signature_parser (const std::string &s, const location &w)
    : it (s.begin ()),
      done (s.end ()),
      where (w)
  {
  }

  ~signature_parser ()
  {
  }

  ref_forwarding_type parse_type ();

  void parse_method_signature (std::list<ref_forwarding_type> &arg_types,
			       std::list<ref_type_variable> &type_vars,
			       ref_forwarding_type &return_type,
			       std::list<ref_forwarding_type> &throws);

  void parse_class_signature (std::list<ref_type_variable> &type_vars,
			      ref_forwarding_type &super,
			      std::list<ref_forwarding_type> &interfaces);
};

#endif // GCJX_BYTECODE_SIGNATURE_HH
