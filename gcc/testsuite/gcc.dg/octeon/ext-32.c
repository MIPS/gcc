/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* SI-truncation of a 32-bit zero-extraction is really a
   sign-extraction or an unaligned word load in this particular
   case.  */
/* { dg-final { scan-assembler-not "dext" } } */
/* { dg-final { scan-assembler-not "sll" } } */

typedef unsigned short int uint16_t;
typedef unsigned long long uint64_t;
struct  prts
{
  uint64_t pad:8;
  uint64_t prts:32;
};
typedef struct flow_record
{
  struct prts key2;
  uint16_t dstsd;
}
FlowRecord, *Flow;
fw_flow_dump (Flow flow)
{
  f ((int) flow->key2.prts, decode_sd (flow->dstsd));
}
