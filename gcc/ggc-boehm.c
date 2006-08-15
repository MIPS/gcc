#include "config.h"
#include "system.h"
#include "hashtab.h"
#include "options.h"
#include "params.h"
#include "timevar.h"
#include "ggc.h"
#include "symtab.h"

/* #define GC_DEBUG */
#include <gc.h>

extern struct ht *ident_hash;

static int ggc_htab_register_weak_ptr(void **slot, void *info);
static int ggc_htab_unregister_weak_ptr(void **slot, void *info);
static void ggc_htab_delete_weak_ptr(void ** slot, void * object,
				     void * info);

static size_t get_used_heap_size(void);
static void register_gty_roots(void);
static void gc_warning_filter(char * msg, GC_word arg);

static enum gt_types_enum *get_type_offset(void * block);
static enum gt_types_enum get_block_type(void * block);

static void register_weak_pointers(void);
static void unregister_weak_pointers(void);

static size_t last_allocated = 0;
static size_t type_overhead = 0;
static ggc_stringpool_roots stringpool_roots;

static GC_warn_proc default_warn_proc;

#define OBJ_OVERHEAD sizeof(enum gt_types_enum)

void
init_ggc (void)
{
  /* Have better idea of roots before initialization, because it performs
     blacklisting according to the current set of roots.  We miss stringpool
     roots here, but we can get information about them only when it's way too
     late. */
  register_gty_roots();

  GC_init();
  GC_disable(); /* Do not collect on allocation */

  stringpool_roots.start = NULL;
  stringpool_roots.one_after_finish = NULL;

  default_warn_proc = GC_set_warn_proc(gc_warning_filter);
}

void *
ggc_alloc_stat (size_t size MEM_STAT_DECL)
{
  void * result = GC_MALLOC(size);
  return result;
}

enum gt_types_enum *
get_type_offset(void * block)
{
  return (enum gt_types_enum *)((char *)block + GC_size(block) - OBJ_OVERHEAD);
}

void *
ggc_alloc_typed_stat (enum gt_types_enum type, size_t size MEM_STAT_DECL)
{
  void * result = NULL;

  size_t actual_obj_size;
  const size_t obj_plus_info_size = size + OBJ_OVERHEAD;
  type_overhead += obj_plus_info_size;

  result = GC_malloc (obj_plus_info_size);
  actual_obj_size = GC_size(result);

  *(get_type_offset(result)) = type;

  /* Verification */
  gcc_assert (type == get_block_type (result));

  return result;
}

void *ggc_alloc_cleared_typed_stat (enum gt_types_enum type,
				    size_t size MEM_STAT_DECL)
{
  void * result = ggc_alloc_typed_stat(type, size);
  memset (result, 0, size);
  return result;
}

enum gt_types_enum
get_block_type(void * block)
{
  return *(get_type_offset(block));
}

void *
ggc_realloc_stat (void *x, size_t size MEM_STAT_DECL)
{
  void * result = GC_REALLOC(x, size);
  return result;
}

void
ggc_htab_delete_weak_ptr(void ** slot, void * object ATTRIBUTE_UNUSED,
			 void * info)
{
  const struct ggc_cache_tab *r = (const struct ggc_cache_tab *) info;
  htab_clear_slot (*r->base, slot);
}

int
ggc_htab_register_weak_ptr(void **slot, void *info)
{
  GC_general_register_disappearing_link_callback (slot, *slot,
						  ggc_htab_delete_weak_ptr,
						  info);
  return 1;
}

int
ggc_htab_unregister_weak_ptr(void **slot, void *info ATTRIBUTE_UNUSED)
{
  int result = GC_unregister_disappearing_link (slot);
  gcc_assert (result != 0);
  return 1;
}

void
register_weak_pointers(void)
{
  const struct ggc_cache_tab *const *ct;
  const struct ggc_cache_tab *cti;

  /* Register hash caches as weak pointers. Boehm's GC weak pointer facility
     will clear any weak pointers to deleted objects.  After collection hash
     table cleanup to shrink it should be performed. */
  for (ct = gt_ggc_cache_rtab; *ct; ct++)
    for (cti = *ct; cti->base != NULL; cti++)
      if (*cti->base)
	{
	  htab_traverse_noresize (*cti->base, ggc_htab_register_weak_ptr,
				  (void *)cti);
	}
}

void
unregister_weak_pointers(void)
{
  const struct ggc_cache_tab *const *ct;
  const struct ggc_cache_tab *cti;

  for (ct = gt_ggc_cache_rtab; *ct; ct++)
    for (cti = *ct; cti->base != NULL; cti++)
      if (*cti->base)
	{
	  htab_traverse_noresize (*cti->base, ggc_htab_unregister_weak_ptr,
				  (void *)cti);
	}
}


void
ggc_collect (void)
{
  const struct ggc_root_tab *const *rt;
  const struct ggc_root_tab *rti;

  /* Avoid frequent unnecessary work by skipping collection if the
     total allocations haven't expanded much since the last
     collection.  */
  float allocated_last_gc =
    MAX (last_allocated, (size_t)PARAM_VALUE (GGC_MIN_HEAPSIZE) * 1024);

  float min_expand = allocated_last_gc * PARAM_VALUE (GGC_MIN_EXPAND) / 100;

  if (get_used_heap_size() < allocated_last_gc + min_expand
      && !ggc_force_collect)
    return;

  timevar_push (TV_GC);
  if (!quiet_flag)
    fprintf (stderr, " {GC %luk -> ",
	     (unsigned long) get_used_heap_size() / 1024);

  if (!stringpool_roots.start)
    stringpool_roots = ggc_register_stringpool_roots();
  else if (ggc_stringpool_moved_p(stringpool_roots))
    {
      ggc_unregister_stringpool_roots(stringpool_roots);
      stringpool_roots = ggc_register_stringpool_roots();
    }

  /* Clear pointers with GTY "deletable" */
  for (rt = gt_ggc_deletable_rtab; *rt; rt++)
    for (rti = *rt; rti->base != NULL; rti++)
      memset (rti->base, 0, rti->stride);

  register_weak_pointers();

  GC_enable();
  GC_gcollect();
  GC_disable();

  unregister_weak_pointers();

  if (!quiet_flag)
    fprintf (stderr, "%luk}", (unsigned long) get_used_heap_size() / 1024);
  last_allocated = get_used_heap_size();
  timevar_pop (TV_GC);
}

