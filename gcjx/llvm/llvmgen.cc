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

#include "typedefs.hh"
#include "llvmgen.hh"
#include "dump.hh"


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
//#include "llvm/Config/config.h"
/** End gnerate c code **/



#include <list>
#include <utility>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>


using namespace std;
using namespace llvm;

// GetFileNameRoot - Helper function to get the basename of a filename.
static inline string
GetFileNameRoot(const string &InputFilename) {
	string IFN = InputFilename;
	string outputFilename;
	int Len = IFN.length();
	if ((Len > 2) &&
		IFN[Len-3] == '.' && IFN[Len-2] == 'b' && IFN[Len-1] == 'c') {
				outputFilename = string(IFN.begin(), IFN.end()-3); // s/.bc/.s/
		} else {
				outputFilename = IFN;
		}
		return outputFilename;
}

const Type * llvm_code_generator::convertType( model_type *type )
{
		if (typemap.find (type) == typemap.end ())
		{
				const Type *Ty;
				assert (type->reference_p ());
				model_class *klass = assert_cast<model_class *> (type);
				Ty = OpaqueType::get();
				module->addTypeName(klass->get_fully_qualified_name (),Ty);
				//typemap[klass] = TypeDB.setType(type,PointerType::get(Ty));
				typemap[klass] = PointerType::get(Ty);
				return typemap[klass];
		}
		return typemap[type];
}

/// CastToType - Cast the specified value to the specified type if it is
/// not already that type.
Value *llvm_code_generator::castToType(Value *V, const Type *Ty) {
#if 0
		if (V->getType() == Ty) return V;
		if (Constant *C = dyn_cast<Constant>(V))
				return ConstantExpr::getCast(C, Ty);

		// Handle cast (cast bool X to T2) to bool as X, because this occurs all over
		// the place.
		if (CastInst *CI = dyn_cast<CastInst>(V))
				if (Ty == Type::BoolTy && CI->getOperand(0)->getType() == Type::BoolTy)
						return CI->getOperand(0);
		return new CastInst(V, Ty, V->getName(), currentBlock);
#endif
	return NULL;
}


llvm_code_generator::llvm_code_generator (compiler *comp,
				directory_cache &dirs)
: code_generator (dirs)
{
		// Set up all primitive types.
		typemap[primitive_byte_type] = Type::SByteTy;
		typemap[primitive_short_type] = Type::ShortTy;
		typemap[primitive_int_type] = Type::IntTy;
		typemap[primitive_long_type] = Type::LongTy;
		typemap[primitive_float_type] = Type::FloatTy;
		typemap[primitive_double_type] = Type::DoubleTy;
		typemap[primitive_boolean_type] = Type::BoolTy;
		typemap[primitive_char_type] = Type::UShortTy;
		typemap[primitive_void_type] = Type::VoidTy;
		typemap[null_type] = PointerType::get(OpaqueType::get());

}

		void
llvm_code_generator::generate (model_class *klass)
{
		cout << "Class " << klass->get_name() << "\n";
		string FeaturesStr;
		string OutputFilename;
		ostream *Out = 0;
		const TargetMachineRegistry::Entry * target = TargetMachineRegistry::getList();

		module = new Module(klass->get_name()); 
		OutputFilename=GetFileNameRoot(klass->get_name())+".cgen";
		//while( target != NULL ) {
		cout << "Target=" << target->Name << "\n";
		//target = target->getNext();
		//}
		auto_ptr<TargetMachine> targetArch(target->CtorFn(*module, 0, FeaturesStr));
		assert(targetArch.get() && "Could not allocate target machine!");
		TargetMachine &Target = *targetArch.get();
		const TargetData &TD = Target.getTargetData();
		// Build up all of the passes that we want to do to the module...
		PassManager Passes;
		Passes.add(new TargetData(TD));
		Out = new ofstream(OutputFilename.c_str());
		if (!Out->good()) {
				cerr << ": error opening " << OutputFilename << "!\n";
				delete Out;
				return ;
		}
		sys::RemoveFileOnSignal(sys::Path(OutputFilename));
		Target.addPassesToEmitFile(Passes,*Out,TargetMachine::AssemblyFile, true);


		//visit the class
		klass->visit(this);
		

		// Output the bytecode file to stdout
		//WriteBytecodeToFile(module, cout);
		Passes.run(*module);
		// Delete the ostream if it's not a stdout stream
		if (Out != &cout) delete Out;
		delete module;
		//dump_tree(klass);
}

