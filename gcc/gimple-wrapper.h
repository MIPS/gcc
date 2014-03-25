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
    T* ptr() const { return static_cast<T *>(Tree); }
    void set_ptr(const void *p) 
		{ Tree = reinterpret_cast<tree_desc *>(const_cast<void *>(p)); }
  public:
    inline _ptr() { Tree = NULL; } inline _ptr(const gimple_null& g ATTRIBUTE_UNUSED) { Tree = NULL; }
    inline _ptr (const tree t)  { set_ptr (t); check_contents (); }
    inline _ptr (const_tree t)  { set_ptr (t); check_contents (); }
    inline _ptr (const T *v) { set_ptr (v); }
    inline _ptr& operator= (const tree t) 
				    { set_ptr (t); check_contents (); return *this;}
    inline _ptr& operator= (const_tree t)
				    { set_ptr (t); check_contents (); return *this;}
    inline _ptr& operator= (const T *v) { set_ptr (v); return *this;}
    inline _ptr& operator= (const gimple_null& g ATTRIBUTE_UNUSED)
				    { Tree = NULL; return *this;}

    bool operator!() const { return Tree == NULL; }
    inline operator tree () const { return reinterpret_cast<tree>(Tree); }

    inline T * operator->() { return ptr (); }
    inline const T * operator->() const { return ptr (); }
    inline T& operator*() { return *ptr (); }
    inline const T& operator*() const { return *ptr (); }

    friend bool is_same(const _ptr a, _ptr b)
				    { return a->code () == b->code (); }

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
    template<typename, typename> friend class _dptr;
};

template<typename pT, typename dT>
class _dptr : public dT
{
  protected:
    static inline bool Test (const dT &);
    inline void check_contents () const;
    static inline bool is_a(const dT &d) { return Test (d); }
    static inline  pT * as_a(dT &d)
			    { return static_cast<pT *>(d.operator->()); }
    static inline  const pT * as_a(const dT &d)
			    { return static_cast<const pT *>(d.operator->()); }
    static inline pT * dyn_cast(dT &d) 
	  { return (Test (d)) ? static_cast<pT *>(d.operator->()) : NULL; }
    static inline const pT * dyn_cast(const dT &d) 
	  { return (Test (d)) ? static_cast<const pT *>(d.operator->()) : NULL; }
    pT *ptr() const { return static_cast<pT *>(dT::Tree); }
    void set_ptr(const void *p) 
		{ dT::Tree = reinterpret_cast<tree_desc *>(const_cast<void *>(p)); }
  public:

    inline _dptr () : dT () { }
    inline _dptr (const gimple_null& g ATTRIBUTE_UNUSED) : dT() { }
    inline _dptr (const tree t) : dT (t) { check_contents(); }
    inline _dptr (const_tree t) : dT (t) { check_contents(); }
    inline _dptr (const dT& d) : dT () { if (d) dT::operator= (dyn_cast (d)); }
    inline _dptr (const pT *n) : dT () {  dT::set_ptr (n); }

    inline _dptr& operator= (const tree t) 
			  { dT::operator= (t); check_contents(); return *this; }
    inline _dptr& operator= (const_tree t) 
			  { dT::operator= (t); check_contents(); return *this; }
    inline _dptr& operator= (const dT& d) 
			  { if (d) dT::operator= (dyn_cast (d)); else dT::set_ptr (NULL); return *this; }
    inline _dptr& operator= (const pT *p)
					  { dT::set_ptr (p); return *this; }
    inline _dptr& operator= (const gimple_null& g ATTRIBUTE_UNUSED)
				    { dT::set_ptr (NULL); return *this;}

    inline pT * operator->() { return static_cast<pT *>(dT::ptr()); }
    inline const pT * operator->() const 
			  { return static_cast<const pT *>(dT::ptr()); }
    inline pT& operator*() { return *static_cast<pT *>(dT::ptr()); }
    inline const pT& operator*() const
			  { return *static_cast<const pT *>(dT::ptr()); }

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


typedef _ptr<value_desc>			value;
template<>
inline void 
_ptr<value_desc>::check_contents() const
{
}

typedef _addr<value>			value_ptr;

typedef _ptr<type_desc>			type;
template<> inline void 
_ptr<type_desc>::check_contents() const
{
#if GIMPLE_CHECKING_ON
  if (Tree)
    Tree->check_node (tcc_type);
#endif
}

typedef _ptr<block_desc>			block;
template<>
inline void 
_ptr<block_desc>::check_contents() const
{
#if GIMPLE_CHECKING_ON
  if (Tree)
    Tree->check_node (BLOCK);
#endif
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
{ return d.Tree->test_node (CC); }


DERIVED_PTR (decl, tcc_declaration, value)
DERIVED_PTR (constant, tcc_constant, value)
DERIVED_PTR (expr, GIMPLE_INVALID_RHS, value)
DERIVED_PTR (comparison, tcc_comparison, value)
DERIVED_PTR (unary, tcc_unary, value)
DERIVED_PTR (binary, tcc_binary, value)

DERIVED_PTR (identifier, IDENTIFIER_NODE, value) 
DERIVED_PTR (integer_cst, INTEGER_CST, value)
DERIVED_PTR (var_decl, VAR_DECL, value)
DERIVED_PTR (parm_decl, PARM_DECL, value)
DERIVED_PTR (result_decl, RESULT_DECL, value)
DERIVED_PTR (label_decl, LABEL_DECL, value)
DERIVED_PTR (function_decl, FUNCTION_DECL, value)
DERIVED_PTR (ssa_name, SSA_NAME, value)
DERIVED_PTR (mem_ref, MEM_REF, value)
DERIVED_PTR (addr_expr, ADDR_EXPR, value)
DERIVED_PTR (value_list, TREE_LIST, value)
DERIVED_PTR (type_list, TREE_LIST, value)
DERIVED_PTR (identifier_list, TREE_LIST, value)
DERIVED_PTR (case_label_expr, CASE_LABEL_EXPR, value)
DERIVED_PTR (truth_not_expr, TRUTH_NOT_EXPR, value)
DERIVED_PTR (with_size_expr, WITH_SIZE_EXPR, value)
DERIVED_PTR (call_expr, CALL_EXPR, value)
DERIVED_PTR (nop_expr, NOP_EXPR, value)
DERIVED_PTR (bit_field_ref, BIT_FIELD_REF, value)
DERIVED_PTR (target_mem_ref, TARGET_MEM_REF, value)
DERIVED_PTR (array_ref, ARRAY_REF, value)
DERIVED_PTR (array_range_ref, ARRAY_RANGE_REF, value)
DERIVED_PTR (obj_type_ref, OBJ_TYPE_REF, value)

DERIVED_PTR (boolean_type, BOOLEAN_TYPE, type)
DERIVED_PTR (integer_type, INTEGER_TYPE, type)
DERIVED_PTR (real_type, REAL_TYPE, type)
DERIVED_PTR (fixed_point_type, FIXED_POINT_TYPE, type)
DERIVED_PTR (function_type, FUNCTION_TYPE, type)
DERIVED_PTR (method_type, METHOD_TYPE, type)
DERIVED_PTR (array_type, ARRAY_TYPE, type)
DERIVED_PTR (complex_type, COMPLEX_TYPE, type)
DERIVED_PTR (vector_type, VECTOR_TYPE, type)

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

} // namespace Gimple

#endif  /* GIMPLE_WRAPPER_H  */
