// Write a CNI header.

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

#ifndef GCJX_HEADER_CNI_HH
#define GCJX_HEADER_CNI_HH

#include "codegen.hh"
#include "directory.hh"

class cni_code_generator : public code_generator
{
protected:

  // The compiler we're using.
  compiler *comp;

  std::string cxxname (model_type *, bool = true);
  void update_modifiers (std::ostream &, modifier_t, modifier_t &);
  void add (model_type *, std::set<model_class *> &,
	    bool &, model_package *, model_package *,
	    model_package *, model_class *);
  void write_includes (std::ostream &, model_class *,
		       const AllMethodsIterator &, const AllMethodsIterator &,
		       const std::list<ref_field> &);
  void write_method (std::ostream &, model_method *, modifier_t &);
  void write_field (std::ostream &, model_field *, modifier_t &);

  void indent (std::ostream &, int);
  void open_package (std::ostream &, model_package *, model_package *, int &);
  void move_to_package (std::ostream &, model_package *, model_package *,
			int &);
  void write_namespaces (std::ostream &, const std::set<model_class *> &);

public:

  /// Construct a new CNI code generator.  Argument is the compiler
  /// being used to generate code.
  cni_code_generator (compiler *, directory_cache &);
  
  /// This is called to generate code for a class and write it.
  void generate (model_class *);	
};

#endif // GCJX_HEADER_CNI_HH
