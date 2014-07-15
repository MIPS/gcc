/* Gimple wrapper class definitions for pointers to gimple objects. 

   Copyright (C) 2014 Free Software Foundation, Inc.
   Contributed by Andrew MacLeod  <amacleod@redhat.com>

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#ifndef GIMPLE_WRAPPER_H
#define GIMPLE_WRAPPER_H

#if defined ENABLE_TREE_CHECKING && (GCC_VERSION >= 2007)
#define GIMPLE_CHECKING_ON	1
extern void tree_check_failed (const_tree, const char *, int, const char *,
                               ...) ATTRIBUTE_NORETURN;
extern void tree_class_check_failed (const_tree, const enum tree_code_class,
                                     const char *, int, const char *)
				     ATTRIBUTE_NORETURN;
extern void tree_contains_struct_check_failed (const_tree,
					   const enum tree_node_structure_enum,
					   const char *, int, const char *)
					   ATTRIBUTE_NORETURN;
extern void tree_operand_check_failed (int, const_tree,
				       const char *, int, const char *)
				       ATTRIBUTE_NORETURN;
extern void tree_int_cst_elt_check_failed (int, int, const char *,
					   int, const char *) 
					   ATTRIBUTE_NORETURN;


#else
#define GIMPLE_CHECKING_ON	0
#endif

/* Define a unique name for checks that are only to help with the re-arch work.
   Things that should never ever fail... but just in case as_a is in the wrong
   place...  unique name so we can easily rip them out later, or do performance
   analysis or eliminate them from the compiler.  */

#define extra_checking_assert	gcc_checking_assert

/* Define a class which performs the same role as TREE_NULL.  Using a special
   class allows all the interface methods to handle NULL without expecting a 
   long or a pointer.  The class knows how to be interpreted as an int or 
   a tree to allow interaction with the existing tree interface.  */

class gimple_null {
  public:
    gimple_null () { }
    bool operator!() const { return true; }
    operator long int() { return 0; }
    operator tree() { return (tree)0; }
};

/* Define a static instance to perform the same function a TREE_NULL. */

static gimple_null NULL_GIMPLE;


/* Forward declare the templates for is_a, as_a, and dyn_cast outside
   the gimple namspace.  
   we cannot use the definition in is-a.h since these gimple wrappers are
   a ctually smart pointer classes, not actual pointers.  Since they are
   class instances passed by value, ths give the 3 routins a different
   signature than the is-a.h templates, so everything just works.  */

template<typename T, typename D> inline bool is_a (D v);
template<typename T, typename D> inline T as_a (D v);
template<typename T, typename D> inline T dyn_cast (D v);

namespace G {

/* This is a basic gimple object pointer.  The different instances of a basic
   type are not compatible with each other.  ie, a type cannot be passed to a
   value as they are fundamentally incompatible.  */

template<typename T>
class _ptr 
{
  protected:
    class tree_desc *Tree;
    inline void check_contents () const;
    static inline _ptr create ();
    inline _ptr copy () const;
    T* ptr() const { return static_cast<T *>(Tree); }
    void set_ptr(const void *p) 
		{ Tree = reinterpret_cast<tree_desc *>(const_cast<void *>(p)); }
  public:
    inline _ptr () { Tree = NULL; }
    inline _ptr (const gimple_null& g ATTRIBUTE_UNUSED) { Tree = NULL; }
    inline _ptr (const tree t)  { set_ptr (t); check_contents (); }
    inline _ptr (const_tree t)  { set_ptr (t); check_contents (); }
    inline _ptr (const T *v) { set_ptr (v); }
    inline _ptr (int x ATTRIBUTE_UNUSED) { gcc_assert (!x); set_ptr (NULL); }
    inline _ptr& operator= (const gimple_null& g ATTRIBUTE_UNUSED)
				    { Tree = NULL; return *this;}
/*
    inline _ptr& operator= (const tree t) 
			    { set_ptr (t); check_contents (); return *this;}
    inline _ptr& operator= (const_tree t)
			    { set_ptr (t); check_contents (); return *this;}
*/
    inline _ptr& operator= (const T *v) { set_ptr (v); return *this;}
    inline _ptr& operator= (int x ATTRIBUTE_UNUSED) 
			  { gcc_assert (!x); set_ptr (NULL); return *this; }

    bool operator!() const { return Tree == NULL; }
    inline operator tree () const { return reinterpret_cast<tree>(Tree); }

