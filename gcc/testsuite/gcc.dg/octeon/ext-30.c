/* It would be nice to make this a runnable test but we don't multilib
   -mno-abicalls.  Instead we rely on the assembler of reporting if a
   macro is expanded in a branch delay slot.  */

/* { dg-do assemble { target *-octeon-* } } */
/* { dg-options "-O2 -mno-abicalls -fno-pic -mabi=64" } */

#include <stdlib.h>

static __attribute__ ((__section__ (".init.data")))
  enum state {
 Start,
 Collect,
 GotHeader,
 SkipIt,
 GotName,
 CopyFile,
 GotSymlink,
 Reset
} state, next_state;
static char *victim;
static __attribute__ ((__section__ (".init.data"))) unsigned count;

int do_start (void);
int do_collect (void);
int do_header (void);
int do_skip (void);
int do_reset (void);
int do_name (void);
int do_copy (void);
int do_symlink (void);

static int (*actions[]) (void) =
{
[Start] = do_start,[Collect] = do_collect,[GotHeader] = do_header,[SkipIt] =
    do_skip,[GotName] = do_name,[CopyFile] = do_copy,[GotSymlink] =
    do_symlink,[Reset] = do_reset,};
int
write_buffer (char *buf, unsigned len)
{
  count = len;
  victim = buf;
  while (!actions[state] ());
  return len - count;
}
