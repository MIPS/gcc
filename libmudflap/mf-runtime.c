/* {{{ Copyright */

/* Mudflap: narrow-pointer bounds-checking by tree rewriting.
   Copyright (C) 2001, 2002 Free Software Foundation, Inc.
   Contributed by Frank Ch. Eigler <fche@redhat.com>

This file is part of GCC.

XXX: libgcc license?
*/

/* }}} */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <execinfo.h>
#include <assert.h>

#include "mf-runtime.h"


#ifdef _MUDFLAP
#error "Do not compile this file with -fmudflap!"
#endif

/* ------------------------------------------------------------------------ */
/* {{{ Utility macros */
#define UNLIKELY(e) (__builtin_expect (!!(e), 0))
#define LIKELY(e) (__builtin_expect (!!(e), 1))

#define MINPTR ((uintptr_t) 0)
#define MAXPTR (~ (uintptr_t) 0)
/* Clamp the addition/subtraction of uintptr_t's to [MINPTR,MAXPTR] */
#define CLAMPSUB(ptr,offset) ((ptr) >= (offset) ? (ptr)-(offset) : MINPTR)
#define CLAMPADD(ptr,offset) ((ptr) <= MAXPTR-(offset) ? (ptr)+(offset) : MAXPTR)
#define CLAMPSZ(ptr,size) ((ptr) <= MAXPTR-(size)+1 ? (ptr)+(size)-1 : MAXPTR)

/* }}} */
/* ------------------------------------------------------------------------ */
/* {{{ Configuration macros */

/*
  Mode selection.  Exactly one of the following mode macros should
  be defined:
   
  MODE_NOP    - make check/register/unregister each a nop
  MODE_NOSIR  - make check/register/unregister always trigger a violation
  MODE_YESSIR - make check/register/unregister a nop; populate lookup cache
  MODE_CHECK  - activate check/register/unregister
*/

#if (MODE_NOP + MODE_NOSIR + MODE_YESSIR + MODE_CHECK) != 1
#error "Incorrect mf-runtime MODE selected."
#endif


/*
  Violation handling.  Exactly one of the following should be defined:

  VIOLATION_NOP  - return control to application
  VIOLATION_SEGV - signal a segv  (XXX: not implemented)
  VIOLATION_CORE - abort()
  VIOLATION_GDB  - fork a debugger on self  (XXX: not implemented)
*/
#if (VIOLATION_NOP + VIOLATION_SEGV + VIOLATION_CORE + VIOLATION_GDB) != 1
#error "Incorrect mf-runtime VIOLATION selected."
#endif



/*
  Option selection.  Any of these macros may be defined:
  
  OPT_TRACE   - emit a trace message for each call
  OPT_STATS   - collect statistics
  OPT_FREEQ   - use a queue of <N> buffers for deferring user free() calls
  OPT_PERSIST - keep history of <N> __mf_unregister'd objects of each class
  OPT_BKTRACE - store <N>-level stack backtrace for better warning messages
  OPT_INIT    - emit ctor/dtor __mf_init/_fini
  OPT_THREADS - support multithreaded applications (XXX: not implemented)
  OPT_INTCHK  - include mudflap internal checks
  OPT_LEAK    - print a list of leaked heap objects on shutdown
  OPT_HELPFUL - give verbose help at violation
  OPT_TREEOPT - periodically rotate the object database tree for optimization
*/

/* }}} */
/* ------------------------------------------------------------------------ */
/* {{{ Required globals.  */

#ifndef LOOKUP_CACHE_MASK_DFL
#define LOOKUP_CACHE_MASK_DFL 1023
#endif
#ifndef LOOKUP_CACHE_SIZE_MAX
#define LOOKUP_CACHE_SIZE_MAX 4096 /* Allows max CACHE_MASK 0x0FFF */
#endif
#ifndef LOOKUP_CACHE_SHIFT_DFL
#define LOOKUP_CACHE_SHIFT_DFL 2
#endif

struct __mf_cache __mf_lookup_cache [LOOKUP_CACHE_SIZE_MAX];
uintptr_t __mf_lc_mask = LOOKUP_CACHE_MASK_DFL;
unsigned char __mf_lc_shift = LOOKUP_CACHE_SHIFT_DFL;
#define LOOKUP_CACHE_SIZE (__mf_lc_mask + 1)

