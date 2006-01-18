// Code generator for bytecode.

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

#ifndef GCJX_LLVM_CLASS_HH
#define GCJX_LLVM_CLASS_HH

#include "llvm/Module.h"
#include "llvm/Instructions.h"
#include "llvm/DerivedTypes.h"
#include <stack>
#include <iostream>

using namespace llvm;
using namespace std;

class llvm_class  
{
public:
  Module *owner;
  StructType *classStruct;
  vector<const Type*> fields;
  llvm_class( Module *owner);
  void addField(const Type *type,string name );
  void emitStruct();
};

#endif // GCJX_LLVM_CLASS_HH
