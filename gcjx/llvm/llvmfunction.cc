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

#include "llvmfunction.hh"

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



/// CastToType - Cast the specified value to the specified type if it is
/// not already that type.
Value *llvm_function::castToType(Value *V, const Type *Ty) {
		if (V->getType() == Ty) return V;
		if (Constant *C = dyn_cast<Constant>(V))
				return ConstantExpr::getCast(C, Ty);

		// Handle cast (cast bool X to T2) to bool as X, because this occurs all over
		// the place.
		if (CastInst *CI = dyn_cast<CastInst>(V))
				if (Ty == Type::BoolTy && CI->getOperand(0)->getType() == Type::BoolTy)
						return CI->getOperand(0);
		return new CastInst(V, Ty, V->getName(), currentBlock);
}


/// emitBlock - Add the specified basic block to the end of the function.  If
/// the previous block falls through into it, add an explicit branch.  Also,
/// manage fixups for EH info.
void llvm_function::emitBlock(BasicBlock *BB) {
		// If the previous block falls through to BB, add an explicit branch.
		if ( currentBlock->getTerminator() == 0) {
				// If the previous block has no label and is empty, remove it: it is a
				// post-terminator block.
				if (currentBlock->getName().empty() && currentBlock->begin() == currentBlock->end())
						currentBlock->eraseFromParent();
				else {
						// Otherwise, fall through to this block.
						new BranchInst(BB, currentBlock);
				}
		}

		// Add this block.
		function->getBasicBlockList().push_back(BB);
		currentBlock = BB;  // It is now the current block.
}

/// CreateTemporary - Create a new alloca instruction of the specified type,
/// inserting it into the entry block and returning it.  The resulting
/// instruction's type is a pointer to the specified type.
AllocaInst *llvm_function::createTemporary(const Type *type) {
  if (allocaInsertionPoint == 0) {
    // Create a dummy instruction in the entry block as a marker to insert new
    // alloc instructions before.  It doesn't matter what this instruction is,
    // it is dead.  This allows us to insert allocas in order without having to
    // scan for an insertion point.
    allocaInsertionPoint = new CastInst(Constant::getNullValue(Type::IntTy),
                                        Type::IntTy, "alloca point");
    // Insert it as the first instruction in the entry block.
    function->begin()->getInstList().insert(function->begin()->begin(),
                                      allocaInsertionPoint);
  }
  return new AllocaInst(type, 0, "memtmp", allocaInsertionPoint);
}




llvm_function::llvm_function ( Module *owner)
:isVarArg(0),
 module(owner),
 function(0),
 name(""),
 parameters(0),
 varStack(),
 returnType(0),
 currentBlock(0),
 returnBlock(0)
{
}

void llvm_function::emitDeclaration() {
	FunctionType *ftype = FunctionType::get(returnType,
		parameters,isVarArg); 
	function = new Function(ftype,Function::ExternalLinkage,
						name,module);

	 currentBlock = NULL;
	//create the return block but add it later
	//we will branch ahead to here on returns
	returnBlock = new BasicBlock("return");
}

void llvm_function::emitFunction() {
		emitBlock(returnBlock);
		//If the function returns a value, get it into a register and return it now.
		if (returnType != Type::VoidTy) {
				//Value *returnValue = new LoadInst(DECL_LLVM(DECL_RESULT(FnDecl)), "retval", currentBlock);
				//returnValue = castToType(RV, function->getReturnType());
				//new ReturnInst(RV, currentBlock);
				new ReturnInst( castToType(pop(),function->getReturnType()),currentBlock);
		} else {
				// Otherwise, just return.
				new ReturnInst(currentBlock);
		}
}

void llvm_function::emitReturnValue(){
		new BranchInst(returnBlock,currentBlock);
}

void llvm_function::emitCall(llvm_function * callee){
	vector<Value*> args;
	args.reserve(callee->parameters.size());
	for( unsigned int i = 0,e = callee->parameters.size(); i < e ;  i++ )
 	{ 
		args.push_back(pop());
	}
	CallInst *call=new CallInst(callee->function,args,callee->name,currentBlock);
	if (callee->returnType != Type::VoidTy) {
		push(call);
	}
}

void llvm_function::setIsVarArg(bool b) 
{
	isVarArg = b;
}

void llvm_function::setReturnType (const Type *type )
{
	returnType = type;
}

void llvm_function::setName (string inName )
{
	name = inName;
}

void llvm_function::initParameters (int size )
{
	parameters.reserve(size);
}

void llvm_function::addParameter (const Type * type,const string name)
{
	parameters.push_back(type);
}


Value * llvm_function::addLocalVariable (const Type * type,Value *size, 
	const string &name )
{
  if (allocaInsertionPoint == 0) {
    // Create a dummy instruction in the entry block as a marker to insert new
    // alloc instructions before.  It doesn't matter what this instruction is,
    // it is dead.  This allows us to insert allocas in order without having to
    // scan for an insertion point.
    allocaInsertionPoint = new CastInst(Constant::getNullValue(Type::IntTy),
                                        Type::IntTy, "alloca point");
    // Insert it as the first instruction in the entry block.
    function->begin()->getInstList().insert(function->begin()->begin(),
                                      allocaInsertionPoint);
   }
	
	lvalue=new AllocaInst(type,size,name,allocaInsertionPoint);
	return lvalue;
}

void llvm_function::startBlock()
{
	if( currentBlock == NULL ) {
		currentBlock = new BasicBlock("EntryBlock",function);
	}else {
		emitBlock(new BasicBlock(""));
	}
}