/* }}} */
/* ------------------------------------------------------------------------ */
/* {{{ OPT_STATS-related globals.  */

#if OPT_STATS
static unsigned long __mf_count_check;
static unsigned long __mf_lookup_cache_reusecount [LOOKUP_CACHE_SIZE_MAX];
static unsigned long __mf_treerot_left, __mf_treerot_right;
static unsigned long __mf_count_register;
static unsigned long __mf_total_register_size [__MF_TYPE_STATIC+1];
static unsigned long __mf_count_unregister;
static unsigned long __mf_total_unregister_size;
static unsigned long __mf_count_violation [__MF_VIOL_UNREGISTER+1];
#endif

/* }}} */
/* ------------------------------------------------------------------------ */
/* {{{ MODE_CHECK-related globals.  */

#if MODE_CHECK

typedef struct __mf_object
{
  uintptr_t low, high;
  int type;
  const char *name;
  unsigned check_count; /* Number of times __mf_check was called.  */

  uintptr_t alloc_pc;
  struct timeval alloc_time;
#ifdef OPT_BKTRACE
  char **alloc_backtrace;
  size_t alloc_backtrace_size;
#endif  

#ifdef OPT_PERSIST
  int deallocated_p;
  uintptr_t dealloc_pc;
  struct timeval dealloc_time;
#ifdef OPT_BKTRACE
  char **dealloc_backtrace;
  size_t dealloc_backtrace_size;
#endif  
#endif
} __mf_object_t;


typedef struct __mf_object_tree
{
  __mf_object_t data;
  struct __mf_object_tree *left;
  struct __mf_object_tree *right;
} __mf_object_tree_t;

/* Live objects: binary tree on __mf_object_t.low */
__mf_object_tree_t *__mf_object_root;
#if OPT_PERSIST
/* Dead objects: circular arrays */
unsigned __mf_object_dead_head[__MF_TYPE_STATIC+1]; /* next empty spot */
__mf_object_tree_t *__mf_object_cemetary[__MF_TYPE_STATIC+1][OPT_PERSIST];
#endif

unsigned __mf_recursion_protect;

static __mf_object_tree_t *__mf_find_object (uintptr_t low, uintptr_t high);
static unsigned __mf_find_objects (uintptr_t ptr_low, uintptr_t ptr_high, 
				   __mf_object_tree_t **objs, unsigned max_objs);
static unsigned __mf_find_dead_objects (uintptr_t ptr_low, uintptr_t ptr_high, 
					__mf_object_tree_t **objs, unsigned max_objs);
static void __mf_link_object (__mf_object_tree_t *obj);
static void __mf_unlink_object (__mf_object_tree_t *obj);
static void __mf_describe_object (__mf_object_t *obj);
#endif

/* }}} */
/* ------------------------------------------------------------------------ */
/* {{{ __mf_check */

void __mf_check (uintptr_t ptr, uintptr_t sz)
{
  unsigned entry_idx = __MF_CACHE_INDEX (ptr);
  struct __mf_cache *entry = & __mf_lookup_cache [entry_idx];
  int violation_p = 0;
  uintptr_t ptr_high = CLAMPSZ (ptr, sz);
  static unsigned recursion = 0;

#if OPT_STATS
  struct __mf_cache old_entry = *entry;
#endif

  if (UNLIKELY (recursion)) return;
  recursion ++;

#if MODE_NOP
  /* Do nothing.  The cache will remain unpopulated.  */
#elif MODE_NOSIR
  violation_p = 1;
#elif MODE_YESSIR
  entry->low = ptr;
  entry->high = ptr_high;
#elif MODE_CHECK
  {
    __mf_object_tree_t *node = __mf_find_object (ptr, ptr_high);
    __mf_object_t *obj = (node != NULL ? (& node->data) : NULL);
    
    if (LIKELY (obj && ptr >= obj->low && ptr_high <= obj->high))
      {
	entry->low = obj->low;
	entry->high = obj->high;
	obj->check_count ++;  /* XXX: what about overflow?  */
      }
    else
      {
	/* XXX:  */
	violation_p = 1;
      }
  }
#endif

#if OPT_STATS
  __mf_count_check ++;

  if (LIKELY (old_entry.low != entry->low || old_entry.high != entry->high))
    /* && (old_entry.low != 0) && (old_entry.high != 0)) */
    __mf_lookup_cache_reusecount [entry_idx] ++;    
#endif
#if OPT_TRACE
  fprintf (stderr, "mf chk p=%08lx s=%lu\n", ptr, sz);
#endif

  if (UNLIKELY (violation_p))
    __mf_violation (ptr, sz, (uintptr_t) __builtin_return_address (0), __MF_VIOL_CHECK);

  recursion --;
}

