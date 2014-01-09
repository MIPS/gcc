#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "gimple.h"
#include "gimple-oacc.h"


bool
is_gimple_oacc (const_gimple stmt)
{
  return is_a <const gimple_statement_oacc> (stmt);
}


/* Set BODY to be for the ACC statement GS.  */
void
gimple_oacc_set_body (gimple gs, gimple_seq body)
{
  gimple_statement_oacc * acc = as_a <gimple_statement_oacc> (gs);
  acc->body = body;
}

gimple_seq *
gimple_oacc_body_ptr (gimple gs)
{
  gimple_statement_oacc * acc = as_a <gimple_statement_oacc> (gs);
  return &acc->body;
}

gimple_seq
gimple_oacc_body (gimple gs)
{
  return *gimple_oacc_body_ptr (gs);
}

unsigned
gimple_oacc_nparams(const_gimple gs)
{
  const gimple_statement_oacc * acc = as_a <const gimple_statement_oacc> (gs);
  return acc->num_ops;
}


/******************************************************************************/


/* Return the clauses associated with OACC_KERNELS GS.  */

tree
gimple_oacc_kernels_clauses (const_gimple gs)
{
  const gimple_statement_oacc_kernels * kernels 
      = as_a <const gimple_statement_oacc_kernels> (gs);
  return kernels->clauses;
}

/* Set CLAUSES to be associated with OACC_KERNELS GS.  */
void
gimple_oacc_kernels_set_clauses (gimple gs, tree clauses)
{
  gimple_statement_oacc_kernels * kernels 
      = as_a <gimple_statement_oacc_kernels> (gs);
  kernels->clauses = clauses;
}

/* Return a pointer to the clauses associated with OACC_KERNELS GS.  */

tree *
gimple_oacc_kernels_clauses_ptr (gimple gs)
{
  gimple_statement_oacc_kernels * kernels 
      = as_a <gimple_statement_oacc_kernels> (gs);
  return &kernels->clauses;
}

/* Return the pointer to child function of OACC_KERNELS GS.  */
tree
gimple_oacc_kernels_child_fn (const_gimple gs)
{
  const gimple_statement_oacc_kernels * kernels 
      = as_a <const gimple_statement_oacc_kernels> (gs);
  return kernels->child_fn;
}

/* Return the child function of OACC_KERNELS GS.  */
tree *
gimple_oacc_kernels_child_fn_ptr (gimple gs)
{
  gimple_statement_oacc_kernels * kernels 
      = as_a <gimple_statement_oacc_kernels> (gs);
  return &kernels->child_fn;
}

/* Set CHILD_FN for OACC_KERNELS GS.  */
void
gimple_oacc_kernels_set_child_fn (gimple gs, tree child_fn)
{
  gimple_statement_oacc_kernels * kernels 
      = as_a <gimple_statement_oacc_kernels> (gs);
  kernels->child_fn = child_fn;
}

tree
gimple_oacc_kernels_param(gimple gs, unsigned i)
{
  gimple_statement_oacc_kernels * kernels 
      = as_a <gimple_statement_oacc_kernels> (gs);
  gcc_gimple_checking_assert (i < kernels->num_ops);
  return kernels->op[i];
}

tree*
gimple_oacc_kernels_params_ptr(gimple gs)
{
  gimple_statement_oacc_kernels * kernels 
      = as_a <gimple_statement_oacc_kernels> (gs);
  return kernels->op;
}

/******************************************************************************/


/* Return the clauses associated with OACC_PARALLEL GS.  */

tree
gimple_oacc_parallel_clauses (const_gimple gs)
{
  const gimple_statement_oacc_parallel * parallel 
      = as_a <const gimple_statement_oacc_parallel> (gs);
  return parallel->clauses;
}


/* Return a pointer to the clauses associated with OACC_PARALLEL GS.  */

tree *
gimple_oacc_parallel_clauses_ptr (gimple gs)
{
  gimple_statement_oacc_parallel * parallel 
      = as_a <gimple_statement_oacc_parallel> (gs);
  return &parallel->clauses;
}


/* Set CLAUSES to be the list of clauses associated with OMP_PARALLEL
   GS.  */

