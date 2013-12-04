#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "gimple.h"
#include "gimple-oacc.h"


bool
is_gimple_acc (const_gimple stmt)
{
  switch (gimple_code (stmt))
  {
  CASE_GIMPLE_ACC:
    return true;
  default:
    return false;
  }
}


/* Set BODY to be for the ACC statement GS.  */
void
gimple_acc_set_body (gimple gs, gimple_seq body)
{
  gs->acc.body = body;
}

gimple_seq *
gimple_acc_body_ptr (gimple gs)
{
  return &gs->acc.body;
}

gimple_seq
gimple_acc_body (gimple gs)
{
  return *gimple_acc_body_ptr (gs);
}

unsigned
gimple_acc_nparams(const_gimple gs)
{
  return gs->acc.membase.opbase.gsbase.num_ops;
}


/******************************************************************************/


/* Return the clauses associated with ACC_KERNELS GS.  */

tree
gimple_acc_kernels_clauses (const_gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_ACC_KERNELS);
  return gs->gimple_acc_kernels.clauses;
}

/* Set CLAUSES to be associated with ACC_KERNELS GS.  */
void
gimple_acc_kernels_set_clauses (gimple gs, tree clauses)
{
  GIMPLE_CHECK (gs, GIMPLE_ACC_KERNELS);
  gs->gimple_acc_kernels.clauses = clauses;
}

/* Return a pointer to the clauses associated with ACC_KERNELS GS.  */

tree *
gimple_acc_kernels_clauses_ptr (gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_ACC_KERNELS);
  return &gs->gimple_acc_kernels.clauses;
}

/* Return the pointer to child function of ACC_KERNELS GS.  */
tree
gimple_acc_kernels_child_fn (const_gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_ACC_KERNELS);
  return gs->gimple_acc_kernels.child_fn;
}

/* Return the child function of ACC_KERNELS GS.  */
tree *
gimple_acc_kernels_child_fn_ptr (gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_ACC_KERNELS);
  return &gs->gimple_acc_kernels.child_fn;
}

/* Set CHILD_FN for ACC_KERNELS GS.  */
void
gimple_acc_kernels_set_child_fn (gimple gs, tree child_fn)
{
  GIMPLE_CHECK (gs, GIMPLE_ACC_KERNELS);
  gs->gimple_acc_kernels.child_fn = child_fn;
}

tree
gimple_acc_kernels_param(gimple gs, unsigned i)
{
  GIMPLE_CHECK (gs, GIMPLE_ACC_KERNELS);
  gcc_gimple_checking_assert (i < gs->acc.membase.opbase.gsbase.num_ops);
  return gs->gimple_acc_kernels.op[i];
}

tree*
gimple_acc_kernels_params_ptr(gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_ACC_KERNELS);
  return gs->gimple_acc_kernels.op;
}

/******************************************************************************/


/* Return the clauses associated with ACC_PARALLEL GS.  */

tree
gimple_acc_parallel_clauses (const_gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_ACC_PARALLEL);
  return gs->gimple_acc_parallel.clauses;
}


/* Return a pointer to the clauses associated with ACC_PARALLEL GS.  */

tree *
gimple_acc_parallel_clauses_ptr (gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_ACC_PARALLEL);
  return &gs->gimple_acc_parallel.clauses;
}


/* Set CLAUSES to be the list of clauses associated with OMP_PARALLEL
   GS.  */

void
gimple_acc_parallel_set_clauses (gimple gs, tree clauses)
{
  GIMPLE_CHECK (gs, GIMPLE_ACC_PARALLEL);
  gs->gimple_acc_parallel.clauses = clauses;
}


/* Return the child function used to hold the body of OMP_PARALLEL GS.  */

tree
gimple_acc_parallel_child_fn (const_gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_ACC_PARALLEL);
  return gs->gimple_acc_parallel.child_fn;
}

/* Return a pointer to the child function used to hold the body of
   ACC_PARALLEL GS.  */

tree *
gimple_acc_parallel_child_fn_ptr (gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_ACC_PARALLEL);
  return &gs->gimple_acc_parallel.child_fn;
}


/* Set CHILD_FN to be the child function for OMP_PARALLEL GS.  */

void
gimple_acc_parallel_set_child_fn (gimple gs, tree child_fn)
{
  GIMPLE_CHECK (gs, GIMPLE_ACC_PARALLEL);
  gs->gimple_acc_parallel.child_fn = child_fn;
}

tree*
gimple_acc_parallel_params_ptr(gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_ACC_PARALLEL);
  return gs->gimple_acc_parallel.op;
}

tree
gimple_acc_parallel_param(gimple gs, unsigned i)
{
  GIMPLE_CHECK (gs, GIMPLE_ACC_PARALLEL);
  gcc_gimple_checking_assert (i < gs->acc.membase.opbase.gsbase.num_ops);
  return gs->gimple_acc_parallel.op[i];
}

tree
gimple_acc_data_clauses (const_gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_ACC_DATA);
  return gs->gimple_acc_data.clauses;
}

void
gimple_acc_data_set_clauses (gimple gs, tree clauses)
{
  GIMPLE_CHECK (gs, GIMPLE_ACC_DATA);
  gs->gimple_acc_data.clauses = clauses;
}


/******************************************************************************/

/* Return the child function of ACC_LOOP GS.  */
tree
gimple_acc_loop_child_fn (const_gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_ACC_LOOP);
  return gs->gimple_acc_loop.child_fn;
}

tree*
gimple_acc_loop_params_ptr(gimple gs)
{
  GIMPLE_CHECK (gs, GIMPLE_ACC_LOOP);
  return gs->gimple_acc_loop.op;
}

tree
gimple_acc_loop_param(gimple gs, unsigned i)
{
  GIMPLE_CHECK (gs, GIMPLE_ACC_LOOP);
  gcc_gimple_checking_assert (i < gs->acc.membase.opbase.gsbase.num_ops);
  return gs->gimple_acc_loop.op[i];
}

/* Set CHILD_FN for ACC_LOOP GS.  */
void
gimple_acc_loop_set_child_fn (gimple gs, tree child_fn)
{
  GIMPLE_CHECK (gs, GIMPLE_ACC_LOOP);
  gs->gimple_acc_loop.child_fn = child_fn;
}

/******************************************************************************/
/********************* Begin of GIMPLE building routines **********************/

gimple
gimple_build_acc_parallel (gimple_seq body, tree clauses, tree child_fn,
                           tree data_arg)
{
  gimple p = gimple_alloc (GIMPLE_ACC_PARALLEL, 0);

  if (body)
  {
    gimple_acc_set_body (p, body);
  }

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
  {
    gimple_acc_set_body (p, body);
  }

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
  {
    gimple_acc_set_body (p, body);
  }

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