/* }}} */
/* {{{ __mf_register */

void
__mf_register (uintptr_t ptr, uintptr_t sz, int type, const char *name)
{
#if MODE_NOP
#elif MODE_NOSIR
  __mf_violation (ptr, sz, (uintptr_t) __builtin_return_address (0), __MF_VIOL_REGISTER);
#elif MODE_YESSIR
  /* Clear the cache.  */
  memset (__mf_lookup_cache, 0, sizeof(__mf_lookup_cache));
#elif MODE_CHECK
  __mf_object_tree_t *ovr_obj[1];
  __mf_object_tree_t *new_obj;
  unsigned num_overlapping_objs;
  static unsigned recursion = 0;
  uintptr_t low = ptr;
  uintptr_t high = CLAMPSZ (ptr, sz);

  if (UNLIKELY (recursion)) return;
  recursion ++;

  /* Treat unknown size indication as 1.  */
  if (UNLIKELY (sz == 0)) sz = 1;

  num_overlapping_objs = __mf_find_objects (low, high, ovr_obj, 1);
  if (UNLIKELY (num_overlapping_objs > 0))
    {
      /* Normally, this would be a violation.  However, accept a
	 single duplicate registration for static objects, since these
	 may come from distinct compilation units.  */
      if (type == __MF_TYPE_STATIC &&
	  num_overlapping_objs == 1 &&
	  ovr_obj[0]->data.type == __MF_TYPE_STATIC &&
	  ovr_obj[0]->data.low == low &&
	  ovr_obj[0]->data.high == high)
	{
	  /* do nothing */
	  ;
	}
      else
	{
	  __mf_violation (ptr, sz, (uintptr_t) __builtin_return_address (0),
			  __MF_VIOL_REGISTER);
	}

      recursion --;
      return;
    }

  new_obj = calloc (1, sizeof(__mf_object_tree_t));
  new_obj->data.low = low;
  new_obj->data.high = high;
  new_obj->data.type = type;
  new_obj->data.name = name;
  new_obj->data.alloc_pc = (uintptr_t) __builtin_return_address (0);
  gettimeofday (& new_obj->data.alloc_time, NULL);

#if OPT_BKTRACE
  {
    void *array [OPT_BKTRACE];
    size_t bt_size;
    bt_size = backtrace (array, OPT_BKTRACE);
    new_obj->data.alloc_backtrace = backtrace_symbols (array, bt_size);
    new_obj->data.alloc_backtrace_size = bt_size;
  }
#endif

  __mf_link_object (new_obj);

  /* We could conceivably call __mf_check() here to prime the cache,
     but then the check_count field is not reliable.  */

  recursion --;

#endif /* MODE_* */

#if OPT_STATS
  __mf_count_register ++;
  __mf_total_register_size [(type < 0) ? 0 :
			    (type > __MF_TYPE_STATIC) ? 0 : 
			    type] += sz;
#endif
#if OPT_TRACE
  fprintf (stderr, "mf reg p=%08lx s=%lu t=%d n=`%s'\n", 
	   ptr, sz, type, name ? name : "");
#endif
}

/* }}} */
/* {{{ __mf_unregister */