void
ggc_free (void * block)
{
  GC_FREE(block); /* For some blocks might be unprofitable? */
}

size_t
ggc_get_size (const void * block)
{
  return GC_size((void *)block) - OBJ_OVERHEAD; /* Note that GC_size may return
						   a bit larger value than
						   originally requested */
}

int
ggc_marked_p (const void * d ATTRIBUTE_UNUSED)
{
  abort();
}

char *
ggc_pch_alloc_object (struct ggc_pch_data * d ATTRIBUTE_UNUSED,
		      void * p ATTRIBUTE_UNUSED, size_t s ATTRIBUTE_UNUSED,
		      bool b ATTRIBUTE_UNUSED,
		      enum gt_types_enum t ATTRIBUTE_UNUSED)
{
  abort();
}

void
ggc_pch_count_object (struct ggc_pch_data * d ATTRIBUTE_UNUSED,
		      void * p ATTRIBUTE_UNUSED, size_t s ATTRIBUTE_UNUSED,
		      bool b ATTRIBUTE_UNUSED,
		      enum gt_types_enum t ATTRIBUTE_UNUSED)
{
  abort();
}

void
ggc_pch_finish (struct ggc_pch_data * d ATTRIBUTE_UNUSED,
		FILE * f ATTRIBUTE_UNUSED)
{
  abort();
}

void
ggc_pch_read (FILE * f ATTRIBUTE_UNUSED, void * p ATTRIBUTE_UNUSED)
{
  abort();
}

void
ggc_pch_this_base (struct ggc_pch_data * d ATTRIBUTE_UNUSED,
		   void * p ATTRIBUTE_UNUSED)
{
  abort();
}

void
ggc_pch_prepare_write (struct ggc_pch_data * d ATTRIBUTE_UNUSED,
		       FILE * f ATTRIBUTE_UNUSED)
{
  abort();
}

size_t
ggc_pch_total_size (struct ggc_pch_data * d ATTRIBUTE_UNUSED)
{
  abort();
}

void
ggc_pch_write_object (struct ggc_pch_data * d ATTRIBUTE_UNUSED,
		      FILE * f ATTRIBUTE_UNUSED, void * p1 ATTRIBUTE_UNUSED,
		      void * p2 ATTRIBUTE_UNUSED, size_t s ATTRIBUTE_UNUSED,
		      bool b ATTRIBUTE_UNUSED)
{
  abort();
}

void
ggc_print_statistics (void)
{
  struct ggc_statistics stats;
  memset (&stats, 0, sizeof(stats));

  last_allocated = 0;

  ggc_print_common_statistics (stderr, &stats);

  fprintf (stderr,
	   "\nMemory still allocated at the end of the compilation process\n");
  fprintf (stderr,
	   "Total heap size: %lu\n", (unsigned long)GC_get_heap_size());
  fprintf (stderr,
	   "Free bytes in the heap: %lu\n",
	   (unsigned long)GC_get_free_bytes());
  fprintf (stderr,
	   "Used bytes in the heap: %lu\n",
	   (unsigned long)get_used_heap_size());
  fprintf (stderr,
	   "Local ggc-boehm overhead: %lu\n",
	   (unsigned long)type_overhead);
}

int
ggc_set_mark (const void * block ATTRIBUTE_UNUSED)
{
  abort();
}

struct ggc_pch_data *
init_ggc_pch (void)
{
  abort();
}


size_t
get_used_heap_size(void)
{
  return GC_get_heap_size() - GC_get_free_bytes();
}

void
register_gty_roots(void)
{
  const struct ggc_root_tab *const *rt;
  const struct ggc_root_tab *rti;

  /* Register ordinary GC roots */
  for (rt = gt_ggc_rtab; *rt; rt++)
    for (rti = *rt; rti->base != NULL; rti++)
      GC_add_roots((char *)rti->base, (char *)rti->base + rti->stride);
}

/* Register the stringpool entries as GGC roots.  In contrast to all other
   roots, that are static, stringpool may increase and move around in memory.
   So it's handled specially. */
ggc_stringpool_roots
ggc_register_stringpool_roots (void)
{
  ggc_stringpool_roots result;
  result.start = ident_hash->entries;
  result.one_after_finish = ident_hash->entries + ident_hash->nslots;

  GC_add_roots (result.start, result.one_after_finish);

  return result;
}

void
ggc_unregister_stringpool_roots (ggc_stringpool_roots roots)
{
  GC_remove_roots (roots.start, roots.one_after_finish);
}

int
ggc_stringpool_moved_p (ggc_stringpool_roots roots)
{
  return (roots.start != ident_hash->entries)
    || (roots.one_after_finish != ident_hash->entries + ident_hash->nslots);
}

void
gc_warning_filter(char * msg, GC_word arg)
{
  if (!quiet_flag)
    (*default_warn_proc)(msg, arg);
}
