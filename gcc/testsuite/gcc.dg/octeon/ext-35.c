/* { dg-do run { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* SI-truncation of a 32 or more bit zero-extraction is really a
   sign-extraction.  */

void abort (void);

typedef unsigned short int uint16_t;
typedef unsigned long long uint64_t;
struct  prts
{
  uint64_t pad:8;
  uint64_t prts:33;
};
decode_sd (uint16_t s) {}
typedef struct flow_record
{
  struct prts key2;
  uint16_t dstsd;
}
FlowRecord, *Flow;

long long g;

f (long long i, int j)
{
  g = i;
}

fw_flow_dump (Flow flow)
{
  f ((int) flow->key2.prts, decode_sd (flow->dstsd));
}

FlowRecord fl;

main ()
{
  fl.key2.prts = 0x176543210ull;
  fw_flow_dump (&fl);
  if (g != (int) 0x176543210ull)
    abort ();

  fl.key2.prts = 0x186543210ull;
  fw_flow_dump (&fl);
  if (g != (int) 0x186543210ull)
    abort ();

  return 0;
}