void
__mf_unregister (uintptr_t ptr, uintptr_t sz)
{
#if MODE_NOP
#elif MODE_NOSIR
  __mf_violation (ptr, sz, (uintptr_t) __builtin_return_address (0), 
		  __MF_VIOL_UNREGISTER);
#elif MODE_YESSIR
  /* Clear the cache.  */
  memset (__mf_lookup_cache, 0, sizeof(__mf_lookup_cache));
#elif MODE_CHECK
  static unsigned recursion = 0;
  __mf_object_tree_t *old_obj = NULL;
  __mf_object_tree_t *del_obj = NULL;  /* Object to actually delete. */
  __mf_object_tree_t *objs[1] = {NULL};
  unsigned num_overlapping_objs;

  if (UNLIKELY (recursion)) return;
  recursion ++;

  /* Treat unknown size indication as 1.  */
  if (sz == 0) sz = 1;

  num_overlapping_objs = __mf_find_objects (ptr, CLAMPSZ (ptr, sz), objs, 1);
  if (UNLIKELY (num_overlapping_objs != 1))
    {
      /* XXX: also: should match ptr == old_obj->low ? */

      __mf_violation (ptr, sz, (uintptr_t) __builtin_return_address (0),
		      __MF_VIOL_UNREGISTER);
      recursion --;
      return;
    }

  old_obj = objs[0];
  __mf_unlink_object (old_obj);

  /* Remove any low/high pointers for this object from the lookup cache.  */
  if (LIKELY (old_obj->data.check_count)) /* Can it possibly exist in the cache?  */
    {
      uintptr_t low = old_obj->data.low;
      uintptr_t high = old_obj->data.high;
      unsigned idx_low = __MF_CACHE_INDEX (low);
      unsigned idx_high = __MF_CACHE_INDEX (high);
      unsigned i;
      for (i = idx_low; i <= idx_high; i++)
	{
	  struct __mf_cache *entry = & __mf_lookup_cache [i];
	  if (entry->low == low && entry->high == high)
	    {
	      entry->low = entry->high = (uintptr_t) 0;
	    }
	}
    }

#if OPT_PERSIST
  old_obj->data.deallocated_p = 1;
  old_obj->left = old_obj->right = NULL;
  old_obj->data.dealloc_pc = (uintptr_t) __builtin_return_address (0);
  gettimeofday (& old_obj->data.dealloc_time, NULL);

#if OPT_BKTRACE
  {
    void *array [OPT_BKTRACE];
    size_t bt_size;
    bt_size = backtrace (array, OPT_BKTRACE);
    old_obj->data.dealloc_backtrace = backtrace_symbols (array, bt_size);
    old_obj->data.dealloc_backtrace_size = bt_size;
  }
#endif /* OPT_BKTRACE */

  /* Put this object into the cemetary.  This may require this plot to
     be recycled, and the previous resident to be designated del_obj.  */

  assert (old_obj->data.type >= __MF_TYPE_UNKNOWN && 
	  old_obj->data.type <= __MF_TYPE_STATIC);
  {
    unsigned row = old_obj->data.type;
    unsigned plot = __mf_object_dead_head [row];

    del_obj = __mf_object_cemetary [row][plot];
    __mf_object_cemetary [row][plot] = old_obj;

    plot ++;
    if (plot == OPT_PERSIST) plot = 0;
    __mf_object_dead_head [row] = plot;
  }

#else  /* ! OPT_PERSIST */
  del_obj = old_obj;
#endif /* OPT_PERSIST */

#if OPT_LEAK
  if (old_obj->data.check_count == 0 &&
      old_obj->data.type == __MF_TYPE_HEAP)
    {
      fprintf (stderr, 
	       "*******\n"
	       "mudflap warning: unaccessed registered object:\n");
      __mf_describe_object (& old_obj->data);
    }
#endif

  if (del_obj != NULL) /* May or may not equal old_obj.  */
    {
#if OPT_BKTRACE
      free (del_obj->data.alloc_backtrace);
#if OPT_PERSIST
      free (del_obj->data.dealloc_backtrace);
#endif
#endif
      free (del_obj);
    }

  recursion --;
#endif /* MODE_* */

#if OPT_STATS
  __mf_count_unregister ++;
  __mf_total_unregister_size += sz;
#endif
#if OPT_TRACE
  fprintf (stderr, "mf unr p=%08lx s=%lu\n", ptr, sz);
#endif
}

/* }}} */
/* ------------------------------------------------------------------------ */
#if MODE_CHECK
/* {{{ __mf_validate_live_object_tree, _object_cemetary */

#if OPT_INTCHK
static void
__mf_validate_live_object_tree (__mf_object_tree_t *obj)
{
  assert (obj != NULL);
#if OPT_PERSIST
  assert (! obj->data.deallocated_p);
#endif
  if (obj->left)
    {
      assert (obj->left->data.high < obj->data.low);
      __mf_validate_live_object_tree (obj->left);
    }
  if (obj->right)
    {
      assert (obj->right->data.low > obj->data.high);
      __mf_validate_live_object_tree (obj->right);
    }
}

