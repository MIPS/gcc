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

#ifndef GCJX_LLVM_FUNCTION_HH
#define GCJX_LLVM_FUNCTION_HH

#include "llvm/Module.h"
#include "llvm/Instructions.h"
#include <stack>
#include <iostream>

using namespace llvm;
using namespace std;

class llvm_function  
{
public:
	bool isVarArg;
	Module* module;
	Function *function;
	string name;
	vector<const Type*> parameters;
	stack<Value*> varStack;
	const Type *returnType;
	BasicBlock *currentBlock;
	BasicBlock *returnBlock;
   //allocaInsertionPoint - Place to insert alloca instructions.
   //Lazily created  and managed by CreateTemporary.
  	Instruction *allocaInsertionPoint;
	Value *lvalue;
	Value *rvalue;

  llvm_function(Module *owner);

	Value * castToType(Value *V, const Type *type);
	void  emitBlock(BasicBlock *BB);
	AllocaInst * createTemporary(const Type *type);
	void initParameters( int size );
	void addParameter(const Type *type,string name );
	void setName(string name);
	void setIsVarArg(bool b);
	void setReturnType(const Type* type);
	void startBlock();
	Value *addLocalVariable (const Type * type,Value *size, const string &name );
	void push(Value *var){
		std::cout << "push\n";
		varStack.push(var);
	}
	Value *pop(){
		std::cout << "pop\n";
		Value *var = varStack.top();
		varStack.pop();
		return var;
	}

	bool empty() {
		return varStack.empty();
	}
	void emitDeclaration();
	void emitFunction();
	void emitReturnValue();
	void llvm_function::emitCall(llvm_function * function);

};

#endif // GCJX_LLVM_GEN_HH
