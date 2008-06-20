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


DUMMY_FN (vect_set_verbosity_level)
DUMMY_FN (vectorize_loops)


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