    inline T * operator->() { return ptr (); }
    inline const T * operator->() const { return ptr (); }
    inline T& operator*() { return *ptr (); }
    inline const T& operator*() const { return *ptr (); }

    template <typename TT>
    friend bool operator== (const _ptr<TT>&, const _ptr<TT>&);
    template <typename TT>
    friend bool operator!= (const _ptr<TT>&, const _ptr<TT>&);
    template <typename TT>
    friend bool operator== (const _ptr<TT>&, const tree&);
    template <typename TT>
    friend bool operator!= (const _ptr<TT>&, const tree&);
    template <typename TT>
    friend bool operator== (const tree&, const _ptr<TT>&);
    template <typename TT>
    friend bool operator!= (const tree&, const _ptr<TT>&);
    template <typename TT>
    friend bool operator== (const _ptr<TT>&, const gimple_null& v);
    template <typename TT>
    friend bool operator!= (const _ptr<TT>&, const gimple_null& v);
    template <typename TT>
    friend bool operator== (const _ptr<TT>& a, const T *b);
    template <typename TT>
    friend bool operator!= (const _ptr<TT>& a, const T *b);
    template <typename TT>
    friend bool operator== (const T *b, const _ptr<TT>& a);
    template <typename TT>
    friend bool operator!= (const T *b, const _ptr<TT>& a);

    template <typename TT> friend bool operator== (const _ptr<TT>&, long);
    template <typename TT> friend bool operator!= (const _ptr<TT>&, long); 
    template <typename TT> friend TT copy (TT ptr);
    template <typename TT> friend TT create ();

    template<typename, typename> friend class _dptr;
    template<typename> friend class _addr;
    template <typename TT, typename D> bool friend ::is_a(D);
    template <typename TT, typename D> TT friend ::as_a(D);
    template <typename TT, typename D> TT friend ::dyn_cast(D);

};

/* Various friend classes.  */

template <typename T>
inline bool 
operator== (const _ptr<T>& t1, const _ptr<T>& t2) 
{ return t1.Tree == t2.Tree; }

template <typename T>
inline bool 
operator!= (const _ptr<T>& t1, const _ptr<T>& t2) 
{ return t1.Tree != t2.Tree; }

template <typename T>
inline bool 
operator== (const _ptr<T>& t1, const tree& t2) 
{ return reinterpret_cast<tree>(t1.Tree) == t2; }

template <typename T>
inline bool 
operator!= (const _ptr<T>& t1, const tree& t2) 
{ return reinterpret_cast<tree>(t1.Tree) != t2; }

template <typename T>
inline bool 
operator== (const tree& t1, const _ptr<T>& t2) 
{ return t1 == reinterpret_cast<tree>(t2.Tree); }

template <typename T>
inline bool 
operator!= (const tree& t1, const _ptr<T>& t2) 
{ return t1 != reinterpret_cast<tree>(t2.Tree); }

template <typename T>
inline bool 
operator== (const _ptr<T>& a, const T *b)
{ return a.ptr() == b; }

template <typename T>
inline bool
operator!= (const _ptr<T>& a, const T *b)
{ return a.ptr() != b; }

template <typename T>
inline bool
operator== (const T *b, const _ptr<T>& a)
{ return a.ptr() == b; }

template <typename T>
inline bool
operator!= (const T *b, const _ptr<T>& a)
{ return a.ptr() != b; }

template <typename T>
inline bool 
operator== (const _ptr<T>& t1, const gimple_null& n ATTRIBUTE_UNUSED)
{ return t1.Tree == NULL; }

template <typename T>
inline bool 
operator!= (const _ptr<T>& t1, const gimple_null& n ATTRIBUTE_UNUSED) 
{ return t1.Tree != NULL; }



/* Define instances and typedefs for the baic gimple objects.  */

typedef _ptr<value_desc>		value;
typedef _ptr<type_desc>			type;
typedef _ptr<block_desc>		block;

/* This template is for any object derived from one of the base types.  It
   includes the test method helper for is_a, as_a and dyn_cast.  */

template<typename pT, typename dT>
class _dptr : public dT
{
  protected:
    pT *ptr() const { return static_cast<pT *>(dT::Tree); }
    void set_ptr(const void *p) 
	    { dT::Tree = reinterpret_cast<tree_desc *>(const_cast<void *>(p)); }
    static inline bool test (value);
    static inline bool test (type);
    static inline _dptr create ();
    inline void check_contents () const;
    inline _dptr copy () const;

