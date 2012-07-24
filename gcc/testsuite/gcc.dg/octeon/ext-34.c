/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* SI-truncation of a 32 or more bit zero-extraction is really a
   sign-extraction.  */
/* { dg-final { scan-assembler "exts\t.*,31" } } */

typedef unsigned short int uint16_t;
typedef unsigned long long uint64_t;
struct  prts
{
  uint64_t pad:8;
  uint64_t prts:33;
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
