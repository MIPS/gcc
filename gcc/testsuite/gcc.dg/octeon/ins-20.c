/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* We should only have moves that either initialize with zero or set
   up function-call arguments.  Otherwise the move is likely to be
   unoptimized sign-extension.  */
/* { dg-final { scan-assembler-not "move\t\\\$\[^45\],\\\$\[^0\]" } } */

typedef unsigned int uint;
typedef unsigned int uint32_t;
typedef unsigned long int uint64_t;
typedef struct
{
  uint64_t tag:32;
}
cvmx_wqe_t __attribute__ ((aligned (128)));
typedef union
{
  uint64_t u64;
  struct
  {
    uint64_t tag:32;
  }
  s;
}
cvmx_pow_tag_req_t;
static inline void
cvmx_pow_tag_sw_nocheck (uint32_t tag)
{
  cvmx_pow_tag_req_t tag_req;
  tag_req.s.tag = tag;
  cvmx_write_io (tag_req.u64);
}

cvmx_wqe_t *work;
int
main ()
{
  uint stage;
  for (stage = 0; stage < (10000 - 1) + 1; stage++)
    cvmx_pow_tag_sw_nocheck (work->tag += 0x100);
}
