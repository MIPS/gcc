#include "config.h"
#include "system.h"
#include "options.h"
#include "params.h"
#include "timevar.h"
#include "ggc.h"

#include <gc.h>

static void register_gty_roots(void);

static size_t last_allocated = 0;

void
init_ggc (void)
{
  GC_init();
  GC_disable(); /* Do not collect on allocation */
  register_gty_roots();
}

void *
ggc_alloc_stat (size_t size MEM_STAT_DECL)
{
  return GC_MALLOC(size);
}

void
ggc_collect (void)
{
  /* Avoid frequent unnecessary work by skipping collection if the
     total allocations haven't expanded much since the last
     collection.  */
  float allocated_last_gc =
    MAX (last_allocated, (size_t)PARAM_VALUE (GGC_MIN_HEAPSIZE) * 1024);

  float min_expand = allocated_last_gc * PARAM_VALUE (GGC_MIN_EXPAND) / 100;

  if (GC_get_heap_size() < allocated_last_gc + min_expand
      && !ggc_force_collect)
    return;

  timevar_push (TV_GC);
  if (!quiet_flag)
    fprintf (stderr, " {GC %luk -> ",
	     (unsigned long) GC_get_heap_size() / 1024);
  GC_enable();
  GC_gcollect();
  GC_disable();
  if (!quiet_flag)
    fprintf (stderr, "%luk}", (unsigned long) GC_get_heap_size() / 1024);
  last_allocated = GC_get_heap_size();
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
  return GC_size(block); /* Note that GC_size may return a bit larger value
			    than originally requested */
}

int
ggc_marked_p (const void * d)
{
  abort();
}

char *
ggc_pch_alloc_object (struct ggc_pch_data * d, void * p, size_t s, bool b,
		      enum gt_types_enum t)
{
  abort();
}

void
ggc_pch_count_object (struct ggc_pch_data * d, void * p, size_t s, bool b,
		      enum gt_types_enum t)
{
  abort();
}

void
ggc_pch_finish (struct ggc_pch_data * d, FILE * f)
{
  abort();
}

void
ggc_pch_read (FILE * f, void * p)
{
  abort();
}

void
ggc_pch_this_base (struct ggc_pch_data * d, void * p)
{
  abort();
}

void
ggc_pch_prepare_write (struct ggc_pch_data * d, FILE * f)
{
  abort();
}

size_t
ggc_pch_total_size (struct ggc_pch_data * d)
{
  abort();
}

void
ggc_pch_write_object (struct ggc_pch_data * d, FILE * f, void * p1, void * p2,
		      size_t s, bool b)
{
  abort();
}

void
ggc_print_statistics (void)
{
  abort();
}

int
ggc_set_mark (const void * block)
{
  abort();
}

struct ggc_pch_data *
init_ggc_pch (void)
{
  abort();
}

void
register_gty_roots(void)
{
  const struct ggc_root_tab *const *rt;
  const struct ggc_root_tab *rti;

  for (rt = gt_ggc_rtab; *rt; rt++)
    for (rti = *rt; rti->base != NULL; rti++)
      GC_add_roots(rti->base, rti->base + rti->stride);

  /* TODO: it might be required to process gt_ggc_cache_rtab here */
}
