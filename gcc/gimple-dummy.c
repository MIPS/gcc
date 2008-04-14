/* FIXME tuples.  HACK.  HACK.  HACK.

   This file defines a collection symbols that have been if0'd out
   during the conversion to GIMPLE tuples.  As the various passes get
   re-enabled, these symbols should be removed.  */

#include "config.h"
#include "system.h"

#define DUMMY_VAR(X)				\
  int X

#define DUMMY_FN(X)				\
  void X (void);				\
  void X (void) { gcc_unreachable (); }

DUMMY_VAR (memory_identifier_string);


DUMMY_FN (canonicalize_induction_variables)
DUMMY_FN (get_type)
DUMMY_FN (ipa_add_method)
DUMMY_FN (ipa_callsite_callee)
DUMMY_FN (ipa_callsite_compute_count)
DUMMY_FN (ipa_callsite_compute_param)
DUMMY_FN (ipa_callsite_param)
DUMMY_FN (ipa_callsite_param_count)
DUMMY_FN (ipa_callsite_param_count_set)
DUMMY_FN (ipa_edges_create)
DUMMY_FN (ipa_edges_free)
DUMMY_FN (ipa_free)
DUMMY_FN (ipa_jf_get_info_type)
DUMMY_FN (ipa_method_compute_modify)
DUMMY_FN (ipa_method_compute_tree_map)
DUMMY_FN (ipa_method_formal_compute_count)
DUMMY_FN (ipa_method_formal_count)
DUMMY_FN (ipa_method_formal_count_set)
DUMMY_FN (ipa_method_get_tree)
DUMMY_FN (ipa_method_modify_print)
DUMMY_FN (ipa_method_tree_print)
DUMMY_FN (ipa_methodlist_init)
DUMMY_FN (ipa_methodlist_not_empty)
DUMMY_FN (ipa_node_create)
DUMMY_FN (ipa_nodes_create)
DUMMY_FN (ipa_nodes_free)
DUMMY_FN (ipa_remove_method)
DUMMY_FN (remove_empty_loops)
DUMMY_FN (tree_ssa_unswitch_loops)
DUMMY_FN (tree_unroll_loops_completely)
DUMMY_FN (vec_set_verbosity_level)
DUMMY_FN (vect_set_verbosity_level)
DUMMY_FN (vectorize_loops)
DUMMY_FN (remove_iv)
DUMMY_FN (diagnose_omp_structured_block_errors)
DUMMY_FN (lower_omp_parallel)
DUMMY_FN (lower_omp_for)
DUMMY_FN (lower_omp_sections)
DUMMY_FN (lower_omp_single)
DUMMY_FN (lower_omp_master)
DUMMY_FN (lower_omp_ordered)
DUMMY_FN (lower_omp_critical)


/* Note that we should mark gimple_unreachable_1 with
   ATTRIBUTE_NORETURN.  But the function will sometimes return, so we
   need to turn off the return warning in the Makefiles for now.

   We also manually declare flag_gimple_conversion to avoid having to
   pull in many standard header files.  The whole setup here is a
   giant hack, so this needs to be as self-contained as possible.  */
extern void gimple_unreachable_1 (const char *, int, const char *);
extern int flag_gimple_conversion;

void
gimple_unreachable_1 (const char *file, int line, const char *fn)
{
  if (flag_gimple_conversion == 0)
    return;

  fprintf (stderr, "%s:%s(%d): GIMPLE conversion ", file, fn, line);

  if (flag_gimple_conversion == 1)
    fprintf (stderr, "warning: Executing unconverted code\n");
  else
    {
      fprintf (stderr, "error: Executing unconverted code\n");
      gcc_unreachable ();
    }
}
