#ifndef TREE_ALIAS_TYPE_H
#define TREE_ALIAS_TYPE_H

#include "varray.h"

union alias_typevar_def;
struct aterm_;
struct aterm_list_a;
enum alias_typevar_kind
{
  ATERM_ATVAR
};
struct alias_typevar_common  GTY (())
{
  enum alias_typevar_kind kind;
  tree decl;
};
struct alias_typevar_aterm GTY (())
{
  struct alias_typevar_common common;
  struct aterm_ * GTY((skip (""))) term;
  struct aterm_list_a *GTY ((skip (""))) ptset;
};
union alias_typevar_def GTY ((desc ("%0.common.kind")))
{
  struct alias_typevar_common GTY ((tag ("-1"))) common;
  struct alias_typevar_aterm GTY ((tag ("ATERM_ATVAR"))) aterm;
};
typedef union alias_typevar_def *alias_typevar;

#define ALIAS_TVAR_KIND(x) ((x)->common.kind)
#define ALIAS_TVAR_DECL(x) ((x)->common.decl)
#define ALIAS_TVAR_ATERM(x) ((x)->aterm.term)
#define ALIAS_TVAR_PTSET(x) ((x)->aterm.ptset)
union alias_type_def;
typedef union alias_type_def *alias_type;

alias_typevar alias_tvar_new_with_aterm PARAMS ((tree, struct aterm_ *));
#endif
