#ifndef TREE_ALIAS_ECR_H_
#define TREE_ALIAS_ECR_H_
#include "bitmap.h"
#include "disjoint-set.h"
#include "tree-alias-type.h"

struct ECR_def  GTY (())
{
  struct disjoint_set_def set;
  alias_type type;
  bitmap  pending;
  unsigned int color;
  unsigned int id;
  alias_typevar var;
};

ECR ECR_new PARAMS ((void));
ECR ECR_new_with_type PARAMS ((alias_type, alias_typevar));
alias_type ECR_get_type PARAMS ((ECR));
#define ECR_get_id(x)  ((x)->id)
int ECR_get_setid PARAMS ((ECR));
#define ECR_get_typevar(x) ((x)->var)
void ECR_union_pending_sets PARAMS ((ECR, ECR, ECR));
void ECR_cjoin PARAMS ((ECR, ECR));
void ECR_join PARAMS ((ECR, ECR));
void ECR_set_type PARAMS ((ECR, alias_type));
#define ECR_find(x) ((ECR) disjoint_set_find ((disjoint_set)(x)))
#define ECR_union(x,y) ((ECR) disjoint_set_union ((disjoint_set)(x), (disjoint_set)(y)))
#define ECR_equiv(x,y) (disjoint_set_equiv ((disjoint_set)(x), (disjoint_set)(y)))
#define ECR_size(x) (disjoint_set_size ((disjoint_set)(x)))
#define ECR_elems(x) (disjoint_set_elems ((disjoint_set)(x)))

#endif