void
gimple_oacc_parallel_set_clauses (gimple gs, tree clauses)
{
  gimple_statement_oacc_parallel * parallel 
      = as_a <gimple_statement_oacc_parallel> (gs);
  parallel->clauses = clauses;
}


/* Return the child function used to hold the body of OMP_PARALLEL GS.  */

tree
gimple_oacc_parallel_child_fn (const_gimple gs)
{
  const gimple_statement_oacc_parallel * parallel 
      = as_a <const gimple_statement_oacc_parallel> (gs);
  return parallel->child_fn;
}

/* Return a pointer to the child function used to hold the body of
   OACC_PARALLEL GS.  */

tree *
gimple_oacc_parallel_child_fn_ptr (gimple gs)
{
  gimple_statement_oacc_parallel * parallel 
      = as_a <gimple_statement_oacc_parallel> (gs);
  return &parallel->child_fn;
}


/* Set CHILD_FN to be the child function for OMP_PARALLEL GS.  */

void
gimple_oacc_parallel_set_child_fn (gimple gs, tree child_fn)
{
  gimple_statement_oacc_parallel * parallel 
      = as_a <gimple_statement_oacc_parallel> (gs);
  parallel->child_fn = child_fn;
}

tree*
gimple_oacc_parallel_params_ptr(gimple gs)
{
  gimple_statement_oacc_parallel * parallel 
      = as_a <gimple_statement_oacc_parallel> (gs);
  return parallel->op;
}

tree
gimple_oacc_parallel_param(gimple gs, unsigned i)
{
  gimple_statement_oacc_parallel * parallel 
      = as_a <gimple_statement_oacc_parallel> (gs);
  gcc_gimple_checking_assert (i < parallel->num_ops);
  return parallel->op[i];
}

tree
gimple_oacc_data_clauses (const_gimple gs)
{
  const gimple_statement_oacc_data * data 
      = as_a <const gimple_statement_oacc_data> (gs);
  return data->clauses;
}

void
gimple_oacc_data_set_clauses (gimple gs, tree clauses)
{
  gimple_statement_oacc_data * data 
      = as_a <gimple_statement_oacc_data> (gs);
  data->clauses = clauses;
}


/******************************************************************************/

/* Return the child function of OACC_LOOP GS.  */
tree
gimple_oacc_loop_child_fn (const_gimple gs)
{
  const gimple_statement_oacc_loop * loop 
      = as_a <const gimple_statement_oacc_loop> (gs);
  return loop->child_fn;
}

tree*
gimple_oacc_loop_params_ptr(gimple gs)
{
  gimple_statement_oacc_loop * loop 
      = as_a <gimple_statement_oacc_loop> (gs);
  return loop->op;
}

tree
gimple_oacc_loop_param(gimple gs, unsigned i)
{
  gimple_statement_oacc_loop * loop 
      = as_a <gimple_statement_oacc_loop> (gs);
  gcc_gimple_checking_assert (i < loop->num_ops);
  return loop->op[i];
}

/* Set CHILD_FN for OACC_LOOP GS.  */
void
gimple_oacc_loop_set_child_fn (gimple gs, tree child_fn)
{
  gimple_statement_oacc_loop * loop 
      = as_a <gimple_statement_oacc_loop> (gs);
  loop->child_fn = child_fn;
}

/* Set CLAUSES to be associated with OACC_LOOP GS.  */
void
gimple_oacc_loop_set_clauses (gimple gs, tree clauses)
{
  gimple_statement_oacc_loop * loop 
      = as_a <gimple_statement_oacc_loop> (gs);
  loop->clauses = clauses;
}

tree
gimple_oacc_loop_clauses (const_gimple gs)
{
  const gimple_statement_oacc_loop * loop 
      = as_a <const gimple_statement_oacc_loop> (gs);
  return loop->clauses;
}

/* Set PRE_BODY to be associated with OACC_LOOP GS.  */
void
gimple_oacc_loop_set_pre_body (gimple gs, gimple_seq pre_body)
{
  gimple_statement_oacc_loop * loop 
      = as_a <gimple_statement_oacc_loop> (gs);
  loop->pre_body = pre_body;
}

