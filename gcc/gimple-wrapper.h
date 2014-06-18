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


class gimple_null {
  public:
    gimple_null () { }
    bool operator!() const { return true; }
    operator long int() { return 0; }
    operator tree() { return (tree)0; }
};

static gimple_null NULL_GIMPLE;

template<typename T, typename D>
inline bool is_a (D v)
{ return T::is_a (v); }

template<typename T, typename D>
inline T as_a (D v)
{ return T::as_a (v); }

template<typename T, typename D>
inline T dyn_cast (D v)
{ return T::dyn_cast (v); }

namespace Gimple {

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
    inline _ptr& operator= (const tree t) 
			    { set_ptr (t); check_contents (); return *this;}
    inline _ptr& operator= (const_tree t)
			    { set_ptr (t); check_contents (); return *this;}
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

    template <typename TT>
    friend bool operator== (const _ptr<TT>&, long);
    template <typename TT>
    friend bool operator!= (const _ptr<TT>&, long); 
    template <typename TT> friend TT copy (TT ptr);
    template <typename TT> friend TT create ();

    template<typename, typename> friend class _dptr;
    template<typename> friend class _addr;
};


template<typename pT, typename dT>
class _dptr : public dT
{
  protected:
    pT *ptr() const { return static_cast<pT *>(dT::Tree); }
    void set_ptr(const void *p) 
	    { dT::Tree = reinterpret_cast<tree_desc *>(const_cast<void *>(p)); }
    static inline bool Test (const dT &);
    inline void check_contents () const;
    static inline _dptr create ();
    inline _dptr copy () const;
    static inline bool is_a(const dT &d) { return Test (d); }
    static inline  pT * as_a(dT &d) { return static_cast<pT *>(d.Tree); }
    static inline  const pT * as_a(const dT &d)
				    { return static_cast<const pT *>(d.Tree); }
    static inline pT * dyn_cast(dT &d) 
		      { return (Test (d)) ? static_cast<pT *>(d.Tree) : NULL; }
    static inline const pT * dyn_cast(const dT &d) 
		{ return (Test (d)) ? static_cast<const pT *>(d.Tree) : NULL; }

  public:

    inline _dptr () : dT () { }
    inline _dptr (const gimple_null& g ATTRIBUTE_UNUSED) : dT() { }
    inline _dptr (const tree t) : dT (t) { check_contents(); }
    inline _dptr (const_tree t) : dT (t) { check_contents(); }
    inline _dptr (const dT& d) : dT () { if (d) set_ptr ((dyn_cast (d))); }
    inline _dptr (const pT *n) : dT () {  dT::set_ptr (n); }
    inline _dptr (int x ATTRIBUTE_UNUSED) : dT () { gcc_assert (!x); }

