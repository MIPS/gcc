#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "gimple.h"
#include "gimple-oacc.h"


bool
is_gimple_acc (const_gimple stmt)
{
  return is_a <const gimple_statement_acc> (stmt);
}


/* Set BODY to be for the ACC statement GS.  */
void
gimple_acc_set_body (gimple gs, gimple_seq body)
{
  gimple_statement_acc * acc = as_a <gimple_statement_acc> (gs);
  acc->body = body;
}

gimple_seq *
gimple_acc_body_ptr (gimple gs)
{
  gimple_statement_acc * acc = as_a <gimple_statement_acc> (gs);
  return &acc->body;
}

gimple_seq
gimple_acc_body (gimple gs)
{
  return *gimple_acc_body_ptr (gs);
}

unsigned
gimple_acc_nparams(const_gimple gs)
{
  const gimple_statement_acc * acc = as_a <const gimple_statement_acc> (gs);
  return acc->num_ops;
}


/******************************************************************************/


/* Return the clauses associated with ACC_KERNELS GS.  */

tree
gimple_acc_kernels_clauses (const_gimple gs)
{
  const gimple_statement_acc_kernels * kernels 
      = as_a <const gimple_statement_acc_kernels> (gs);
  return kernels->clauses;
}

/* Set CLAUSES to be associated with ACC_KERNELS GS.  */
void
gimple_acc_kernels_set_clauses (gimple gs, tree clauses)
{
  gimple_statement_acc_kernels * kernels 
      = as_a <gimple_statement_acc_kernels> (gs);
  kernels->clauses = clauses;
}

/* Return a pointer to the clauses associated with ACC_KERNELS GS.  */

tree *
gimple_acc_kernels_clauses_ptr (gimple gs)
{
  gimple_statement_acc_kernels * kernels 
      = as_a <gimple_statement_acc_kernels> (gs);
  return &kernels->clauses;
}

/* Return the pointer to child function of ACC_KERNELS GS.  */
tree
gimple_acc_kernels_child_fn (const_gimple gs)
{
  const gimple_statement_acc_kernels * kernels 
      = as_a <const gimple_statement_acc_kernels> (gs);
  return kernels->child_fn;
}

/* Return the child function of ACC_KERNELS GS.  */
tree *
gimple_acc_kernels_child_fn_ptr (gimple gs)
{
  gimple_statement_acc_kernels * kernels 
      = as_a <gimple_statement_acc_kernels> (gs);
  return &kernels->child_fn;
}

/* Set CHILD_FN for ACC_KERNELS GS.  */
void
gimple_acc_kernels_set_child_fn (gimple gs, tree child_fn)
{
  gimple_statement_acc_kernels * kernels 
      = as_a <gimple_statement_acc_kernels> (gs);
  kernels->child_fn = child_fn;
}

tree
gimple_acc_kernels_param(gimple gs, unsigned i)
{
  gimple_statement_acc_kernels * kernels 
      = as_a <gimple_statement_acc_kernels> (gs);
  gcc_gimple_checking_assert (i < kernels->num_ops);
  return kernels->op[i];
}

tree*
gimple_acc_kernels_params_ptr(gimple gs)
{
  gimple_statement_acc_kernels * kernels 
      = as_a <gimple_statement_acc_kernels> (gs);
  return kernels->op;
}

/******************************************************************************/


/* Return the clauses associated with ACC_PARALLEL GS.  */

tree
gimple_acc_parallel_clauses (const_gimple gs)
{
  const gimple_statement_acc_parallel * parallel 
      = as_a <const gimple_statement_acc_parallel> (gs);
  return parallel->clauses;
}


/* Return a pointer to the clauses associated with ACC_PARALLEL GS.  */

tree *
gimple_acc_parallel_clauses_ptr (gimple gs)
{
  gimple_statement_acc_parallel * parallel 
      = as_a <gimple_statement_acc_parallel> (gs);
  return &parallel->clauses;
}


/* Set CLAUSES to be the list of clauses associated with OMP_PARALLEL
   GS.  */