llvm_code_generator::~llvm_code_generator ()
{
		//XXX dunno about this is it enough ?
		delete currentMethod;
		delete this;
}

		void
llvm_code_generator::generate( model_method *m)
{
		m->visit (this);
}

llvm_function * 
llvm_code_generator::get_method (model_method * method ) 
{
	
	llvm_function * function;
	if (methodMap.find (method) == methodMap.end ()) {
		function = new llvm_function(module);
		const list<ref_variable_decl> & params = method->get_parameters ();
cout << "Method " << method->get_name() << " " << method->get_pretty_name() << "\n";
		function->setName(method->get_name());
		function->initParameters(params.size());
		for (list<ref_variable_decl>::const_iterator i = params.begin ();
						i != params.end (); ++i) 
		{
				model_type *type = (*i)->type ();
				const Type *Ty =convertType(type);
				function->addParameter(Ty,(*i)->get_name());
		}
		function->setReturnType(convertType(method->get_return_type()));
		//finished with the declaration 
		function->emitDeclaration();
		methodMap[method]=function;
		return function;
	} 
	function = methodMap[method];
	assert(function != 0 );
	return function;
}

void 
llvm_code_generator::visit_class (model_class *klass, const std::string &descr,
                    const std::string &name)
{
cout << " visit_class callled --------- " << descr << "++" << name << "\n";
	currentClass = new llvm_class(module);
  	list<ref_field> fields = klass->get_fields ();
  	for (list<ref_field>::const_iterator i = fields.begin ();
       i != fields.end (); ++i)
    {
		(*i).get ()->visit(this);
	}
	currentClass->emitStruct();


	// First we generate code for the methods.  Then we write the bytes
	// for the fields and the methods in a separate step.  We do this
	// because code generation might require adding a new field.
	list<ref_method> methods = klass->get_methods ();
	for (list<ref_method>::const_iterator i = methods.begin ();
					i != methods.end (); ++i)
	{
				generate ((*i).get ());
	}


}

void
llvm_code_generator::visit_field (model_field *field)
{
cout << " VISIT visit_field " << field->get_name() << "\n";
		model_type *type = field->type ();
		const Type *llvmType =convertType(type);
		currentClass->addField(llvmType,field->get_name());
#if 0
		Value *size = 0;
cout << "----> ADDING LOCAL VARIABLE " << name << "\n";
		Value *location =currentMethod->addLocalVariable(Ty,size,name);
		localmap[decl] = location;
      	if (init){
      		init->visit (this);
			new StoreInst(currentMethod->pop(),location,currentMethod->currentBlock);
		}
#endif
}

void 
llvm_code_generator::visit_method (model_method * method,
				const list<ref_variable_decl> & params,
				const ref_block &block)
{
	 currentMethod = get_method(method);
	 list<ref_variable_decl>::const_iterator i = params.begin ();
		for (Function::arg_iterator arg = currentMethod->function->arg_begin ();
						arg != currentMethod->function->arg_end(); arg++,i++) 
		{
			model_variable_decl *decl = (*i).get();
			localmap[decl]=arg;
		}
		//start walking
		if( block)
		block->visit(this);	
		//finish the function
		currentMethod->emitFunction();
}

		void
llvm_code_generator::visit_assert (model_assert *stmt, 
				const ref_expression &first,
				const ref_expression &second)
{
cout << "VISIT  model_assert  \n";
	if(first) 
		first->visit(this);
	if(second) 
		second->visit(this);
}

		void
llvm_code_generator::visit_block (model_block *block,
				const list<ref_stmt> &statements)
{
	currentMethod->startBlock();
	::visit(this,statements);
}

		void
llvm_code_generator::visit_bytecode_block (model_bytecode_block *block,
				int, int, int, const uint8 *)
{
}

		void
llvm_code_generator::visit_break (model_break *brk, const ref_stmt &target)
{
cout << "VISIT  break  \n";
}

		void
llvm_code_generator::visit_catch (model_catch *stmt, 
				const ref_variable_decl &var,
				const ref_block &body)
{
cout << "VISIT  catch  \n";
}

