/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "51" } } */
/* { dg-final { scan-assembler "movz" } } */
/* { dg-final { scan-assembler-not "beq" } } */
typedef unsigned long long uint64_t;
union cvmx_buf_ptr {
    void* ptr;
    uint64_t u64;
    struct
    {
        uint64_t i : 1;
        uint64_t back : 4;

        uint64_t pool : 3;
        uint64_t size :16;
        uint64_t addr :40;
    } s;
};

typedef union cvmx_buf_ptr cvmx_buf_ptr_t;

void f(long long *a, long long use_ipd_no_wptr, uint64_t ptr)
{
  cvmx_buf_ptr_t packet_ptr;
  packet_ptr.u64 = 0;
  if (use_ipd_no_wptr)
  {
    packet_ptr.s.pool = (0);
    packet_ptr.s.size = (16 * (128));
  }
  packet_ptr.s.addr = ptr;
  *a = packet_ptr.u64;
}