#if OPT_PERSIST
static void
__mf_validate_object_cemetary ()
{
  unsigned cls;
  unsigned i;

  for (cls = __MF_TYPE_UNKNOWN; cls <= __MF_TYPE_STATIC; cls++)
    {
      assert (__mf_object_dead_head [cls] >= 0 &&
	      __mf_object_dead_head [cls] < OPT_PERSIST);
      for (i = 0; i < OPT_PERSIST; i++)
	{
	  __mf_object_tree_t *obj = __mf_object_cemetary [cls][i];
	  if (obj != NULL)
	    {
	      assert (obj->data.deallocated_p);
	      assert (obj->left == NULL);
	      assert (obj->right == NULL);
	    }
	}
    }
}
#endif

static void 
__mf_validate_objects ()
{
  if (__mf_object_root)
    __mf_validate_live_object_tree (__mf_object_root);
#if OPT_PERSIST
  __mf_validate_object_cemetary ();
#endif
}

#endif

/* }}} */
/* {{{ __mf_find_object[s] */

/* Find overlapping live objecs between [low,high].  Return up to
   max_objs of their pointers in objs[].  Return total count of
   overlaps (may exceed max_objs). */

#if OPT_STATS
/* XXX: track traversal statistics, like average depth, balance.  */
#endif

static unsigned
__mf_find_objects_rec (uintptr_t low, uintptr_t high, __mf_object_tree_t **nodep,
		       __mf_object_tree_t **objs, unsigned max_objs)
{
  unsigned count;
  __mf_object_tree_t *node = *nodep;

  assert (low <= high);
  assert (max_objs == 0 || objs != NULL);

  if (UNLIKELY (node == NULL)) return 0;

  /* Traverse down left subtree. */
  count = 0;
  if (low < node->data.low)
    count += __mf_find_objects_rec (low, high, & node->left, objs, max_objs);

  /* Track the used slots of objs[].  */
  if (count < max_objs)
    {
      objs += count;
      max_objs -= count;
    }
  else
    {
      max_objs = 0;
    }

  /* Check for overlap with this node.  */
  if (high >= node->data.low && low <= node->data.high)
    {
      count ++;
      if (max_objs > 0)  /* Any room left?  */
	{
	  objs[0] = node;
	  objs ++;
	  max_objs --;
	}
    }

  /* Traverse down right subtree.  */
  if (high > node->data.high)
    count += __mf_find_objects_rec (low, high, & node->right, objs, max_objs);
  /* There is no need to manipulate objs/max_objs any further.  */


  /* Rotate a child node up if its access count is higher. */
  /* XXX: Should there be a minimum check_count delta that triggers this?  */
  /* XXX: Should age (scale down) system-wide check_counts periodically.  */
  if (UNLIKELY ((node->left && node->left->data.check_count > node->data.check_count) &&
		((!node->right || (node->right && 
				   node->left->data.check_count > 
				   node->right->data.check_count)))))
    {
      __mf_object_tree_t *l = node->left;
      __mf_object_tree_t *l_r = l->right;

      *nodep = l;
      l->right = node;
      node->left = l_r;
      __mf_treerot_left ++;
    }
  else if (UNLIKELY ((node->right && node->right->data.check_count > node->data.check_count) &&
		     ((!node->left || (node->left && 
				       node->right->data.check_count > 
				       node->left->data.check_count)))))
    {
      __mf_object_tree_t *r = node->right;
      __mf_object_tree_t *r_l = r->left;

      *nodep = r;
      r->left = node;
      node->right = r_l;
      __mf_treerot_right ++;
    }


  return count;
}


__mf_object_tree_t *
__mf_find_object (uintptr_t low, uintptr_t high)
{
  __mf_object_tree_t* objects[1]; /* Find at most one.  */
  unsigned count;
#if OPT_INTCHK
  __mf_validate_objects ();
#endif
  count = __mf_find_objects_rec (low, high, & __mf_object_root, objects, 1);
  if (count == 1)
    return objects[0];
  else
    return NULL;
}


unsigned
__mf_find_objects (uintptr_t ptr_low, uintptr_t ptr_high,
		   __mf_object_tree_t **objs, unsigned max_objs)
{
#if OPT_INTCHK
  __mf_validate_objects ();
#endif
  return __mf_find_objects_rec (ptr_low, ptr_high, & __mf_object_root, objs, max_objs);
}

