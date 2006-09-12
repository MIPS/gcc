#include "version.h"

/* This is the location of the online document giving instructions for
   reporting bugs.  If you distribute a modified version of GCC,
   please change this to refer to a document giving instructions for
   reporting bugs to you, not us.  (You are of course welcome to
   forward us bugs reported to you, if you determine that they are
   not bugs in your modifications.)  */

const char bug_report_url[] = BUGURL;

/* The complete version string, assembled from several pieces.
   BASEVER, DATESTAMP and DEVPHASE are defined by the Makefile.  */

const char version_string[] = BASEVER DATESTAMP DEVPHASE;
const char pkgversion_string[] = PKGVERSION;
