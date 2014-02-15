/* static struct stat uses ASM_OUTPUT_LOCAL to emit it in .cvmx_shared_bss
   section.  */ 

/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O0" } */
/* { dg-final { scan-assembler ".local\tstate\n\t.section\t.cvmx_shared_bss,\"aw\",\@nobits\n\[^\n\]*\n\[^\n\]*\n\[^\n\]*\nstate:\n\t.space\t12\n" } } */
/* { dg-final { scan-assembler ".globl\tport_lock\n\[^\n\]*\n\[^\n\]*\n\[^\n\]*\nport_lock:\n\t.space\t40\n" } } */

#define CVMX_SHARED __attribute__ ((cvmx_shared))

#define CACHE_LINE_ALIGN __attribute__ ((aligned(128)))

typedef struct {
    volatile unsigned int value;
} cvmx_spinlock_t;

typedef struct {
   unsigned int stack_top;
   unsigned int heap_base;
} cvmx_sysinfo_t;

CVMX_SHARED static struct {
  cvmx_sysinfo_t sysinfo;
  cvmx_spinlock_t lock;
} state = { .lock = {0} };

CVMX_SHARED CACHE_LINE_ALIGN cvmx_spinlock_t  port_lock[10];

main()
{
  state.lock = port_lock[2];
  return state.lock.value; 
}
