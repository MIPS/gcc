/* Miscellaneous utilities.  Things that are used in both input and output
   or do not belong clearly to either are put here.

   Copyright 2008 Free Software Foundation, Inc.
   Contributed by Doug Kwan <dougkwan@google.com>

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to
the Free Software Foundation, 51 Franklin Street, Fifth Floor,
Boston, MA 02110-1301, USA.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "gimple.h"
#include "bitmap.h"
#include "vec.h"
#include "lto-header.h"
#include "lto-utils.h"

/* Statistics gathered during LTO, WPA and LTRANS.  */
struct lto_stats_d lto_stats;

/* LTO uses bitmaps with different life-times.  So use a seperate
   obstack for all LTO bitmaps.  */

static bitmap_obstack lto_obstack;
static bool lto_obstack_initialized;

/* We use this vector to keep extra DECL flags used by various parts of LTO.
   Vector is indexed by DECL_UID.  */

DEF_VEC_I(lto_decl_flags_t);
DEF_VEC_ALLOC_I(lto_decl_flags_t,heap);
static VEC(lto_decl_flags_t,heap) *lto_decl_flags;

/* Allocate a bitmap from heap.  Initializes the LTO obstack if necessary.  */

bitmap
lto_bitmap_alloc (void)
{
  if (!lto_obstack_initialized)
    {
      bitmap_obstack_initialize (&lto_obstack);
      lto_obstack_initialized = true;
    }
  return BITMAP_ALLOC (&lto_obstack);
}

/* Free bitmap B.  */

void
lto_bitmap_free (bitmap b)
{
  BITMAP_FREE (b);
}

/* Get the var-flags of VAR.  */

lto_decl_flags_t
lto_get_decl_flags (tree var)
{
  unsigned idx = DECL_UID (var);
  if (!lto_decl_flags || VEC_length (lto_decl_flags_t, lto_decl_flags) <= idx)
    return 0;
  return VEC_index (lto_decl_flags_t, lto_decl_flags, idx);
}

/* Set the var-flags of VAR to FLAGS.  */

void
lto_set_decl_flags (tree var, lto_decl_flags_t flags)
{
  unsigned idx = DECL_UID (var);
  if (!lto_decl_flags || VEC_length (lto_decl_flags_t, lto_decl_flags) <= idx)
    VEC_safe_grow_cleared (lto_decl_flags_t, heap, lto_decl_flags, idx + 1);
  VEC_replace (lto_decl_flags_t, lto_decl_flags, idx, flags);
}

/* Get a section name for a particular type or name.  The NAME field
   is only used if SECTION_TYPE is LTO_section_function_body or
   lto_static_initializer.  For all others it is ignored.  The callee
   of this function is responcible to free the returned name.  */

char *
lto_get_section_name (int section_type, const char *name)
{
  switch (section_type)
    {
    case LTO_section_function_body:
      return concat (LTO_SECTION_NAME_PREFIX, name, NULL);

    case LTO_section_static_initializer:
      return concat (LTO_SECTION_NAME_PREFIX, ".statics", NULL);

    case LTO_section_symtab:
      return concat (LTO_SECTION_NAME_PREFIX, ".symtab", NULL);

    case LTO_section_decls:
      return concat (LTO_SECTION_NAME_PREFIX, ".decls", NULL);

    case LTO_section_cgraph:
      return concat (LTO_SECTION_NAME_PREFIX, ".cgraph", NULL);

    case LTO_section_ipa_pure_const:
      return concat (LTO_SECTION_NAME_PREFIX, ".pureconst", NULL);

    case LTO_section_ipa_reference:
      return concat (LTO_SECTION_NAME_PREFIX, ".reference", NULL);

    case LTO_section_wpa_fixup:
      return concat (LTO_SECTION_NAME_PREFIX, ".wpa_fixup", NULL);

    case LTO_section_opts:
      return concat (LTO_SECTION_NAME_PREFIX, ".opts", NULL);

    default:
      gcc_unreachable ();
    }
}


/* Show various memory usage statistics related to LTO.  */

void
print_lto_report (void)
{
  const char *s = (flag_lto) ? "LTO" : (flag_wpa) ? "WPA" : "LTRANS";
  unsigned i;

  fprintf (stderr, "%s statistics\n", s);
  fprintf (stderr, "[%s] # of input files: "
	   HOST_WIDE_INT_PRINT_UNSIGNED "\n", s, lto_stats.num_input_files);

  fprintf (stderr, "[%s] # of input cgraph nodes: " 
	   HOST_WIDE_INT_PRINT_UNSIGNED "\n", s,
	   lto_stats.num_input_cgraph_nodes);

  fprintf (stderr, "[%s] # of function bodies: "
	   HOST_WIDE_INT_PRINT_UNSIGNED "\n", s,
	   lto_stats.num_function_bodies);

  fprintf (stderr, "[%s] ", s);
  print_gimple_types_stats ();

  for (i = 0; i < NUM_TREE_CODES; i++)
    if (lto_stats.num_trees[i])
      fprintf (stderr, "[%s] # of '%s' objects read: "
	       HOST_WIDE_INT_PRINT_UNSIGNED "\n", s,
	       tree_code_name[i], lto_stats.num_trees[i]);

  if (flag_lto)
    {
      fprintf (stderr, "[%s] Compression: "
	       HOST_WIDE_INT_PRINT_UNSIGNED " output bytes, "
	       HOST_WIDE_INT_PRINT_UNSIGNED " compressed bytes", s,
	       lto_stats.num_output_il_bytes,
	       lto_stats.num_compressed_il_bytes);
      if (lto_stats.num_output_il_bytes > 0)
	{
	  const float dividend = (float) lto_stats.num_compressed_il_bytes;
	  const float divisor = (float) lto_stats.num_output_il_bytes;
	  fprintf (stderr, " (ratio: %f)", dividend / divisor);
	}
      fprintf (stderr, "\n");
    }

  if (flag_wpa)
    {
      fprintf (stderr, "[%s] # of output files: "
	       HOST_WIDE_INT_PRINT_UNSIGNED "\n", s,
	       lto_stats.num_output_files);

      fprintf (stderr, "[%s] # of output cgraph nodes: "
	       HOST_WIDE_INT_PRINT_UNSIGNED "\n", s,
	       lto_stats.num_output_cgraph_nodes);

      fprintf (stderr, "[%s] # callgraph partitions: "
	       HOST_WIDE_INT_PRINT_UNSIGNED "\n", s,
	       lto_stats.num_cgraph_partitions);

      fprintf (stderr, "[%s] Compression: "
	       HOST_WIDE_INT_PRINT_UNSIGNED " input bytes, "
	       HOST_WIDE_INT_PRINT_UNSIGNED " uncompressed bytes", s,
	       lto_stats.num_input_il_bytes,
	       lto_stats.num_uncompressed_il_bytes);
      if (lto_stats.num_input_il_bytes > 0)
	{
	  const float dividend = (float) lto_stats.num_uncompressed_il_bytes;
	  const float divisor = (float) lto_stats.num_input_il_bytes;
	  fprintf (stderr, " (ratio: %f)", dividend / divisor);
	}
      fprintf (stderr, "\n");
    }

  for (i = 0; i < LTO_N_SECTION_TYPES; i++)
    fprintf (stderr, "[%s] Size of mmap'd section %s: "
	     HOST_WIDE_INT_PRINT_UNSIGNED " bytes\n", s,
	     lto_section_name[i], lto_stats.section_size[i]);
}
