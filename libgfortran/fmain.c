#include "config.h"
#include "libgfortran.h"

/* The main Fortran program actually is a function, called MAIN__.
   We call it from the main() function in this file.  */
void MAIN__ (void);

/* Main procedure for fortran programs.  All we do is set up the environment
   for the Fortran program.  There's not a lot we can do right now.  */
int
main (int argc, char *argv[])
{
  /* Set up the runtime environment.  */
  init (argc, argv);

  /* Call the Fortran main program.  Internally this is a function
     called MAIN__ */
  MAIN__ ();

  /* And shut everything down again.  */
  cleanup ();

  /* Bye-bye!  */
  return 0;
}
