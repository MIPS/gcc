 implementing signed and unsigned arithmetic.
  unsigned u15:15;
  if ((x.u15 - 2) >= 0)		/* promoted value should be signed */

  /* Conditionalize check on whether integers are 4 bytes or larger, i.e.
     larger than a 31 bit bitfield.  */
  if (sizeof (int) >= 4)
    {
      if ((x.u31 - 2) >= 0)	/* promoted value should be signed */
	abort ();
    }
  else
    {
      if ((x.u31 - 2) < 0)	/* promoted value should be UNsigned */
	abort ();
    }
