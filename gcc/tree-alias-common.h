#ifndef TREE_ALIAS_COMMON
#define TREE_ALIAS_COMMON
#include "tree-alias-type.h"
/**
   @brief Alias analysis function pointers.
   
   Functions implemented by the actual alias analysis algorithms in
   order for them to work with the common points-to structure.
*/
struct tree_alias_ops
{
  /**
     @brief Initialization

     Called right before we start using the other functions.
  */
  void (*init) (struct tree_alias_ops *);

  /**
     @brief Cleanup
     
     Called when we are finished with the alias analyzer.
  */
  void (*cleanup) (struct tree_alias_ops *);
  
  /**
     @brief Add variable
     
     Called when we want to inform the alias analyzer about a new
     variable we've found.
  */
  alias_typevar (*add_var) (struct tree_alias_ops *, tree);
  
  /**
     @brief Add variable equivalent to existing one.

     Called when we want to inform the alias analyzer about a new
     variable that has the same points-to set as an existing variable.
  */
  alias_typevar (*add_var_same) (struct tree_alias_ops *, tree,
			   alias_typevar);
  /**
     @brief Process a simple assignment (<tt>a = b</tt>)
     
     Called to process simple assignment statements of the form <tt>a =
     b</tt>, where a and b are both variables.
  */
  void (*simple_assign) (struct tree_alias_ops *, alias_typevar,
			 alias_typevar);
  /**
     @brief Process an address assignment (<tt>a = &b</tt>)
     
     Called to process address assignment statements of the form <tt>a =
     &b</tt>, where a and b are both variables.
  */
  void (*addr_assign) (struct tree_alias_ops *, alias_typevar, alias_typevar);

  /**
     @brief Process a pointer assignment (<tt>a = *b</tt>)

     Called to process pointer assignment statements of the form <tt>a =
     *b</tt>, where a and b are both variables.
  */
  void (*ptr_assign) (struct tree_alias_ops *, alias_typevar, alias_typevar);

  /**
     @brief Process an operator assignment (<tt>a = op (...)</tt>)
     
     Called to process operators of the form <tt>a = op(....)</tt>, where a is
     a variable.
  */
  void (*op_assign) (struct tree_alias_ops *, alias_typevar, varray_type);

  /**
     @brief Process a heap assignment (<tt>a = alloc (...)</tt>)
     
     Called to process a heap assignment of the form <tt>a = *alloc
     (...)</tt>, where a is a variable, and *alloc is a function that
     returns new memory. 
  */
  void (*heap_assign) (struct tree_alias_ops *, alias_typevar);

  /**
     @brief Process an assignment to a pointer (<tt>*a = b</tt>)
     
     Called to process assignment to pointer statements of the form
     <tt>*a = b</tt>, where a and b are both variables.
  */
  void (*assign_ptr) (struct tree_alias_ops *, alias_typevar, alias_typevar);

  /**
     @brief Process a function definition
     
     Called to inform the alias analyzer about a new function
     definition.
  */
  void (*function_def) (struct tree_alias_ops *, alias_typevar,
			varray_type, alias_typevar);

  /**
     @brief Process a function call
     @return Return 1 if we need to assume conservative side-effects.
     
  */
  int (*function_call) (struct tree_alias_ops *, alias_typevar,
			alias_typevar, varray_type);
  
  /**
     @brief Determine if two typevars may alias
  */
  bool (*may_alias) (struct tree_alias_ops *, alias_typevar, alias_typevar);

  /**
     @brief Private data
  */
  void *data;
  int ip:1; /*Interprocedural */
  int ip_partial:1; /* Can do conservative interprocedural
		       analysis if we save the info.*/

};

extern struct tree_alias_ops *current_alias_ops;
extern void create_alias_vars PARAMS ((tree));
extern void delete_alias_vars PARAMS ((void));
extern void init_alias_vars PARAMS ((void));
extern bool ptr_may_alias_var PARAMS ((tree, tree));

enum pta_type
  {
    PTA_NONE,
    PTA_ANDERSEN
  };
extern enum pta_type flag_tree_points_to;
extern const char *alias_get_name PARAMS ((tree));
#endif
