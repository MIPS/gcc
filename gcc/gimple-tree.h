#ifndef GIMPLE_TREE_H
#define GIMPLE_TREE_H

#include "gimple-tree-core.h"

// TreeBase methods

inline enum tree_code 
TreeBase::TreeCode () const
{ return (enum tree_code) Tree->base.code; }

inline void
TreeBase::SetTreeCode (enum tree_code c) 
{ Tree->base.code = c; }



// GimpleType  methods.

inline enum tree_code 
GimpleType::code () const
{ return TreeBase::TreeCode(); }

inline GimpleType
GimpleType::type() const
{ return GimpleType (CONTAINS_STRUCT_CHECK (Tree, TS_TYPED)->typed.type); }

inline bool 
GimpleType::pointer_type_p () const
{ return code() == POINTER_TYPE || code() == REFERENCE_TYPE; }

inline bool 
GimpleType::type_unsigned () const
{ return Tree->base.u.bits.unsigned_flag; }

inline enum machine_mode
GimpleType::type_mode () const
{ 
  if (code() == VECTOR_TYPE)
    return vector_type_mode (Tree);
  else
    return Tree->type_common.mode;
}
  

// GimpleValue methods
//
inline enum tree_code 
GimpleValue::code () const
{ return TreeBase::TreeCode(); }

inline void
GimpleValue::set_code (enum tree_code c) 
{ TreeBase::SetTreeCode (c); }

inline GimpleType
GimpleValue::type() const
{ return GimpleType (CONTAINS_STRUCT_CHECK (Tree, TS_TYPED)->typed.type); }

inline void 
GimpleValue::set_type (GimpleType t)
{ CONTAINS_STRUCT_CHECK (Tree, TS_TYPED)->typed.type = t; }

//
// GimpleDecl methods
//

inline GimpleIdentifier
GimpleDecl::decl_name() const
{ return DECL_MINIMAL_CHECK (Tree)->decl_minimal.name; }

inline bool
GimpleDecl::decl_ignored_p () const
{ return DECL_COMMON_CHECK (Tree)->decl_common.ignored_flag; }

inline GimpleDecl 
GimpleDecl::decl_abstract_origin () const
{
  GimpleDecl orig = DECL_COMMON_CHECK (Tree)->decl_common.abstract_origin;
  return orig;
}

inline bool 
GimpleDecl::decl_from_inline () const
{
  GimpleDecl abstract_origin = decl_abstract_origin ();
  return abstract_origin != NULL_TREE && abstract_origin != Tree;
}

inline bool
GimpleDecl::readonly () const
{ return NON_TYPE_CHECK (Tree)->base.readonly_flag; }


//
// SSADecl
//

inline unsigned int 
SSADecl::ssa_name_version () const
{ return Tree->base.u.version; }

inline void
SSADecl::set_ssa_name_version (unsigned int v) 
{ Tree->base.u.version = v; }


inline bool
SSADecl::ssa_name_occurs_in_abnormal_phi () const
{ return Tree->base.asm_written_flag; }

inline GimpleDecl
SSADecl::ssa_name_var () const
{ 
  GimpleValue tmp = Tree->ssa_name.var;
  if (tmp == NULL_TREE || tmp.code() == IDENTIFIER_NODE)
    return NULL_TREE;
  return tmp;
}

inline GimpleIdentifier
SSADecl::ssa_name_identifier () const
{ 
  GimpleValue tmp = Tree->ssa_name.var;
  if (tmp != NULL_TREE)
    {
      if (tmp.code() == IDENTIFIER_NODE)
        return GimpleIdentifier(tmp);
      else
        {
	  GimpleDecl d = tmp;
	  return d.decl_name ();
	}
    }
  return NULL_TREE;
}

inline void
SSADecl::set_ssa_name_var (GimpleDecl decl)
{ Tree->ssa_name.var = decl; }

inline void
SSADecl::set_ssa_name_identifier (GimpleIdentifier ident)
{ Tree->ssa_name.var = ident; }

inline gimple 
SSADecl::ssa_name_def_stmt () const
{ return Tree->ssa_name.def_stmt; }

inline void 
SSADecl::set_ssa_name_def_stmt (gimple stmt)
{ Tree->ssa_name.def_stmt = stmt; }

inline struct ptr_info_def *
SSADecl::ssa_name_ptr_info () const
{ return Tree->ssa_name.ptr_info; }

inline void 
SSADecl::set_ssa_name_ptr_info (struct ptr_info_def *p)
{ Tree->ssa_name.ptr_info = p; }

inline bool 
SSADecl::ssa_name_in_free_list () const
{ return Tree->base.nothrow_flag; }

inline void 
SSADecl::set_ssa_name_in_free_list (bool f)
{ Tree->base.nothrow_flag = f; }

inline bool 
SSADecl::ssa_name_is_default_def () const
{ return Tree->base.default_def_flag; }

inline void 
SSADecl::set_ssa_name_is_default_def (bool f)
{ Tree->base.default_def_flag = f; }

inline struct ssa_use_operand_d * 
SSADecl::ssa_name_imm_use_node_ptr () const
{ return &(Tree->ssa_name.imm_uses); }

inline bool
SSADecl::ssa_name_same_base (const SSADecl &n) const
{ return (Tree->ssa_name.var == n.Tree->ssa_name.var); }

#endif