void
gimple_acc_parallel_set_clauses (gimple gs, tree clauses)
{
  gimple_statement_acc_parallel * parallel 
      = as_a <gimple_statement_acc_parallel> (gs);
  parallel->clauses = clauses;
}


/* Return the child function used to hold the body of OMP_PARALLEL GS.  */

tree
gimple_acc_parallel_child_fn (const_gimple gs)
{
  const gimple_statement_acc_parallel * parallel 
      = as_a <const gimple_statement_acc_parallel> (gs);
  return parallel->child_fn;
}

/* Return a pointer to the child function used to hold the body of
   ACC_PARALLEL GS.  */

tree *
gimple_acc_parallel_child_fn_ptr (gimple gs)
{
  gimple_statement_acc_parallel * parallel 
      = as_a <gimple_statement_acc_parallel> (gs);
  return &parallel->child_fn;
}


/* Set CHILD_FN to be the child function for OMP_PARALLEL GS.  */

void
gimple_acc_parallel_set_child_fn (gimple gs, tree child_fn)
{
  gimple_statement_acc_parallel * parallel 
      = as_a <gimple_statement_acc_parallel> (gs);
  parallel->child_fn = child_fn;
}

tree*
gimple_acc_parallel_params_ptr(gimple gs)
{
  gimple_statement_acc_parallel * parallel 
      = as_a <gimple_statement_acc_parallel> (gs);
  return parallel->op;
}

tree
gimple_acc_parallel_param(gimple gs, unsigned i)
{
  gimple_statement_acc_parallel * parallel 
      = as_a <gimple_statement_acc_parallel> (gs);
  gcc_gimple_checking_assert (i < parallel->num_ops);
  return parallel->op[i];
}

tree
gimple_acc_data_clauses (const_gimple gs)
{
  const gimple_statement_acc_data * data 
      = as_a <const gimple_statement_acc_data> (gs);
  return data->clauses;
}

void
gimple_acc_data_set_clauses (gimple gs, tree clauses)
{
  gimple_statement_acc_data * data 
      = as_a <gimple_statement_acc_data> (gs);
  data->clauses = clauses;
}


/******************************************************************************/

/* Return the child function of ACC_LOOP GS.  */
tree
gimple_acc_loop_child_fn (const_gimple gs)
{
  const gimple_statement_acc_loop * loop 
      = as_a <const gimple_statement_acc_loop> (gs);
  return loop->child_fn;
}

tree*
gimple_acc_loop_params_ptr(gimple gs)
{
  gimple_statement_acc_loop * loop 
      = as_a <gimple_statement_acc_loop> (gs);
  return loop->op;
}

tree
gimple_acc_loop_param(gimple gs, unsigned i)
{
  gimple_statement_acc_loop * loop 
      = as_a <gimple_statement_acc_loop> (gs);
  gcc_gimple_checking_assert (i < loop->num_ops);
  return loop->op[i];
}

/* Set CHILD_FN for ACC_LOOP GS.  */
void
gimple_acc_loop_set_child_fn (gimple gs, tree child_fn)
{
  gimple_statement_acc_loop * loop 
      = as_a <gimple_statement_acc_loop> (gs);
  loop->child_fn = child_fn;
}

/* Set CLAUSES to be associated with ACC_LOOP GS.  */
void
gimple_acc_loop_set_clauses (gimple gs, tree clauses)
{
  gimple_statement_acc_loop * loop 
      = as_a <gimple_statement_acc_loop> (gs);
  loop->clauses = clauses;
}

tree
gimple_acc_loop_clauses (const_gimple gs)
{
  const gimple_statement_acc_loop * loop 
      = as_a <const gimple_statement_acc_loop> (gs);
  return loop->clauses;
}

/* Set PRE_BODY to be associated with ACC_LOOP GS.  */
void
gimple_acc_loop_set_pre_body (gimple gs, gimple_seq pre_body)
{
  gimple_statement_acc_loop * loop 
      = as_a <gimple_statement_acc_loop> (gs);
  loop->pre_body = pre_body;
}