  public:

    inline _dptr () : dT () { }
    inline _dptr (const gimple_null& g ATTRIBUTE_UNUSED) : dT() { }
    inline _dptr (const tree t) : dT (t) { check_contents(); }
    inline _dptr (const_tree t) : dT (t) { check_contents(); }
    inline _dptr (const pT *n) : dT () {  dT::set_ptr (n); }
    inline _dptr (int x ATTRIBUTE_UNUSED) : dT () { gcc_assert (!x); }
/*
    inline _dptr& operator= (const tree t) 
				{ set_ptr (t); check_contents(); return *this; }
    inline _dptr& operator= (const_tree t) 
				{ set_ptr (t); check_contents(); return *this; }
*/
    inline _dptr& operator= (const pT *p) { set_ptr (p); return *this; }
    inline _dptr& operator= (const gimple_null& g ATTRIBUTE_UNUSED)
						{ set_ptr (NULL); return *this;}
    inline _dptr& operator= (int x ATTRIBUTE_UNUSED) 
			    { gcc_assert (!x); set_ptr (NULL); return *this; }

    inline pT * operator->() { return ptr(); }
    inline const pT * operator->() const { return ptr(); }
    inline pT& operator*() { return *ptr(); }
    inline const pT& operator*() const { return *ptr(); }

    template <typename T1, typename T2>
    friend bool operator== (const _dptr<T1,T2>& a, const T1 *b);
    template <typename T1, typename T2>
    friend bool operator!= (const _dptr<T1,T2>& a, const T1 *b);
    template <typename T1, typename T2>
    friend bool operator== (const T1 *b, const _dptr<T1,T2>& a);
    template <typename T1, typename T2>
    friend bool operator!= (const T1 *b, const _dptr<T1,T2>& a);

