/* Data required for function inlining.  */
typedef struct inline_data
{
  /* FUNCTION_DECL for function being inlined.  */
  tree callee;
  /* FUNCTION_DECL for function being inlined into.  */
  tree caller;
  /* struct function for function being inlined.  Usually this is the same
     as DECL_STRUCT_FUNCTION (callee), but can be different if saved_cfg
     and saved_eh are in use.  */
  struct function *callee_cfun;
  /* The VAR_DECL for the return value.  */
  tree retvar;
  /* The map from local declarations in the inlined function to
     equivalents in the function into which it is being inlined.  */
  splay_tree decl_map;
  /* We use the same mechanism to build clones that we do to perform
     inlining.  However, there are a few places where we need to
     distinguish between those two situations.  This flag is true if
     we are cloning, rather than inlining.  */
  bool cloning_p;
  /* Similarly for saving function body.  */
  bool saving_p;
  /* Versioning function is slightly different from inlining. */
  bool versioning_p;
  /* Copy basic blocks in the same method only. */
  bool copybb_only_p;
  /* Callgraph node of function we are inlining into.  */
  struct cgraph_node *node;
  /* Callgraph node of currently inlined function.  */
  struct cgraph_node *current_node;
  /* Current BLOCK.  */
  tree block;
  varray_type ipa_info;
  /* Exception region the inlined call lie in.  */
  int eh_region;
  /* Take region number in the function being copied, add this value and
     get eh region number of the duplicate in the function we inline into.  */
  int eh_region_offset;
} inline_data;

/* Duplicate code.  */
basic_block copy_bb (inline_data *, basic_block, int, int);
void copy_edges_for_bb (basic_block, int);