void llvm_code_generator::visit_continue (model_continue *stmt,
				const ref_stmt &target)
{
cout << "VISIT  continue  \n";
}

		void
llvm_code_generator::visit_class_decl_stmt (model_class_decl_stmt *stmt,
				const ref_class &klass)
{
cout << "VISIT ---- class  decl_stmt  \n";
}

void llvm_code_generator::visit_do (model_do *stmt, const ref_expression &cond,
				const ref_stmt &body)
{
	if(cond) 
		cond->visit(this);
cout << "VISIT  do  \n";
}

void llvm_code_generator::visit_empty (model_empty *)
{
cout << "VISIT  empty  \n";
}

void llvm_code_generator::visit_expression_stmt (model_expression_stmt *,
				const ref_expression &expr)
{
	if( expr)
		expr->visit(this);
cout << " visit_expression_stmt \n";
}

void llvm_code_generator::visit_for_enhanced (model_for_enhanced *,
				const ref_stmt &, const ref_expression & expr,
				const ref_variable_decl &)
{
cout << "VISIT  visit_for_enhanced  \n";
	if( expr)
		expr->visit(this);
}

void llvm_code_generator::visit_for (model_for *, const ref_stmt &,
				const ref_expression &expr, const ref_stmt &,
				const ref_stmt &)
{
	if( expr)
		expr->visit(this);
cout << "VISIT  visit_for\n";
}

void llvm_code_generator::visit_if (model_if *, const ref_expression &cond,
				const ref_stmt &true_branch, const ref_stmt &false_branch)
{
	if( cond)
		cond->visit(this);
cout << "VISIT  visit_if\n";
}

void llvm_code_generator::visit_label (model_label *, const ref_stmt &)
{
cout << "VISIT  visit_label\n";
}

void llvm_code_generator::visit_return (model_return *, const ref_expression &expr)
{
	if(expr) 
		expr->visit(this);
cout << "VISIT  visit_return\n";
	currentMethod->emitReturnValue();
}

void llvm_code_generator::visit_switch (model_switch *,
				const ref_expression & expr,
				const list<ref_switch_block> &)
{
	if(expr) 
		expr->visit(this);
cout << "VISIT  visit_switch\n";
}

void llvm_code_generator::visit_switch_block (model_switch_block *,
				const list<ref_stmt> &)
{
cout << "VISIT  visit_switch_block\n";
}

void llvm_code_generator::visit_synchronized (model_synchronized *,
				const ref_expression & expr,
				const ref_stmt &)
{
	if(expr) 
		expr->visit(this);
cout << "VISIT  visit_synchronized\n";
}

void llvm_code_generator::visit_throw (model_throw *, const ref_expression & expr)
{
	if(expr) 
		expr->visit(this);
cout << "VISIT  visit_throw\n";
}

void llvm_code_generator::visit_try (model_try *, const ref_block &,
				const list<ref_catch> &, const ref_block &)
{
cout << "VISIT  visit_try\n";
}

void llvm_code_generator::visit_variable_stmt (model_variable_stmt *,
				const list<ref_variable_decl> & decls)
{
	for (list<ref_variable_decl>::const_iterator i = decls.begin ();
				i != decls.end (); ++i) 
	{
		(*i)->visit(this);
	}		
}
void llvm_code_generator::visit_variable_decl (model_variable_decl *decl,
                            const std::string &name,
                            const ref_forwarding_type &fortype,
                            const ref_expression &init,
                            bool isfinal,
                            bool isused)
{
		Value *size = 0;
		model_type *type = decl->type ();
		const Type *Ty =convertType(type);
cout << "----> ADDING LOCAL VARIABLE " << name << "\n";
		Value *location =currentMethod->addLocalVariable(Ty,size,name);
		localmap[decl] = location;
      	if (init){
      		init->visit (this);
			new StoreInst(currentMethod->pop(),location,currentMethod->currentBlock);
		}
}

void llvm_code_generator::visit_while (model_while *, const ref_expression &cond,
				const ref_stmt &body)
{
	if(cond)
		cond->visit(this);
	body->visit(this);
	
cout << "VISIT  visit_while\n";
}