gimple_seq
gimple_oacc_loop_pre_body (const_gimple gs)
{
  const gimple_statement_oacc_loop * loop 
      = as_a <const gimple_statement_oacc_loop> (gs);
  return loop->pre_body;
}

void
gimple_oacc_loop_set_collapse (gimple gs, size_t collapse)
{
  gimple_statement_oacc_loop * loop 
      = as_a <gimple_statement_oacc_loop> (gs);
  loop->collapse = collapse;
}

size_t
gimple_oacc_loop_collapse (const_gimple gs)
{
  const gimple_statement_oacc_loop * loop 
      = as_a <const gimple_statement_oacc_loop> (gs);
  return loop->collapse;
}

void
gimple_oacc_loop_set_index (gimple gs, size_t i, tree index)
{
  gimple_statement_oacc_loop * loop 
      = as_a <gimple_statement_oacc_loop> (gs);
  gcc_gimple_checking_assert (i < loop->collapse);
  loop->iter[i].index = index;
}

tree
gimple_oacc_loop_index (const_gimple gs, size_t i)
{
  const gimple_statement_oacc_loop * loop 
      = as_a <const gimple_statement_oacc_loop> (gs);
  gcc_gimple_checking_assert (i < loop->collapse);
  return loop->iter[i].index;
}

void
gimple_oacc_loop_set_initial (gimple gs, size_t i, tree initial)
{
  gimple_statement_oacc_loop * loop 
      = as_a <gimple_statement_oacc_loop> (gs);
  gcc_gimple_checking_assert (i < loop->collapse);
  loop->iter[i].initial = initial;
}

tree
gimple_oacc_loop_initial (const_gimple gs, size_t i)
{
  const gimple_statement_oacc_loop * loop 
      = as_a <const gimple_statement_oacc_loop> (gs);
  gcc_gimple_checking_assert (i < loop->collapse);
  return loop->iter[i].initial;
}

void
gimple_oacc_loop_set_final (gimple gs, size_t i, tree final)
{
  gimple_statement_oacc_loop * loop 
      = as_a <gimple_statement_oacc_loop> (gs);
  gcc_gimple_checking_assert (i < loop->collapse);
  loop->iter[i].final = final;
}

tree
gimple_oacc_loop_final (const_gimple gs, size_t i)
{
  const gimple_statement_oacc_loop * loop 
      = as_a <const gimple_statement_oacc_loop> (gs);
  gcc_gimple_checking_assert (i < loop->collapse);
  return loop->iter[i].final;
}

void
gimple_oacc_loop_set_incr (gimple gs, size_t i, tree incr)
{
  gimple_statement_oacc_loop * loop 
      = as_a <gimple_statement_oacc_loop> (gs);
  gcc_gimple_checking_assert (i < loop->collapse);
  loop->iter[i].incr = incr;
}

tree
gimple_oacc_loop_incr (const_gimple gs, size_t i)
{
  const gimple_statement_oacc_loop * loop 
      = as_a <const gimple_statement_oacc_loop> (gs);
  gcc_gimple_checking_assert (i < loop->collapse);
  return loop->iter[i].incr;
}

void
gimple_oacc_loop_set_cond (gimple gs, size_t i, enum tree_code cond)
{
  gimple_statement_oacc_loop * loop 
      = as_a <gimple_statement_oacc_loop> (gs);
  gcc_gimple_checking_assert (i < loop->collapse);
  loop->iter[i].cond = cond;
}

enum tree_code
gimple_oacc_loop_cond (const_gimple gs, size_t i)
{
  const gimple_statement_oacc_loop * loop 
      = as_a <const gimple_statement_oacc_loop> (gs);
  gcc_gimple_checking_assert (i < loop->collapse);
  return loop->iter[i].cond;
}

gimple
gimple_oacc_data_region_end_statement (const_gimple gs)
{
  const gimple_statement_oacc_region_end * region_end 
      = as_a <const gimple_statement_oacc_region_end> (gs);
  return region_end->stmt;
}

void
gimple_oacc_data_region_end_set_statement(gimple gs, gimple s)
{
  gimple_statement_oacc_region_end * region_end 
      = as_a <gimple_statement_oacc_region_end> (gs);
  region_end->stmt = s;
}

