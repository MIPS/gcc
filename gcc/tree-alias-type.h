#ifndef TREE_ALIAS_TYPE_H
#define TREE_ALIAS_TYPE_H

struct ECR_def;
typedef struct ECR_def *ECR;

struct alias_typevar_def  GTY (())
{
  tree decl;
  ECR ecr;
};

typedef struct alias_typevar_def *alias_typevar;

union alias_type_def;
typedef union alias_type_def *alias_type;

alias_typevar alias_tvar_new_with_at PARAMS ((tree, alias_type));
alias_typevar alias_tvar_new PARAMS ((tree));
alias_typevar alias_tvar_new_equiv_to PARAMS ((tree, alias_typevar));
ECR alias_tvar_get_ECR PARAMS ((alias_typevar));
ECR alias_tvar_get_orig_ECR PARAMS ((alias_typevar));
bool alias_tvar_is_alias PARAMS ((alias_typevar));
varray_type alias_tvar_pointsto PARAMS ((alias_typevar));
void alias_tvar_allpointsto PARAMS ((alias_typevar, varray_type *));

enum alias_types
{
  BOTTOM_ATYPE,
  VALUE_ATYPE,
  LOCATION_ATYPE,
  FUNCTION_ATYPE
};
typedef varray_type (*pointsto_func) PARAMS ((alias_type));
typedef void (*unify_func) PARAMS ((alias_type, alias_type));

struct alias_type_common  GTY (())
{
  enum alias_types kind;
  unsigned int id;
  pointsto_func pointsto;
  unify_func unify;
};
/*
  This struct represents Steensgaard's alpha type, the non-standard
  type describing values. */
struct alias_value_type  GTY (())
{
  struct alias_type_common common;
  ECR location;
  ECR function;
};
/* 
   This struct represents what Steensgaard refers to these as tau
   types, the non-standard types describing locations. A location is
   either  bottom or a value.  So this type is  representative of a
   pointer to another tau type (location_type), or a lambda type
   (function_type).  
   
   In theory, one could get away with just having a pointer to the
   alpha type (value_type). However, the indirection this would add to
   the routines isn't worth the one extra pointer it costs to store
   the alpha type's fields directly.
*/


struct alias_location_type  GTY (())
{
  struct alias_type_common common;
  ECR location;
  ECR function;
};
/* This struct implements what Steensgaard refers to as lambda types,
   the non-standard type describing functions.  */
struct alias_function_type  GTY (())
{
  struct alias_type_common common;
  alias_type retval;
  varray_type GTY ((param_is (union alias_type_def))) arguments;
};
union alias_type_def GTY ((desc ("%0.common.kind")))
{
  struct alias_type_common  GTY ((tag ("BOTTOM_ATYPE"))) common;
  struct alias_value_type  GTY ((tag ("VALUE_ATYPE"))) value;
  struct alias_location_type GTY ((tag ("LOCATION_ATYPE"))) location;
  struct alias_function_type  GTY ((tag ("FUNCTION_ATYPE"))) function;
};

#define ALIAS_TYPE_KIND(x) ((x)->common.kind)
#define ALIAS_TYPE_ID(x) ((x)->common.id)
#define SET_ALIAS_TYPE_UNIFY(x, y) ((x)->common.unify = (y))
#define SET_ALIAS_TYPE_POINTSTO(x, y) ((x)->common.pointsto = (y))
#define ALIAS_TYPE_UNIFY(x, y) ((x)->common.unify((x), (y)))
#define ALIAS_TYPE_POINTSTO(x) ((x)->common.pointsto((x)))

#define ALIAS_VTYPE_FUNC(x) ((x)->value.function)
#define ALIAS_VTYPE_LOC(x) ((x)->value.location)

#define ALIAS_LTYPE_FUNC(x) ((x)->location.function)
#define ALIAS_LTYPE_LOC(x)  ((x)->location.location)

#define ALIAS_FTYPE_ARGUMENTS(x) ((x)->function.arguments)
#define ALIAS_FTYPE_RETVAL(x) ((x)->function.retval)

extern alias_type alias_bottom;

alias_type alias_vtype_new PARAMS ((void));
alias_type alias_vtype_new_with_lf PARAMS ((ECR, ECR));
ECR alias_vtype_loc PARAMS ((alias_type));
ECR alias_vtype_func PARAMS ((alias_type));

alias_type alias_ltype_new PARAMS ((void));
alias_type alias_ltype_new_with_lf PARAMS ((ECR, ECR));
ECR alias_ltype_loc PARAMS ((alias_type));
ECR alias_ltype_func PARAMS ((alias_type));

alias_type alias_ftype_new PARAMS ((void));
void alias_ftype_add_argument PARAMS ((alias_type, alias_type));
void alias_ftype_add_new_arguments PARAMS ((alias_type, int));

void init_alias_type PARAMS ((void));
#endif
