#ifndef DISJOINT_SET_H
#define DISJOINT_SET_H
#include "varray.h"


struct disjoint_set_def GTY ((ptr_alias (struct ECR_def))) 
{
  struct disjoint_set_def *parent;
  int rank;
  /* FIXME: Yes, I know this sucks, and makes it non-reusable, 
     we'll deal later. */
  varray_type GTY ((param_is (struct ECR_def))) elements;
};
typedef struct disjoint_set_def *disjoint_set;

disjoint_set disjoint_set_new PARAMS ((void));
disjoint_set disjoint_set_union PARAMS ((disjoint_set, disjoint_set));
disjoint_set disjoint_set_find PARAMS ((disjoint_set));
bool disjoint_set_is_rep PARAMS ((disjoint_set));
int disjoint_set_size PARAMS ((disjoint_set));
varray_type disjoint_set_elems PARAMS ((disjoint_set));
bool disjoint_set_equiv PARAMS ((disjoint_set, disjoint_set));
void disjoint_set_merge_elems PARAMS ((disjoint_set, disjoint_set));
#endif
