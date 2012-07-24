/* Check that we create profile data for simple-exec.  */

/* { dg-do run { target *-octeon-elf* } } */
/* { dg-options "-O2 -fprofile-generate" } */
/* { dg-final { upvar 1 name testcase				} } */
/* { dg-final { if [file exists fdo-1.gcda0] \{			} } */
/* { dg-final {     pass "$testcase: profile-feedback data"	} } */
/* { dg-final { \} else \{					} } */
/* { dg-final {     fail "$testcase: profile-feedback data"	} } */
/* { dg-final { \}						} } */

/* { dg-final { remote_file host delete fdo-1.gcno fdo-1.gcda0	} } */

int
main ()
{
  return 0;
}