gimple_seq
gimple_acc_loop_pre_body (const_gimple gs)
{
  const gimple_statement_acc_loop * loop 
      = as_a <const gimple_statement_acc_loop> (gs);
  return loop->pre_body;
}

void
gimple_acc_loop_set_collapse (gimple gs, size_t collapse)
{
  gimple_statement_acc_loop * loop 
      = as_a <gimple_statement_acc_loop> (gs);
  loop->collapse = collapse;
}

size_t
gimple_acc_loop_collapse (const_gimple gs)
{
  const gimple_statement_acc_loop * loop 
      = as_a <const gimple_statement_acc_loop> (gs);
  return loop->collapse;
}

void
gimple_acc_loop_set_index (gimple gs, size_t i, tree index)
{
  gimple_statement_acc_loop * loop 
      = as_a <gimple_statement_acc_loop> (gs);
  gcc_gimple_checking_assert (i < loop->collapse);
  loop->iter[i].index = index;
}

tree
gimple_acc_loop_index (const_gimple gs, size_t i)
{
  const gimple_statement_acc_loop * loop 
      = as_a <const gimple_statement_acc_loop> (gs);
  gcc_gimple_checking_assert (i < loop->collapse);
  return loop->iter[i].index;
}

void
gimple_acc_loop_set_initial (gimple gs, size_t i, tree initial)
{
  gimple_statement_acc_loop * loop 
      = as_a <gimple_statement_acc_loop> (gs);
  gcc_gimple_checking_assert (i < loop->collapse);
  loop->iter[i].initial = initial;
}

tree
gimple_acc_loop_initial (const_gimple gs, size_t i)
{
  const gimple_statement_acc_loop * loop 
      = as_a <const gimple_statement_acc_loop> (gs);
  gcc_gimple_checking_assert (i < loop->collapse);
  return loop->iter[i].initial;
}

void
gimple_acc_loop_set_final (gimple gs, size_t i, tree final)
{
  gimple_statement_acc_loop * loop 
      = as_a <gimple_statement_acc_loop> (gs);
  gcc_gimple_checking_assert (i < loop->collapse);
  loop->iter[i].final = final;
}

tree
gimple_acc_loop_final (const_gimple gs, size_t i)
{
  const gimple_statement_acc_loop * loop 
      = as_a <const gimple_statement_acc_loop> (gs);
  gcc_gimple_checking_assert (i < loop->collapse);
  return loop->iter[i].final;
}

void
gimple_acc_loop_set_incr (gimple gs, size_t i, tree incr)
{
  gimple_statement_acc_loop * loop 
      = as_a <gimple_statement_acc_loop> (gs);
  gcc_gimple_checking_assert (i < loop->collapse);
  loop->iter[i].incr = incr;
}

tree
gimple_acc_loop_incr (const_gimple gs, size_t i)
{
  const gimple_statement_acc_loop * loop 
      = as_a <const gimple_statement_acc_loop> (gs);
  gcc_gimple_checking_assert (i < loop->collapse);
  return loop->iter[i].incr;
}

void
gimple_acc_loop_set_cond (gimple gs, size_t i, enum tree_code cond)
{
  gimple_statement_acc_loop * loop 
      = as_a <gimple_statement_acc_loop> (gs);
  gcc_gimple_checking_assert (i < loop->collapse);
  loop->iter[i].cond = cond;
}

enum tree_code
gimple_acc_loop_cond (const_gimple gs, size_t i)
{
  const gimple_statement_acc_loop * loop 
      = as_a <const gimple_statement_acc_loop> (gs);
  gcc_gimple_checking_assert (i < loop->collapse);
  return loop->iter[i].cond;
}

gimple
gimple_acc_data_region_end_statement (const_gimple gs)
{
  const gimple_statement_acc_region_end * region_end 
      = as_a <const gimple_statement_acc_region_end> (gs);
  return region_end->stmt;
}

void
gimple_acc_data_region_end_set_statement(gimple gs, gimple s)
{
  gimple_statement_acc_region_end * region_end 
      = as_a <gimple_statement_acc_region_end> (gs);
  region_end->stmt = s;
}

