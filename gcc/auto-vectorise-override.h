/**
 * auto-vectorise-override.h
 *
 * This file provides the interface to load auto vectorisation overrides from a file.
 *
 * The file name is given by environment variable AUTOVECTORISE_OVERRIDE.  If that file is not found then the default
 * heuristic should be used.  The file contains one loop specifier on each line.  There should be no blank lines or
 * comments, etc (BTW, a bit of this is nicked out of my plugin stuff for loop unrolling - it uses a much more robust
 * approach with XML, decent error checking, reporting, etc. - I just stripped it down and made a few hacks).
 *
 * Each loop specifier looks like this:
 * filename=<filename> function=<functionname> loop=<loopnum> target=<targetid>
 *
 * The <filename> and <functionname> fields can be glob patterns.
 * The <loopnum> field is either an integer or the special value, "*", which matches all loops.
 * The <targetid> field is either an integer or the special values "default" or "none".  An integer >= 0
 * indicates that the loop matched by the other fields should be vectorised for the given target.  A value of -1 means
 * that no vectorisation should be done.  -2 means that the default GCC heuristic should be used (i.e.
 * that the behaviour for matching loops is not to be overriden).  Using "default" is that same as giving an integer of
 * -2; using "none" is the same as giving an integer of -1.
 *
 * The system will try to match loops against specifications in lexical order.  As soon as a matching specification is
 * found the target is determined.  This means that the most specific specifications should be at the top of the file.
 * If no matching specification is found, then GCC's default heuristic will be used.
 *
 * Example Specification Files
 * ---------------------------
 * The first file says that all loops for which the main_input_filename matches "foo*.c" and whose function name matches
 * "bar*" will be vectorised by GCC's default heuristic, while all other loops will not be vectorised.
 *
 * filename=foo*.c function=bar* loop=* target=default
 * filename=* function=* loop=* target=none
 *
 * The next gives specific targets for individual loops.  Any other loops, not being matches will use the default
 * heuristic.
 *
 * filename=foo.c function=bar loop=1 target=1
 * filename=foo.c function=bar loop=2 target=2
 *
 * The last file makes a mistake.  Because the least specific line is at the top, all loops will match it.  The more
 * specific lines below will never have any effect
 *
 * filename=* function=* loop=* target=none
 * filename=foo*.c function=bar* loop=* target=default
 *
 * Lifecycle
 * ---------
 * The function, AutoVectorise_Override_setup should be called before calling AutoVectorise_Override_getTarget;
 * preferably once near the beginning of the compilation.  AutoVectorise_Override_shutdown cleans up any data structures
 * created when the specification file was read.
 */

#ifndef GCC_AUTOVECTORISEOVERRIDE_H_
#define GCC_AUTOVECTORISEOVERRIDE_H_

struct loop;

/**
 * This is the name of the environment variable in which holds the file name of the override specification file.
 */
#define AUTOVECTORISE_OVERRIDE_ENVNAME "AUTOVECTORISE_OVERRIDE"
/**
 * Return code from AutoVectorise_Override_getTarget for not vectorising.
 */
#define AUTOVECTORISE_OVERRIDE_DONT -1
/**
 * Return code from AutoVectorise_Override_getTarget for doing the default heuristics' vectorising.
 */
#define AUTOVECTORISE_OVERRIDE_DEFAULT -2
/**
 * Parse the specification file if the environment variable is set.
 *
 * @return TRUE on success; FALSE otherwise (AutoVectorise_Override_shutdown should still be called)
 */
bool AutoVectorise_Override_setup( void );
/**
 * Get the vectorisation target for a given loop.
 *
 * @param loop the loop to consider; may not be NULL
 * @return the target id; negative if the default heuristic should be used; 0 if no auto-vectorisation should be done;
 * positive for a particular target
 */
int AutoVectorise_Override_getTarget( struct loop* loop );
/**
 * Clean up data structures created when parsing the specification file
 */
void AutoVectorise_Override_shutdown( void );

#endif /*GCC_AUTOVECTORISEOVERRIDE_H_*/