void llvm_code_generator::visit_array_initializer (model_array_initializer *,
				const ref_forwarding_type &,
				const list<ref_expression> &)
{
cout << "VISIT  visit_array_initializer \n";
}

void llvm_code_generator::visit_array_ref (model_array_ref *,
				const ref_expression &,
				const ref_expression &)
{
cout << "VISIT  visit_array_ref \n";
}

void llvm_code_generator::visit_assignment (model_assignment *,
				const ref_expression &,
				const ref_expression &)
{
cout << "VISIT VARIALBEL ASSIGNMENT visit_assignment \n";
}

void llvm_code_generator::visit_op_assignment (model_minus_equal *,
				const ref_expression &,
				const ref_expression &)
{
cout << "VISIT OP model_minus_equal \n";
}

void llvm_code_generator::visit_op_assignment (model_mult_equal *,
				const ref_expression &,
				const ref_expression &)
{
cout << "VISIT OP model_mult_equal \n";
}

void llvm_code_generator::visit_op_assignment (model_div_equal *,
				const ref_expression &,
				const ref_expression &)
{
}

void llvm_code_generator::visit_op_assignment (model_and_equal *,
				const ref_expression &,
				const ref_expression &)
{
cout << "VISIT  model_and_equal \n";
}

void llvm_code_generator::visit_op_assignment (model_or_equal *,
				const ref_expression &,
				const ref_expression &)
{
cout << "VISIT  model_or_equal \n";
}

void llvm_code_generator::visit_op_assignment (model_plus_equal *,
				const ref_expression &,
				const ref_expression &)
{
cout << "VISIT  model_plus_equal \n";
}

void llvm_code_generator::visit_op_assignment (model_xor_equal *,
				const ref_expression &,
				const ref_expression &)
{
cout << "VISIT  model_xor_equal \n";
}

void llvm_code_generator::visit_op_assignment (model_mod_equal *,
				const ref_expression &,
				const ref_expression &)
{
cout << "VISIT OP model_mod_equal \n";
}

void llvm_code_generator::visit_op_assignment (model_ls_equal *,
				const ref_expression &,
				const ref_expression &)
{
cout << "VISIT OP visit_op_assignment \n";
}

void llvm_code_generator::visit_op_assignment (model_rs_equal *,
				const ref_expression &,
				const ref_expression &)
{
cout << "VISIT OP model_rs_equal \n";
}

void llvm_code_generator::visit_op_assignment (model_urs_equal *,
				const ref_expression &,
				const ref_expression &)
{
cout << "VISIT OP model_urs_equal \n";
}

void llvm_code_generator::visit_arith_binary (model_minus *,
				const ref_expression &,
				const ref_expression &)
{
cout << "VISIT  model_minus \n";
}

void llvm_code_generator::visit_arith_binary (model_mult *,
				const ref_expression &,
				const ref_expression &)
{
cout << "VISIT  model_mult \n";
}

void llvm_code_generator::visit_arith_binary (model_div *,
				const ref_expression &,
				const ref_expression &)
{
cout << "VISIT  model_div \n";
}

void llvm_code_generator::visit_arith_binary (model_mod *,
				const ref_expression &,
				const ref_expression &)
{
cout << "VISIT  model_mod \n";
}

void llvm_code_generator::visit_arith_binary (model_and *,
				const ref_expression &,
				const ref_expression &)
{
cout << "VISIT  model_and \n";
}

void llvm_code_generator::visit_arith_binary (model_or *,
				const ref_expression &,
				const ref_expression &)
{
cout << "VISIT  model_or \n";
}

void llvm_code_generator::visit_arith_binary (model_xor *,
				const ref_expression &,
				const ref_expression &)
{
cout << "VISIT  model_xor \n";
}

		void
llvm_code_generator::visit_arith_binary (model_plus *,
				const ref_expression &lhs,
				const ref_expression &rhs)
{
	lhs->visit(this);
	rhs->visit(this);
 	// Create the add instruction... does not insert...
  	Instruction *add =BinaryOperator::create(Instruction::Add,
		currentMethod->pop(),currentMethod->pop(),"addresult",
		currentMethod->currentBlock);
	currentMethod->push(add);
cout << "VISIT OP visit_arith_binary model_plus \n";
}

		void