    template <typename T, typename D> bool friend ::is_a(D);
    template <typename T, typename D> T friend ::as_a(D);
    template <typename T, typename D> T friend ::dyn_cast(D);
    template <typename TT> friend TT copy (TT ptr);
    template <typename TT> friend TT create ();
    template<typename> friend class _addr;
};

/* Various friend classes.  */

template <typename pT, typename dT>
inline bool operator== (const _dptr<pT, dT>& a, const pT *b)
{ return a.ptr() == b; }

template <typename pT, typename dT>
inline bool operator!= (const _dptr<pT, dT>& a, const pT *b)
{ return a.ptr() != b; }

template <typename pT, typename dT>
inline bool operator== (const pT *b, const _dptr<pT, dT>& a)
{ return a.ptr() == b; }

template <typename pT, typename dT>
inline bool operator!= (const pT *b, const _dptr<pT, dT>& a)
{ return a.ptr() != b; }

template <typename T>
inline bool 
operator== (const _ptr<T>& t1, long t2) 
{ return t1.Tree == reinterpret_cast<class tree_desc *>(t2); }

template <typename T>
inline bool 
operator!= (const _ptr<T>& t1, long t2) 
{ return t1.Tree != reinterpret_cast<class tree_desc *>(t2); }


/* This is the basic address of an object type.  ie, a pointer to smart poimter.
   This class is used instead of &value. 
   The rationale for this is again to allow for interaction with the
   unconverted tree interface.  It is a requirement that the address of a
   gimple object be type compatible with a 'tree *'.  This class allows that by
   allowng construction from trees and casting to trees. 
   When the tree interface is no longer required, this can be replaced with 
   a simple 'T *'.  */

template<typename T>
class _addr 
{
  protected:
    T *ptr;
  public:
    inline _addr () { ptr = NULL; }
    inline _addr (const gimple_null& g ATTRIBUTE_UNUSED) { ptr = NULL;}
    inline _addr (tree *t) { ptr = reinterpret_cast<T *> (t); }
    inline _addr (T *t) { ptr = t; }
    inline T& operator*() { return *ptr; }
    inline operator tree *() const { return reinterpret_cast<tree *>(ptr); }
};

/* From tree.c for create () and copy () methods.  */

extern void * gimple_copy_node (const void *t);
extern void * gimple_make_node (enum tree_code t);

template<typename T>
inline _ptr<T> 
_ptr<T>::copy() const
{
  return _ptr<T> (reinterpret_cast<tree> (gimple_copy_node (ptr())));
}

template<typename T, typename D>
inline _dptr<T, D> 
_dptr<T, D>::copy() const
{
  return _dptr<T, D> (reinterpret_cast<tree> (gimple_copy_node (ptr())));
}

template<typename T>
inline T
copy(T ptr)
{
  return ptr.copy();
}

template<typename T>
inline T
create ()
{
  return T::create ();
}


//////////////////////////////////////////////////////////////////
//
// Instances
//
//////////////////////////////////////////////////////////////////



/* A value doesnt need to check anything.  */
template<>
inline void 
_ptr<value_desc>::check_contents() const
{
}

/* There cannot be an instance of just a value node, it must be sort
   of concrete derived type.  */
template<>
inline _ptr<value_desc> 
_ptr<value_desc>::create()
{
  gcc_unreachable ();
}

/* Check a type node.  */
template<>
inline void 
_ptr<type_desc>::check_contents() const
{
#if GIMPLE_CHECKING_ON
  if (Tree)
    Tree->check_node (tcc_type);
#endif
}

/* A type node must be some sort of concrete derived type when created.  */
template<>
inline _ptr<type_desc> 
_ptr<type_desc>::create()
{
  gcc_unreachable ();
}

/* Check for a block node.  */
template<>
inline void 
_ptr<block_desc>::check_contents() const
{
#if GIMPLE_CHECKING_ON
  if (Tree)
    Tree->check_node (BLOCK);
#endif
}

/* Craeat a block node.  */
template<>
inline _ptr<block_desc> 
_ptr<block_desc>::create()
{
  return _ptr<block_desc> (reinterpret_cast<tree> (gimple_make_node (BLOCK)));
}

#if GIMPLE_CHECKING_ON
#define CHKNODE(...) if (Tree) Tree->check_node (__VA_ARGS__)
#else
#define CHKNODE(CC)
#endif

/* Define the basic template functions for a derived type which does not form
   a concrete type.  This is for things like a DECL node... it is derived from
   a VALUE node, but you cannot create a node of that type. it has to be one
   of the concrete decl objects, like a PARM_DECL.
   Note the test method works off the BASE class for the type.  This allows
   a test for any intermediary class along the inheritance chain. */

#define DERIVED_PTR(LAB, CC, DT, BT)			\
typedef _dptr<LAB ## _desc, DT>	LAB;			\
template<> inline void 					\
_dptr<LAB ## _desc, DT>::check_contents() const		\
{ CHKNODE (CC) ; }					\
template<> inline bool					\
_dptr<LAB ## _desc, DT>::test(BT b)			\
{ return b.ptr()->test_node (CC); }			\
template<> inline _dptr<LAB ## _desc, DT>		\
_dptr<LAB ## _desc, DT>::create ()			\
{ gcc_unreachable (); }


/* Define the basic template functions for a derived type which does form
   a concrete type. 
   Note the test method works off the BASE class for the type.  This allows
   a test for any intermediary class along the inheritance chain. */

#define TERMINAL_PTR(LAB, CC, DT, BT)			\
typedef _dptr<LAB ## _desc, DT>	LAB;			\
template<> inline void 					\
_dptr<LAB ## _desc, DT>::check_contents() const		\
{ CHKNODE (CC) ; }					\
template<> inline bool					\
_dptr<LAB ## _desc, DT>::test(BT b)			\
{ return b.ptr()->test_node (CC); }			\
template<> inline _dptr<LAB ## _desc, DT>		\
_dptr<LAB ## _desc, DT>::create ()			\
{ return _dptr<LAB ## _desc, DT> 			\
   (reinterpret_cast<tree> (gimple_make_node (CC))); }



#define MULTIARGS(...) __VA_ARGS__

/* Declare all the intermediate inherited types whihc do not define a concrete
   type with  a single gimple_code.  */

DERIVED_PTR (decl, tcc_declaration, value, value)
DERIVED_PTR (constant, tcc_constant, value, value)
DERIVED_PTR (comparison, tcc_comparison, value, value)
DERIVED_PTR (unary, tcc_unary, value, value)
DERIVED_PTR (binary, tcc_binary, value, value)
DERIVED_PTR (decl_with_rtl, TS_DECL_WRTL, decl, value)
DERIVED_PTR (decl_with_viz, TS_DECL_WITH_VIS, decl_with_rtl, value)
DERIVED_PTR (decl_noncommon, TS_DECL_NON_COMMON, decl_with_viz, value)

DERIVED_PTR (numerical_type, 
	     MULTIARGS (INTEGER_TYPE, ENUMERAL_TYPE, BOOLEAN_TYPE, REAL_TYPE,
			FIXED_POINT_TYPE),
	     type, type)
DERIVED_PTR (function_or_method_type,
	     MULTIARGS (FUNCTION_TYPE, METHOD_TYPE),
	     type, type)

/* Define all the concrete derived types which form actual node instances.  */

TERMINAL_PTR (identifier, IDENTIFIER_NODE, value, value) 
TERMINAL_PTR (integer_cst, INTEGER_CST, constant, value)
TERMINAL_PTR (real_cst, REAL_CST, constant, value)
TERMINAL_PTR (string_cst, STRING_CST, constant, value)
TERMINAL_PTR (var_decl, VAR_DECL, decl_with_viz, value)
TERMINAL_PTR (parm_decl, PARM_DECL, decl_with_rtl, value)
TERMINAL_PTR (result_decl, RESULT_DECL, decl_with_rtl, value)
TERMINAL_PTR (label_decl, LABEL_DECL, decl_with_rtl, value)
TERMINAL_PTR (function_decl, FUNCTION_DECL, decl_noncommon, value)
TERMINAL_PTR (debug_expr_decl, DEBUG_EXPR_DECL, decl, value)

TERMINAL_PTR (ssa_name, SSA_NAME, value, value)
TERMINAL_PTR (mem_ref, MEM_REF, value, value)
TERMINAL_PTR (addr_expr, ADDR_EXPR, value, value)
TERMINAL_PTR (value_list, TREE_LIST, value, value)
TERMINAL_PTR (type_list, TREE_LIST, value_list, value)
TERMINAL_PTR (identifier_list, TREE_LIST, value_list, value)
TERMINAL_PTR (case_label_expr, CASE_LABEL_EXPR, value, value)
TERMINAL_PTR (truth_not_expr, TRUTH_NOT_EXPR, value, value)
TERMINAL_PTR (with_size_expr, WITH_SIZE_EXPR, value, value)
TERMINAL_PTR (call_expr, CALL_EXPR, value, value)
TERMINAL_PTR (nop_expr, NOP_EXPR, value, value)
TERMINAL_PTR (bit_field_ref, BIT_FIELD_REF, value, value)
TERMINAL_PTR (target_mem_ref, TARGET_MEM_REF, value, value)
TERMINAL_PTR (array_ref, ARRAY_REF, value, value)
TERMINAL_PTR (array_range_ref, ARRAY_RANGE_REF, value, value)
TERMINAL_PTR (obj_type_ref, OBJ_TYPE_REF, value, value)
TERMINAL_PTR (constructor, CONSTRUCTOR, value, value)
TERMINAL_PTR (modify_expr, MODIFY_EXPR, value, value)
TERMINAL_PTR (const_decl, CONST_DECL, decl, value)

TERMINAL_PTR (boolean_type, BOOLEAN_TYPE, type, type)
TERMINAL_PTR (integer_type, INTEGER_TYPE, type, type)
TERMINAL_PTR (real_type, REAL_TYPE, type, type)
TERMINAL_PTR (fixed_point_type, FIXED_POINT_TYPE, type, type)
TERMINAL_PTR (function_type, FUNCTION_TYPE, function_or_method_type, type)
TERMINAL_PTR (method_type, METHOD_TYPE, function_or_method_type, type)
TERMINAL_PTR (array_type, ARRAY_TYPE, type, type)
TERMINAL_PTR (complex_type, COMPLEX_TYPE, type, type)
TERMINAL_PTR (vector_type, VECTOR_TYPE, type, type)


/* define the pointer to pointer types required.  */

typedef _addr<type>			type_ptr;
typedef _addr<value>			value_ptr;
typedef _addr<integer_cst>		integer_cst_ptr;

} // namespace G

/* Implement is_a ().  */

template<typename T, typename D>
inline bool is_a (D v)
{ 
  return T::test (v);
}

/* Implement as_a ().  */

template<typename T, typename D>
inline T as_a (D v)
{
  T t;
  t.set_ptr (v.ptr ());
  return t;
}

/* Implement dyn_cast ().  */
template<typename T, typename D>
inline T dyn_cast (D v)
{ 
  T t;
  if (T::test(v))
    t.set_ptr(v.ptr ());
  return t;
}


#endif  /* GIMPLE_WRAPPER_H  */
