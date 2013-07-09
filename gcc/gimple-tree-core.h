
#ifndef GIMPLE_CORE_H
#define GIMPLE_CORE_H


class GimpleExpr;
// This class accesses parts of trees that are common to all tree types.
// All methods other than comparisons are protected forcing inheriting
// classes to provide their own iterface.  This will ensure that any future  
// implementations of those classes will have any required methods exposed
// directly in that class.

class TreeBase {
  public:
    friend bool operator== (const TreeBase&, const TreeBase&);
    friend bool operator!= (const TreeBase&, const TreeBase&);
    friend bool operator== (const TreeBase&, const tree&);
    friend bool operator!= (const TreeBase&, const tree&);
    friend bool operator== (const tree&, const TreeBase&);
    friend bool operator!= (const tree&, const TreeBase&);
    operator bool() const { return Tree != NULL_TREE; }
    bool operator!() const { return Tree == NULL_TREE; }
  protected:
    tree Tree;
    inline TreeBase ()  { Tree = NULL; }
    inline TreeBase (const tree t) { Tree = t; }
    inline operator tree () const { return Tree; }
    inline TreeBase& operator= (const tree t) { Tree = t; return *this;}

    enum tree_code TreeCode () const;
    void SetTreeCode (enum tree_code);
};

class GimpleType : public TreeBase
{
  protected:
    inline void type_check() const 
			       { if (Tree) TREE_CLASS_CHECK (Tree, tcc_type); }
  public:
    inline GimpleType () : TreeBase () { }
    inline GimpleType (const tree t) : TreeBase (t) { type_check(); }
    inline operator tree () const { return Tree; }
    inline GimpleType& operator= (const tree t) 
				     { Tree = t; type_check (); return *this;}
    GimpleType type() const;
    enum tree_code code () const;
    bool pointer_type_p () const;
    bool type_unsigned () const;
    enum machine_mode type_mode () const;
};


class GimpleIdentifier : public TreeBase
{
  protected:
    inline void ident_check() const { if (Tree) IDENTIFIER_NODE_CHECK (Tree); }
  public:
    inline GimpleIdentifier () : TreeBase () { }
    inline GimpleIdentifier (const tree t) : TreeBase (t) { ident_check(); }
    inline operator tree () const { return Tree; }
    inline GimpleIdentifier& operator= (const tree t) 
				     { Tree = t; ident_check (); return *this;}
  
};

class GimpleValue : public TreeBase
{
  protected:
  public:
    inline GimpleValue () : TreeBase ()  { }
    inline GimpleValue (const tree t) : TreeBase (t) { }
    inline GimpleValue (const GimpleValue& v) : TreeBase (v.Tree) { }
    inline operator tree() const { return Tree; }
    inline GimpleValue& operator= (const tree t) 
				       { Tree = t; return *this;}
    inline GimpleValue& operator= (const GimpleValue& v) 
				       { Tree = v; return *this;}

    enum tree_code code () const;
    void set_code (enum tree_code);
    GimpleType type() const;
    void set_type (GimpleType);
};

class GimpleDecl : public GimpleValue
{
  protected:
    tree tree_type();
    inline void decl_check() const
			{ if (Tree) TREE_CLASS_CHECK (Tree, tcc_declaration); }
  
  public:
    inline GimpleDecl () : GimpleValue()  { }
    inline GimpleDecl (const tree t) : GimpleValue (t) { decl_check(); }
    inline GimpleDecl (const GimpleValue& v) : GimpleValue (v) { decl_check(); }
    inline GimpleDecl (const GimpleDecl& d) : GimpleValue (d) { }
    inline GimpleDecl& operator= (const tree t) 
		   { GimpleValue::operator= (t); decl_check (); return *this;}
    inline GimpleDecl& operator= (const GimpleValue& v) 
		   { GimpleValue::operator= (v); decl_check (); return *this;}
    inline GimpleDecl& operator= (const GimpleDecl& d) 
		   { GimpleValue::operator= (d); return *this;}

    GimpleIdentifier decl_name() const;
    GimpleDecl decl_abstract_origin () const;
    bool decl_from_inline () const;
    bool readonly () const;
    bool decl_ignored_p () const;
};

class SSADecl : public GimpleValue
{
  protected:
    inline void SSANameCheck () const { if (Tree) SSA_NAME_CHECK (Tree); }
  public:
    inline SSADecl () : GimpleValue () { }
    inline SSADecl (const tree t) : GimpleValue (t) { SSANameCheck(); }
    inline SSADecl (const GimpleValue& d) : GimpleValue (d) { SSANameCheck(); }
    inline SSADecl (const SSADecl& n) : GimpleValue (n) { }
    inline SSADecl& operator= (const tree t) 
	      { GimpleValue::operator= (t); SSANameCheck(); return *this; }
    inline SSADecl& operator= (const GimpleValue &d) 
	      { GimpleValue::operator= (d); SSANameCheck(); return *this; }
    inline SSADecl& operator= (const SSADecl &n) 
	      { GimpleValue::operator= (n); return *this; }

    unsigned int ssa_name_version () const;
    void set_ssa_name_version(unsigned int);
    GimpleDecl ssa_name_var () const ;
    void set_ssa_name_var (GimpleDecl);
    GimpleIdentifier ssa_name_identifier () const;
    void set_ssa_name_identifier (GimpleIdentifier);
    gimple ssa_name_def_stmt () const;
    void set_ssa_name_def_stmt (gimple);
    struct ptr_info_def *ssa_name_ptr_info () const;
    void set_ssa_name_ptr_info (struct ptr_info_def *);
    bool ssa_name_in_free_list () const;
    void set_ssa_name_in_free_list (bool);
    bool ssa_name_is_default_def () const;
    void set_ssa_name_is_default_def (bool);
    struct ssa_use_operand_d * ssa_name_imm_use_node_ptr () const;
    bool ssa_name_occurs_in_abnormal_phi () const;
    bool ssa_name_same_base (const SSADecl& n) const;
};



inline bool 
operator== (const TreeBase& t1, const tree& t2) 
{ return t1.Tree == t2; }

inline bool 
operator!= (const TreeBase& t1, const tree& t2) 
{ return t1.Tree != t2; }

inline bool 
operator== (const TreeBase& t1, const TreeBase& t2) 
{ return t1.Tree == t2.Tree; }

inline bool 
operator!= (const TreeBase& t1, const TreeBase& t2) 
{ return t1.Tree != t2.Tree; }

inline bool 
operator== (const tree& t1, const TreeBase& t2) 
{ return t1 == t2.Tree; }

inline bool 
operator!= (const tree& t1, const TreeBase& t2) 
{ return t1 != t2.Tree; }


#endif
