
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "bitvec.h"

void
bitvec::dump (FILE *file) const
{
  size_t bits = m_bits.length () * bits_per_elt;
  fprintf (file, "n_bits = %d, set = {", (int) bits);

  for (size_t pos = 30, i = 0; i < bits; i++)
    if ((*this)[i])
      {
	if (pos > 70)
	  {
	    fprintf (file, "\n  ");
	    pos = 0;
	  }

	fprintf (file, "%d ", (int) i);
	pos += 2 + (i >= 10) + (i >= 100) + (i >= 1000);
      }

  fprintf (file, "}\n");
}

ssize_t
bitvec::first_set_bit () const
{
  for (size_t i = begin (); i < end (); i++)
    if ((*this)[i])
      return i;

  return -1;
}