gimple
gimple_oacc_compute_region_end_statement(const_gimple gs)
{
  const gimple_statement_oacc_region_end * region_end 
      = as_a <const gimple_statement_oacc_region_end> (gs);
  return region_end->stmt;
}

void
gimple_oacc_compute_region_end_set_statement(gimple gs, gimple s)
{
  gimple_statement_oacc_region_end * region_end 
      = as_a <gimple_statement_oacc_region_end> (gs);
  region_end->stmt = s;
}

/******************************************************************************/
/********************* Begin of GIMPLE building routines **********************/

gimple
gimple_build_oacc_parallel (gimple_seq body, tree clauses, tree child_fn,
                           tree data_arg)
{
  gimple p = gimple_alloc (GIMPLE_OACC_PARALLEL, 0);

  if (body)
    gimple_oacc_set_body (p, body);

  gimple_oacc_parallel_set_clauses (p, clauses);
  gimple_oacc_parallel_set_child_fn (p, child_fn);

  return p;
}

gimple
gimple_build_oacc_kernels (gimple_seq body, tree clauses, tree child_fn,
                          tree data_arg)
{
  gimple p = gimple_alloc (GIMPLE_OACC_KERNELS, 0);

  if (body)
  {
    gimple_oacc_set_body (p, body);
  }

  gimple_oacc_kernels_set_clauses (p, clauses);
  gimple_oacc_kernels_set_child_fn (p, child_fn);

  return p;
}

gimple
gimple_build_oacc_data (gimple_seq body, tree clauses, tree child_fn,
                       tree data_arg)
{
  gimple p = gimple_alloc (GIMPLE_OACC_DATA, 0);

  if (body)
    gimple_oacc_set_body (p, body);

  gimple_oacc_data_set_clauses (p, clauses);
  return p;
}

gimple
gimple_build_oacc_cache (gimple_seq body, tree clauses, tree child_fn,
                        tree data_arg)
{
  gimple p = gimple_alloc (GIMPLE_OACC_CACHE, 0);

  if (body)
  {
    gimple_oacc_set_body (p, body);
  }

  return p;
}

gimple
gimple_build_oacc_wait (gimple_seq body, tree clauses, tree child_fn,
                       tree data_arg)
{
  gimple p = gimple_alloc (GIMPLE_OACC_WAIT, 0);

  if (body)
  {
    gimple_oacc_set_body (p, body);
  }

  return p;
}

gimple
gimple_build_oacc_host_data (gimple_seq body, tree clauses, tree child_fn,
                            tree data_arg)
{
  gimple p = gimple_alloc (GIMPLE_OACC_HOST_DATA, 0);

  if (body)
  {
    gimple_oacc_set_body (p, body);
  }

  return p;
}

gimple
gimple_build_oacc_loop (gimple_seq body, tree clauses, tree child_fn,
                       tree data_arg)
{
  gimple p = gimple_alloc (GIMPLE_OACC_LOOP, 0);

  if (body)
    gimple_oacc_set_body (p, body);

  gimple_oacc_loop_set_clauses (p, clauses);
  return p;
}

gimple
gimple_build_oacc_declare (gimple_seq body, tree clauses, tree child_fn,
                          tree data_arg)
{
  gimple p = gimple_alloc (GIMPLE_OACC_DECLARE, 0);

  if (body)
  {
    gimple_oacc_set_body (p, body);
  }

  return p;
}

gimple
gimple_build_oacc_update (gimple_seq body, tree clauses, tree child_fn,
                         tree data_arg)
{
  gimple p = gimple_alloc (GIMPLE_OACC_UPDATE, 0);

  if (body)
  {
    gimple_oacc_set_body (p, body);
  }

  return p;
}

gimple
gimple_build_oacc_compute_region_end ()
{
  gimple p = gimple_alloc (GIMPLE_OACC_COMPUTE_REGION_END, 0);

  return p;
}

gimple
gimple_build_oacc_data_region_end ()
{
  gimple p = gimple_alloc (GIMPLE_OACC_DATA_REGION_END, 0);

  return p;
}

/********************** End of GIMPLE building routines ***********************/
/******************************************************************************/
