#include <stdio.h>
#include <unistd.h>
#include <string.h>

char test_1 (FILE *logfile)
{
  char *password = getpass (">"); /* { dg-message "\\(1\\) state of 'password': 'start' -> 'sensitive'" } */
  fprintf (logfile, "got password %s\n", password); /* { dg-warning "sensitive value 'password' written to output file \\\[CWE-532\\\]" } */
  /* { dg-message "\\(2\\) here \\('password' is in state 'sensitive'\\)" "" { target *-*-* } .-1 } */
}

char test_2 (FILE *logfile, int i)
{
  char *password = getpass (">"); /* { dg-message "\\(1\\) state of 'password': 'start' -> 'sensitive'" } */
  fprintf (logfile, "got password[%i]: %s\n", i, password); /* { dg-warning "sensitive value 'password' written to output file \\\[CWE-532\\\]" } */
  /* { dg-message "\\(2\\) here \\('password' is in state 'sensitive'\\)" "" { target *-*-* } .-1 } */
}

char test_3 (FILE *logfile)
{
  char *password = getpass (">"); /* { dg-message "\\(1\\) state of 'password': 'start' -> 'sensitive'" } */
  printf ("got password %s\n", password); /* { dg-warning "sensitive value 'password' written to output file \\\[CWE-532\\\]" } */
  /* { dg-message "\\(2\\) here \\('password' is in state 'sensitive'\\)" "" { target *-*-* } .-1 } */
}

char test_4 (FILE *logfile)
{
  char *password = getpass (">"); /* { dg-message "\\(1\\) state of 'password': 'start' -> 'sensitive'" } */
  fwrite (password, strlen (password), 1, logfile); /* { dg-warning "sensitive value 'password' written to output file \\\[CWE-532\\\]" } */
  /* { dg-message "\\(2\\) here \\('password' is in state 'sensitive'\\)" "" { target *-*-* } .-1 } */
}

/* TODO: strdup etc, strcpy, memcpy, etc.  */
