// Copyright (C) 2001 Free Software Foundation, Inc.
// Contributed by Ben Elliston <bje@redhat.com>

// PR 80: Packed enums use minimum required storage.
// execution test - XFAIL *-*-*

extern "C" void abort();

enum numbers { one, two, three } __attribute__ ((packed)) nums;  // gets bogus error - - XFAIL *-*-*
enum colours { red = 1000, green, blue } __attribute__ ((packed)) cols;  // gets bogus error - - XFAIL *-*-*

int
main()
{
  if (sizeof (nums) != 1)
    abort ();

  if (sizeof (cols) != 2)
    abort ();

  return 0;
}