gimple
gimple_acc_compute_region_end_statement(const_gimple gs)
{
  const gimple_statement_acc_region_end * region_end 
      = as_a <const gimple_statement_acc_region_end> (gs);
  return region_end->stmt;
}

void
gimple_acc_compute_region_end_set_statement(gimple gs, gimple s)
{
  gimple_statement_acc_region_end * region_end 
      = as_a <gimple_statement_acc_region_end> (gs);
  region_end->stmt = s;
}

/******************************************************************************/
/********************* Begin of GIMPLE building routines **********************/

gimple
gimple_build_acc_parallel (gimple_seq body, tree clauses, tree child_fn,
                           tree data_arg)
{
  gimple p = gimple_alloc (GIMPLE_ACC_PARALLEL, 0);

  if (body)
    gimple_acc_set_body (p, body);

  gimple_acc_parallel_set_clauses (p, clauses);
  gimple_acc_parallel_set_child_fn (p, child_fn);

  return p;
}

gimple
gimple_build_acc_kernels (gimple_seq body, tree clauses, tree child_fn,
                          tree data_arg)
{
  gimple p = gimple_alloc (GIMPLE_ACC_KERNELS, 0);

  if (body)
  {
    gimple_acc_set_body (p, body);
  }

  gimple_acc_kernels_set_clauses (p, clauses);
  gimple_acc_kernels_set_child_fn (p, child_fn);

  return p;
}

gimple
gimple_build_acc_data (gimple_seq body, tree clauses, tree child_fn,
                       tree data_arg)
{
  gimple p = gimple_alloc (GIMPLE_ACC_DATA, 0);

  if (body)
    gimple_acc_set_body (p, body);

  gimple_acc_data_set_clauses (p, clauses);
  return p;
}

gimple
gimple_build_acc_cache (gimple_seq body, tree clauses, tree child_fn,
                        tree data_arg)
{
  gimple p = gimple_alloc (GIMPLE_ACC_CACHE, 0);

  if (body)
  {
    gimple_acc_set_body (p, body);
  }

  return p;
}

gimple
gimple_build_acc_wait (gimple_seq body, tree clauses, tree child_fn,
                       tree data_arg)
{
  gimple p = gimple_alloc (GIMPLE_ACC_WAIT, 0);

  if (body)
  {
    gimple_acc_set_body (p, body);
  }

  return p;
}

gimple
gimple_build_acc_host_data (gimple_seq body, tree clauses, tree child_fn,
                            tree data_arg)
{
  gimple p = gimple_alloc (GIMPLE_ACC_HOST_DATA, 0);

  if (body)
  {
    gimple_acc_set_body (p, body);
  }

  return p;
}

gimple
gimple_build_acc_loop (gimple_seq body, tree clauses, tree child_fn,
                       tree data_arg)
{
  gimple p = gimple_alloc (GIMPLE_ACC_LOOP, 0);

  if (body)
    gimple_acc_set_body (p, body);

  gimple_acc_loop_set_clauses (p, clauses);
  return p;
}

gimple
gimple_build_acc_declare (gimple_seq body, tree clauses, tree child_fn,
                          tree data_arg)
{
  gimple p = gimple_alloc (GIMPLE_ACC_DECLARE, 0);

  if (body)
  {
    gimple_acc_set_body (p, body);
  }

  return p;
}

gimple
gimple_build_acc_update (gimple_seq body, tree clauses, tree child_fn,
                         tree data_arg)
{
  gimple p = gimple_alloc (GIMPLE_ACC_UPDATE, 0);

  if (body)
  {
    gimple_acc_set_body (p, body);
  }

  return p;
}

gimple
gimple_build_acc_compute_region_end ()
{
  gimple p = gimple_alloc (GIMPLE_ACC_COMPUTE_REGION_END, 0);

  return p;
}

gimple
gimple_build_acc_data_region_end ()
{
  gimple p = gimple_alloc (GIMPLE_ACC_DATA_REGION_END, 0);

  return p;
}

/********************** End of GIMPLE building routines ***********************/
/******************************************************************************/