/* }}} */
/* {{{ __mf_link_object */

static void
__mf_link_object2 (__mf_object_tree_t *ptr, __mf_object_tree_t **link)
{
  __mf_object_tree_t *node = *link;

  assert (ptr != NULL);
  if (node == NULL) 
    {
      *link = ptr;
      return;
    }

  if (ptr->data.high < node->data.low)
    return __mf_link_object2 (ptr, & node->left);
  else if (ptr->data.low > node->data.high)
    return __mf_link_object2 (ptr, & node->right);
  else
    abort (); /* XXX: duplicate object */
}


void
__mf_link_object (__mf_object_tree_t *ptr)
{
#if OPT_INTCHK
  __mf_validate_objects ();
#endif
  return __mf_link_object2 (ptr, & __mf_object_root);
}

/* }}} */
/* {{{ __mf_unlink_object */

static void
__mf_unlink_object2 (__mf_object_tree_t *ptr, __mf_object_tree_t **link)
{
  __mf_object_tree_t *node = *link;
  
  assert (ptr != NULL);
  if (node == ptr) 
    {
      static unsigned promote_left_p = 0;
      promote_left_p = 1 - promote_left_p;

      /* Alternate promoting the left & right subtrees. */
      if (promote_left_p)
	{
	  *link = ptr->left;
	  if (ptr->right != NULL)
	    __mf_link_object2 (ptr->right, link);
	}
      else
	{
	  *link = ptr->right;
	  if (ptr->left != NULL)
	    __mf_link_object2 (ptr->left, link);
	}

      return;
    }

  if (ptr->data.high < node->data.low)
    return __mf_unlink_object2 (ptr, & node->left);
  else if (ptr->data.low > node->data.high)
    return __mf_unlink_object2 (ptr, & node->right);
  else
    abort (); /* XXX: missing object */
}

static void
__mf_unlink_object (__mf_object_tree_t *node)
{
  __mf_unlink_object2 (node, & __mf_object_root);
}

/* }}} */
/* {{{ __mf_find_dead_objects */

/* Find overlapping dead objecs between [low,high].  Return up to
   max_objs of their pointers in objs[].  Return total count of
   overlaps (may exceed max_objs).  */

static unsigned
__mf_find_dead_objects (uintptr_t low, uintptr_t high,
			__mf_object_tree_t **objs, unsigned max_objs)
{
#if OPT_PERSIST
  unsigned count;
  unsigned recollection;
  unsigned row;

  assert (low <= high);
  assert (max_objs == 0 || objs != NULL);

  /* Widen the search from the most recent plots in each row, looking
     backward in time.  */
  recollection = 0;
  while (recollection < OPT_PERSIST)
    {
      count = 0;

      for (row = __MF_TYPE_UNKNOWN; row <= __MF_TYPE_STATIC; row ++)
	{
	  unsigned plot;
	  unsigned i;

	  plot = __mf_object_dead_head [row];
	  for (i = 0; i <= recollection; i ++)
	    {
	      __mf_object_tree_t *obj;

	      /* Look backward through row: it's a circular buffer.  */
	      if (plot > 0) plot --;
	      else plot = OPT_PERSIST - 1;

	      obj = __mf_object_cemetary [row][plot];
	      if (obj && obj->data.low <= high && obj->data.high >= low)
		{
		  /* Found an overlapping dead object!  */
		  if (count < max_objs)
		    objs [count] = obj;
		  count ++;
		}
	    }
	}

      if (count)
	break;

      /* Look farther back in time.  */
      recollection = (recollection * 2) + 1;
    }

  return count;

#else /* ! OPT_PERSIST */
  return 0;
#endif
}

/* }}} */
/* {{{ __mf_describe_object */