llvm_code_generator::visit_arith_shift (model_left_shift *,
				const ref_expression &,
				const ref_expression &)
{
cout << "VISIT  model_left_shift \n";
}

		void
llvm_code_generator::visit_arith_shift (model_right_shift *,
				const ref_expression &,
				const ref_expression &)
{
cout << "VISIT  model_right_shift \n";

}

		void
llvm_code_generator::visit_arith_shift (model_unsigned_right_shift *,
				const ref_expression &,
				const ref_expression &)
{
cout << "VISIT  model_unsigned_right_shift \n";

}

		void
llvm_code_generator::visit_cast (model_cast *, const ref_forwarding_type &,
				const ref_expression &)
{
cout << "VISIT visit_cast \n";
}

		void
llvm_code_generator::visit_class_ref (model_class_ref *,
				const ref_forwarding_type &)
{
cout << "VISIT  visit_class_ref  \n";
}

		void
llvm_code_generator::visit_comparison (model_equal *,
				const ref_expression &,
				const ref_expression &)
{

cout << "VISIT visit_comparison model_equal \n";
}

		void
llvm_code_generator::visit_comparison (model_notequal *,
				const ref_expression &,
				const ref_expression &)
{
cout << "VISIT  model_notequal  \n";
}

		void
llvm_code_generator::visit_comparison (model_lessthan *,
				const ref_expression &,
				const ref_expression &)
{
cout << "VISIT  model_lessthan  \n";

}

		void
llvm_code_generator::visit_comparison (model_greaterthan *,
				const ref_expression &,
				const ref_expression &)
{
cout << "VISIT  model_greaterthan  \n";

}

		void
llvm_code_generator::visit_comparison (model_lessthanequal *,
				const ref_expression &,
				const ref_expression &)
{
cout << "VISIT  model_lessthanequal  \n";

}

		void
llvm_code_generator::visit_comparison (model_greaterthanequal *,
				const ref_expression &,
				const ref_expression &)
{
cout << "VISIT  model_greaterthanequal  \n";

}

		void
llvm_code_generator::visit_conditional (model_conditional *,
				const ref_expression &,
				const ref_expression &,
				const ref_expression &)
{
cout << "VISIT  model_conditional  \n";

}

		void
llvm_code_generator::visit_field_ref (model_field_ref *,
				const ref_expression &,
				model_field *)
{
cout << "VISIT visit_field_ref \n";

}

		void
llvm_code_generator::visit_field_initializer (model_field_initializer *,
				model_field *)
{
cout << "VISIT FIELD INITIALIZER \n";

}

		void
llvm_code_generator::visit_instanceof (model_instanceof *,
				const ref_expression &,
				const ref_forwarding_type &)
{
cout << "VISIT  model_instanceof  \n";

}

		void
llvm_code_generator::visit_logical_binary (model_lor *,
				const ref_expression &,
				const ref_expression &)
{
cout << "VISIT  model_lor  \n";

}

		void
llvm_code_generator::visit_logical_binary (model_land *,
				const ref_expression &,
				const ref_expression &)
{
cout << "VISIT  model_land  \n";

}

		void
llvm_code_generator::visit_simple_literal (model_literal_base *,
				const jboolean &val)
{
cout << "VISIT  visit_simple_literal jboolean \n";
	currentMethod->rvalue = ConstantBool::get(Type::BoolTy,val);

}

		void
llvm_code_generator::visit_simple_literal (model_literal_base *,
				const jbyte &val)
{
cout << "VISIT  visit_simple_literal jbyte \n";
	currentMethod->rvalue = ConstantSInt::get(Type::SByteTy,val);
}

		void
llvm_code_generator::visit_simple_literal (model_literal_base *,
				const jchar &val)
{
cout << "VISIT  visit_simple_literal jchar \n";
	currentMethod->rvalue = ConstantSInt::get(Type::ShortTy,val);

}

		void
llvm_code_generator::visit_simple_literal (model_literal_base *,
				const jshort &val)
{
cout << "VISIT  visit_simple_literal jshort \n";
	currentMethod->rvalue = ConstantSInt::get(Type::ShortTy,val);

}

		void
llvm_code_generator::visit_simple_literal (model_literal_base *,
				const jint &val)
{
cout << "VISIT visit_simple_literal jint " << val << "\n";
	currentMethod->push( ConstantSInt::get(Type::IntTy,val));
}

		void
