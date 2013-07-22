
class gimple_combine
{
 public:
  gimple_combine(bool reas = false)
     : allow_full_reassiocation_(reas)
  {}

  tree build (location_t, enum tree_code, tree, tree);
  tree build (location_t, enum tree_code, tree, tree, tree);
  tree build (location_t, enum tree_code, tree, tree, tree, tree);

  tree combine (gimple);
 protected:

  /* The default hook just returns all as nonzero.  */
  virtual double_int nonzerobitsf(tree) { return double_int_minus_one; }
  /* The default hook just returns the variable. */
  virtual tree valueizerv(tree var) { return var; }

 private:
  bool allow_full_reassiocation_;


  tree binary (location_t, enum tree_code, tree, tree, tree);
  tree unary (location_t, enum tree_code, tree, tree);
  tree ternary (location_t, enum tree_code, tree, tree, tree, tree);
  tree convert (location_t loc, tree type, tree arg)
  {
    return build (loc, NOP_EXPR, type, arg);
  }
  tree negate_expr (location_t loc, tree type, tree arg)
  {
    return build (loc, NEGATE_EXPR, type, arg);
  }

  tree valueizerf (tree val);
  double_int nonzerobits (tree val);
  bool signbit_zero (tree expr);
  void defcodefor_name_3 (tree, enum tree_code *, tree *, tree *,
			  tree *);
  bool truth_valued_ssa_name (tree);
  tree comparison (location_t, enum tree_code,
		   tree, tree, tree);
  tree gimple_cond (gimple stmt);
  tree conds (location_t, enum tree_code,
	      tree, tree, tree, tree);
  tree comparisons (location_t, enum tree_code, enum tree_code,
		    enum tree_code, tree,
		    tree, tree);
  tree not_neg_abs_expr (location_t, enum tree_code,
			 tree, tree);
  tree bitwise_binary_1 (location_t, enum tree_code, tree,
			 tree, tree);
  tree cond_expr (location_t, enum tree_code,
		  tree, tree, tree, tree);
  tree switch_stmt (gimple);
  tree cond_stmt (gimple);
  tree lookup_logical_inverted_value (tree);
  tree comparisons_cst (location_t, enum tree_code, tree,
			enum tree_code, enum tree_code,
			tree, tree, tree);
  tree bitwise_binary (location_t, enum tree_code, tree,
		       tree, tree);
  tree shift_rotate (location_t, enum tree_code, tree, tree, tree);
  tree view_convert (location_t, enum tree_code, tree, tree);
  tree references (location_t, tree, tree);
  tree mult_expr (location_t, enum tree_code, tree, tree, tree);
  tree call_stmt (location_t, tree, tree, tree *, unsigned, bool);
  tree conversions (location_t, enum tree_code, tree, tree);
  tree addr_ref_difference (location_t, tree, tree, tree);
  tree constructor (tree);
  tree plusminus (location_t, enum tree_code, tree, tree, tree);
  tree complex_expr (tree, tree);
  tree real_imag_parts (enum tree_code, tree);
  tree pointer_plus (location_t, enum tree_code, tree, tree, tree);
  tree minmax_expr (enum tree_code, tree, tree);
  tree mod_expr (location_t, enum tree_code, tree, tree, tree);
  tree addr_expr (tree);
};

bool replace_rhs_after_ssa_combine (gimple_stmt_iterator *, tree);