static void
__mf_describe_object (__mf_object_t *obj)
{
  fprintf (stderr,
	   "mudflap object %p: name=`%s'\n"
	   "bounds=[%08lx,%08lx] area=%s access-count=%u\n"
	   "alloc time=%lu.%06lu pc=%08lx\n",
	   obj, (obj->name ? obj->name : ""), obj->low, obj->high,
	   (obj->type == __MF_TYPE_HEAP ? "heap" :
	    obj->type == __MF_TYPE_STACK ? "stack" :
	    obj->type == __MF_TYPE_STATIC ? "static" :
	    "unknown"),
	   obj->check_count,
	   obj->alloc_time.tv_sec, obj->alloc_time.tv_usec, obj->alloc_pc);
#if OPT_BKTRACE
  {
    int i;
    for (i=0; i<obj->alloc_backtrace_size; i++)
      fprintf (stderr, "      %s\n", obj->alloc_backtrace[i]);
  }
#endif
#if OPT_PERSIST
  if (obj->deallocated_p)
    {
      fprintf (stderr, "dealloc time=%lu.%06lu pc=%08lx\n",
	       obj->dealloc_time.tv_sec, obj->dealloc_time.tv_usec, obj->dealloc_pc);
#if OPT_BKTRACE
      {
	int i;
	for (i=0; i<obj->dealloc_backtrace_size; i++)
	  fprintf (stderr, "      %s\n", obj->dealloc_backtrace[i]);
      }
#endif
    }
#endif
}

#if OPT_LEAK
static unsigned
__mf_report_leaks (__mf_object_tree_t *node)
{
  static unsigned count = 0;  /* Shared amongst recursive calls.  */
  /* XXX: when to reset?  */

  if (node == NULL) return count;

  /* Inorder traversal. */
  __mf_report_leaks (node->left);
  if (node->data.type == __MF_TYPE_HEAP)
    {
      count ++;
      fprintf (stderr, "Leaked object %u:\n", count);
      __mf_describe_object (& node->data);
    }
  __mf_report_leaks (node->right);

  return count;
}
#endif

/* }}} */
#endif
/* ------------------------------------------------------------------------ */
/* {{{ __mf_report */

void
__mf_report ()
{
#if OPT_STATS
  fprintf (stderr,
	   "*******\n"
	   "mudflap stats:\n"
	   "calls to __mf_check: %lu rot: %lu/%lu\n"
	   "         __mf_register: %lu [%luB, %luB, %luB, %luB]\n"
	   "         __mf_unregister: %lu [%luB]\n"
	   "         __mf_violation: [%lu, %lu, %lu, %lu]\n",
	   __mf_count_check, __mf_treerot_left, __mf_treerot_right,
	   __mf_count_register,
	   __mf_total_register_size[0], __mf_total_register_size[1],
	   __mf_total_register_size[2], __mf_total_register_size[3],
	   __mf_count_unregister, __mf_total_unregister_size,
	   __mf_count_violation[0], __mf_count_violation[1],
	   __mf_count_violation[2], __mf_count_violation[3]);

  /* Lookup cache stats.  */
  {
    unsigned i;
    unsigned max_reuse = 0;
    unsigned num_used = 0;
    unsigned num_unused = 0;

    for (i = 0; i < LOOKUP_CACHE_SIZE; i++)
      {
	if (__mf_lookup_cache_reusecount[i])
	  num_used ++;
	else
	  num_unused ++;
	if (max_reuse < __mf_lookup_cache_reusecount[i])
	  max_reuse = __mf_lookup_cache_reusecount[i];
      }
    fprintf (stderr, "lookup cache slots used: %u  unused: %u  peak-reuse: %u\n",
	     num_used, num_unused, max_reuse);
  }

  {
    unsigned live_count;
    live_count = __mf_find_objects ((uintptr_t) 0, ~ (uintptr_t) 0, NULL, 0);
    fprintf (stderr, "number of live objects: %u\n", live_count);
  }

#if OPT_PERSIST
  {
    unsigned dead_count = 0;
    unsigned row, plot;
    for (row = __MF_TYPE_UNKNOWN; row <= __MF_TYPE_STATIC; row ++)
      for (plot = 0 ; plot < OPT_PERSIST; plot ++)
	if (__mf_object_cemetary [row][plot] != 0)
	  dead_count ++;
    fprintf (stderr, "          persistent dead objects: %u\n", dead_count);
  }
#endif
#endif /* OPT_STATS */
#if OPT_LEAK && MODE_CHECK
  {
    unsigned l = __mf_report_leaks (__mf_object_root);
    fprintf (stderr, "number of leaked objects: %u\n", l);
  }
#endif
}

/* }}} */
/* {{{ OPT_INIT support */

