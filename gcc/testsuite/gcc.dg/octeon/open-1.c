/* Copyright 1992, 1993, 1994, 1995, 1996, 1999
/* { dg-do run { target mipsisa64-octeon-* } } */

/* Test overwriting open() newlib syscall. */

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdarg.h>

#define BUFSIZE 1024

char *buf;

int
open (const char *str, int flags, ...)
{
   int mode;
   va_list ap;
   
   va_start (ap, flags);
   if (flags & O_CREAT)
     mode = va_arg (ap, int);
   else
     mode = 0;
   va_end (ap);

   printf ("mode is %d\n", mode);

   _open (str, flags, mode);
}

void
foo ()
{
  int j, fd;

  buf = (char *) malloc (BUFSIZE);
  for (j = 0; j < BUFSIZE; ++j)
    buf[j] = j;

  /* open the file */
  fd = open ("foo.data", O_CREAT | O_RDWR, 0666);
  if (fd == -1)
    {
      perror ("foo.data open failed");
      return;
    }
  write (fd, buf, BUFSIZE);
}

main ()
{
  foo ();
  return 0;
}
