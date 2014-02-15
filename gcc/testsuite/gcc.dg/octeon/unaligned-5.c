/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */

/* Used to ICE.  */

struct to
{
  char c;
  short s;
}
/* So that store_split_bit_field->...->extract_split_bit_field.  */
  __attribute__ ((packed));

struct from
{
  char c;
  short s[10];
  int i;			/* make it 32-bit aligned */
};

f (struct to *to, struct from *from, unsigned n)
{
  int i;
  for (i = 0; i < n; i++)
    {
      /* Generate TARGET_MEM_REF.  */
      to[i].s = from->s[i];
    }
}