llvm_code_generator::visit_simple_literal (model_literal_base *,
				const jlong &val)
{
cout << "VISIT visit_simple_literal jlong \n";
	currentMethod->rvalue = ConstantSInt::get(Type::LongTy,val);
}

		void
llvm_code_generator::visit_simple_literal (model_literal_base *,
				const jfloat &val)
{
cout << "VISIT visit_simple_literal jfloat \n";
	currentMethod->rvalue = ConstantFP::get(Type::FloatTy,val);

}

		void
llvm_code_generator::visit_simple_literal (model_literal_base *,
				const jdouble &val)
{
cout << "VISIT visit_simple_literal jdouble \n";
	currentMethod->rvalue = ConstantFP::get(Type::DoubleTy,val);
}

		void
llvm_code_generator::visit_string_literal (model_string_literal *,
				const string &val)
{
cout << "VISIT visit_string_literal \n";
}

		void
llvm_code_generator::visit_method_invocation (model_method_invocation *,
				model_method * method,
				const ref_expression &this_expr,
				const list<ref_expression> &args)
{
	if( this_expr )
		this_expr->visit(this);
	if (method->static_p ()) {
		//push this ?
	}else {
		//push class ?

	}

    for (std::list<ref_expression>::const_iterator i = args.begin ();
     	i != args.end (); ++i)
	{
		(*i)->visit (this);
	}
	llvm_function *function = get_method(method);
	currentMethod->emitCall(function);
}

		void
llvm_code_generator::visit_type_qualified_invocation (model_type_qualified_invocation *,
				const model_method *,
				const list<ref_expression> &,
				bool)
{

cout << "VISIT visit_type_qualified_invocation \n";
}

		void
llvm_code_generator::visit_super_invocation (model_super_invocation *,
				const model_method *,
				const list<ref_expression> &args,
				const ref_expression &finit)
{
    for (list<ref_expression>::const_iterator i = args.begin ();
     	i != args.end (); ++i)
	{
		(*i)->visit (this);
	}
#if 0
	/* field initializer */
	if( finit ) 
		finit->visit(this);
#endif
cout << "VISIT visit_super_invocation \n";
}

		void
llvm_code_generator::visit_this_invocation (model_this_invocation *,
				const model_method *,
				const list<ref_expression> &)
{
cout << "VISIT visit_this_invocation \n";

}

		void
llvm_code_generator::visit_new (model_new *, const model_method *,
				const ref_forwarding_type &,
				const list<ref_expression> &)
{

}

		void
llvm_code_generator::visit_new_array (model_new_array *,
				const ref_forwarding_type &,
				const list<ref_expression> &,
				const ref_expression &)
{

}

		void
llvm_code_generator::visit_null_literal (model_null_literal *)
{

cout << "VISIT visit_null_literal \n";
}

		void
llvm_code_generator::visit_prefix_simple (model_prefix_plus *,
				const ref_expression &)
{

}

		void
llvm_code_generator::visit_prefix_simple (model_prefix_minus *,
				const ref_expression &)
{

}

		void
llvm_code_generator::visit_prefix_simple (model_bitwise_not *,
				const ref_expression &)
{

}

		void
llvm_code_generator::visit_prefix_simple (model_logical_not *,
				const ref_expression &)
{

}

		void
llvm_code_generator::visit_prefix_side_effect (model_prefix_plusplus *,
				const ref_expression &)
{

}

		void
llvm_code_generator::visit_prefix_side_effect (model_prefix_minusminus *,
				const ref_expression &)
{

}

		void
llvm_code_generator::visit_postfix_side_effect (model_postfix_plusplus *,
				const ref_expression &)
{

}

		void
llvm_code_generator::visit_postfix_side_effect (model_postfix_minusminus *,
				const ref_expression &)
{

}

		void
llvm_code_generator::visit_this (model_this *)
{

cout << "VISIT visit_this \n";
}

		void
llvm_code_generator::visit_simple_variable_ref (model_simple_variable_ref *,
				const model_variable_decl *decl)
{
	currentMethod->push(localmap[decl]);
cout << "VISIT visit_simple_variable_ref \n";

}


