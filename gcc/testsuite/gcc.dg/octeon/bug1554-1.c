/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */

typedef signed short int16;
typedef signed int int32;
l110 (int16 l111[4], int16 l79[4])
{
  int16 l40, l112, l113, l114[4][4], l115, l116;
  int16 l117, l118, l119, l120, l121;
  int32 l129, l130, l131, l132;

  for (l115 = 0; l115 < 4; l115++)
    {
      l117 = 1;
      for (l40 = 0; l40 < 4; l40++)
	{
	  if (l115 == l40)
	    continue;
	  for (l112 = l40 + 1; l112 < 4; l112++)
	    {
	      if (l114[l112][l40] == 0)
		l117 = 0;
	    }
	}
    }
  use (l117);
}