    inline _dptr& operator= (const tree t) 
				{ set_ptr (t); check_contents(); return *this; }
    inline _dptr& operator= (const_tree t) 
				{ set_ptr (t); check_contents(); return *this; }
    inline _dptr& operator= (const dT& d) 
	   { if (d) set_ptr (dyn_cast (d)); else set_ptr (NULL); return *this; }
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


//////////////////////////////////////////////////////////////////
//
// Instances
//
//////////////////////////////////////////////////////////////////

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


typedef _ptr<value_ops_desc>		value;

typedef _ptr<type_desc>			type;
typedef _ptr<block_desc>		block;

template<>
inline void 
_ptr<value_ops_desc>::check_contents() const
{
}

template<>
inline _ptr<value_ops_desc> 
_ptr<value_ops_desc>::create()
{
  gcc_unreachable ();
}

template<>
inline void 
_ptr<type_desc>::check_contents() const
{
#if GIMPLE_CHECKING_ON
  if (Tree)
    Tree->check_node (tcc_type);
#endif
}

template<>
inline _ptr<type_desc> 
_ptr<type_desc>::create()
{
  gcc_unreachable ();
}

template<>
inline void 
_ptr<block_desc>::check_contents() const
{
#if GIMPLE_CHECKING_ON
  if (Tree)
    Tree->check_node (BLOCK);
#endif
}

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

#define DERIVED_PTR(LAB, CC, DT)			\
typedef _dptr<LAB ## _desc, DT>	LAB;			\
template<> inline void 					\
_dptr<LAB ## _desc, DT>::check_contents() const		\
{ CHKNODE (CC) ; }					\
template<> inline bool					\
_dptr<LAB ## _desc, DT>::Test(const DT &d)		\
{ return d.Tree->test_node (CC); }			\
template<> inline _dptr<LAB ## _desc, DT>		\
_dptr<LAB ## _desc, DT>::create ()			\
{ gcc_unreachable (); }



#define TERMINAL_PTR(LAB, CC, DT)			\
typedef _dptr<LAB ## _desc, DT>	LAB;			\
template<> inline void 					\
_dptr<LAB ## _desc, DT>::check_contents() const		\
{ CHKNODE (CC) ; }					\
template<> inline bool					\
_dptr<LAB ## _desc, DT>::Test(const DT &d)		\
{ return d.Tree->test_node (CC); }			\
template<> inline _dptr<LAB ## _desc, DT>		\
_dptr<LAB ## _desc, DT>::create ()			\
{ return _dptr<LAB ## _desc, DT> 			\
   (reinterpret_cast<tree> (gimple_make_node (CC))); }




DERIVED_PTR (decl, tcc_declaration, value)
DERIVED_PTR (constant, tcc_constant, value)
DERIVED_PTR (comparison, tcc_comparison, value)
DERIVED_PTR (unary, tcc_unary, value)
DERIVED_PTR (binary, tcc_binary, value)

TERMINAL_PTR (identifier, IDENTIFIER_NODE, value) 
TERMINAL_PTR (integer_cst, INTEGER_CST, value)
TERMINAL_PTR (real_cst, REAL_CST, value)
TERMINAL_PTR (var_decl, VAR_DECL, value)
TERMINAL_PTR (parm_decl, PARM_DECL, value)
TERMINAL_PTR (result_decl, RESULT_DECL, value)
TERMINAL_PTR (label_decl, LABEL_DECL, value)
TERMINAL_PTR (function_decl, FUNCTION_DECL, value)
TERMINAL_PTR (debug_expr_decl, DEBUG_EXPR_DECL, value)

TERMINAL_PTR (ssa_name, SSA_NAME, value)
TERMINAL_PTR (mem_ref, MEM_REF, value)
TERMINAL_PTR (addr_expr, ADDR_EXPR, value)
TERMINAL_PTR (value_list, TREE_LIST, value)
TERMINAL_PTR (type_list, TREE_LIST, value)
TERMINAL_PTR (identifier_list, TREE_LIST, value)
TERMINAL_PTR (case_label_expr, CASE_LABEL_EXPR, value)
TERMINAL_PTR (truth_not_expr, TRUTH_NOT_EXPR, value)
TERMINAL_PTR (with_size_expr, WITH_SIZE_EXPR, value)
TERMINAL_PTR (call_expr, CALL_EXPR, value)
TERMINAL_PTR (nop_expr, NOP_EXPR, value)
TERMINAL_PTR (bit_field_ref, BIT_FIELD_REF, value)
TERMINAL_PTR (target_mem_ref, TARGET_MEM_REF, value)
TERMINAL_PTR (array_ref, ARRAY_REF, value)
TERMINAL_PTR (array_range_ref, ARRAY_RANGE_REF, value)
TERMINAL_PTR (obj_type_ref, OBJ_TYPE_REF, value)
TERMINAL_PTR (constructor, CONSTRUCTOR, value)
TERMINAL_PTR (modify_expr, MODIFY_EXPR, value)

TERMINAL_PTR (boolean_type, BOOLEAN_TYPE, type)
TERMINAL_PTR (integer_type, INTEGER_TYPE, type)
TERMINAL_PTR (real_type, REAL_TYPE, type)
TERMINAL_PTR (fixed_point_type, FIXED_POINT_TYPE, type)
TERMINAL_PTR (function_type, FUNCTION_TYPE, type)
TERMINAL_PTR (method_type, METHOD_TYPE, type)
TERMINAL_PTR (array_type, ARRAY_TYPE, type)
TERMINAL_PTR (complex_type, COMPLEX_TYPE, type)
TERMINAL_PTR (vector_type, VECTOR_TYPE, type)

DERIVED_PTR (decl_with_rtl, TS_DECL_WRTL, value)
DERIVED_PTR (decl_with_viz, TS_DECL_WITH_VIS, value)
DERIVED_PTR (decl_noncommon, TS_DECL_NON_COMMON, value)

#define MULTIARGS(...) __VA_ARGS__
DERIVED_PTR (numerical_type, 
	     MULTIARGS (INTEGER_TYPE, ENUMERAL_TYPE, BOOLEAN_TYPE, REAL_TYPE,
			FIXED_POINT_TYPE),
	     type)

DERIVED_PTR (function_or_method_type,
	     MULTIARGS (FUNCTION_TYPE, METHOD_TYPE),
	     type)

typedef _addr<type>			type_ptr;
typedef _addr<value>			value_ptr;
typedef _addr<integer_cst>		integer_cst_ptr;

} // namespace Gimple


#endif  /* GIMPLE_WRAPPER_H  */
