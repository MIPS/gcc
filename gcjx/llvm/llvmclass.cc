// Code generator for llvm.

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

#include "llvmclass.hh"

#include "llvm/SymbolTable.h"
#include "llvm/Module.h"
#include "llvm/DerivedTypes.h"
#include "llvm/Instructions.h"
#include "llvm/Constants.h"
#include "llvm/Instructions.h"
#include "llvm/Bytecode/Writer.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/DepthFirstIterator.h"

/** Generate C code**/
#include "llvm/Bytecode/Reader.h"
#include "llvm/Target/SubtargetFeature.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetMachineRegistry.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Module.h"
#include "llvm/PassManager.h"
#include "llvm/Pass.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/PluginLoader.h"
#include "llvm/Support/PassNameParser.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/System/Signals.h"
#include "llvm/Config/config.h"
/** End gnerate c code **/



#include <list>
#include <utility>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>


using namespace std;
using namespace llvm;


llvm_class::llvm_class( Module *owner){
	this->owner = owner;
}

void
llvm_class::addField(const Type *type,string name )
{
	fields.push_back(type);
}

void
llvm_class::emitStruct()
{
  classStruct = StructType::get(fields);
}