#if OPT_INIT

#if 0
/* XXX */
#define CTOR  __attribute__ ((section (".init")))
#define DTOR  __attribute__ ((section (".fini")))
#endif

#define CTOR  __attribute__ ((constructor))
#define DTOR  __attribute__ ((destructor))

extern void __mf_init () CTOR;
void __mf_init ()
{
#if OPT_TRACE
  fprintf (stderr, "mf init\n");
#endif
}


extern void __mf_fini () DTOR;
void __mf_fini ()
{
#if OPT_TRACE
  fprintf (stderr, "mf fini\n");
#endif
  __mf_report ();
}

#endif

/* }}} */
/* ------------------------------------------------------------------------ */
/* {{{ __mf_violation */

void
__mf_violation (uintptr_t ptr, uintptr_t sz, uintptr_t pc, int type)
{
  static unsigned violation_number;

#if OPT_TRACE
  fprintf (stderr, "mf violation pc=%08lx type=%d\n", pc, type);
#endif
#if OPT_STATS
  __mf_count_violation [(type < 0) ? 0 :
			(type > __MF_VIOL_UNREGISTER) ? 0 :
			type] ++;
#endif

  /* Print out a basic warning message.  */
  /* XXX: should even this be conditional?  */
  {
    struct timeval now;
    gettimeofday (& now, NULL);

    violation_number ++;
    fprintf (stderr,
	     "*******\n"
	     "mudflap violation %u: time=%lu.%06lu ptr=%08lx size=%lx pc=%08lx type=%s\n", 
	     violation_number,
	     now.tv_sec, now.tv_usec, ptr, sz, pc,
	     ((type == __MF_VIOL_CHECK) ? "check" :
	      (type == __MF_VIOL_REGISTER) ? "register" :
	      (type == __MF_VIOL_UNREGISTER) ? "unregister" :
	      "unknown"));
  }

#if OPT_HELPFUL
  {
    unsigned dead_p;
    unsigned num_helpful = 0;

    /* Look for nearby objects.  For this, we start with s_low/s_high
       pointing to the given area, looking for overlapping objects.
       If none show up, widen the search area and keep looking. */

    if (sz == 0) sz = 1;

    for (dead_p = 0; dead_p <= 1; dead_p ++) /* for dead_p in 0 1 */
      {
	enum {max_objs = 3}; /* magic */
	__mf_object_tree_t *objs[max_objs];
	unsigned num_objs = 0;
	uintptr_t s_low, s_high;
	unsigned tries = 0;
	unsigned i;

	s_low = ptr;
	s_high = CLAMPSZ (ptr, sz);

	while (tries < 4) /* magic */
	  {
	    if (dead_p)
	      num_objs = __mf_find_dead_objects (s_low, s_high, objs, max_objs);
	    else
	      num_objs = __mf_find_objects (s_low, s_high, objs, max_objs);

	    if (num_objs) /* good enough */
	      break;

	    tries ++;
	    s_low = CLAMPSUB (s_low, (sz * tries));
	    s_high = CLAMPADD (s_high, (sz * tries));
	  }

	for (i = 0; i < num_objs; i++)
	  {
	    __mf_object_t *obj = & objs[i]->data;
	    uintptr_t low = ptr;
	    uintptr_t high = CLAMPSZ (ptr, sz);
	    unsigned before = (high < obj->low) ? obj->low - high : 0;
	    unsigned after = (low > obj->high) ? low - obj->high : 0;
	    unsigned into = (high >= obj->low && low <= obj->high) ? low - obj->low : 0;

	    fprintf (stderr, "Nearby object %u: ptr is %uB %s\n",
		     i + 1,
		     (before ? before :
		      after ? after :
		      into),
		     (before ? "before" :
		      after ? "after" :
		      "into"));
	    __mf_describe_object (obj);
	  }
	num_helpful += num_objs;
      }

    fprintf (stderr, "number of nearby objects: %u\n", num_helpful);
  }
#endif

  /* How to finally handle this violation?  */
#if VIOLATION_NOP
  return;
#elif VIOLATION_SEGV
  kill (getpid(), SIGSEGV);
#elif VIOLATION_CORE
  abort ();
#elif VIOLATION_GDB
#error "not yet implemented"
#endif
}

/* }}} */
/* ------------------------------------------------------------------------ */
