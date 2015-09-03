/* { dg-options "-mcompact-branches=never -G0 -mno-abicalls" } */
/* { dg-final { scan-assembler-not "beq\t\\\$4,\\\$0,\\\$L\[0-9\]+\n\tlw\t\\\$4,%lo\\(mips_cm_is64\\)\\(\\\$4\\)" } } */

enum
{
  CPU_R4700,
  CPU_R5000,
  CPU_R5500,
  CPU_NEVADA,
  CPU_RM7000,
  CPU_SR71000,
  CPU_4KC,
  CPU_4KEC,
  CPU_4KSC,
  CPU_24K,
  CPU_34K,
  CPU_1004K,
  CPU_74K,
  CPU_ALCHEMY,
  CPU_PR4450,
  CPU_BMIPS32,
  CPU_BMIPS3300,
  CPU_BMIPS5000,
  CPU_JZRISC,
  CPU_M14KC,
  CPU_M14KEC,
  CPU_INTERAPTIV,
  CPU_P5600,
  CPU_PROAPTIV,
  CPU_1074K,
  CPU_M5150,
  CPU_I6400,
  CPU_R3000,
  CPU_5KC,
  CPU_5KE,
  CPU_20KC,
  CPU_25KF,
  CPU_SB1,
  CPU_SB1A,
  CPU_XLP,
  CPU_QEMU_GENERIC
};

struct cpuinfo_mips
{
  long options;
  int isa_level;
} cpu_data[1];

struct thread_info
{
  int cpu;
};

int a, b, c, d, e, mips_cm_is64;

static int __attribute__ ((__cold__))
mips_sc_probe_cm3 ()
{
  struct thread_info *info;
  struct cpuinfo_mips *c = &cpu_data[info->cpu];
  if (mips_cm_is64)
    c->options = 0;
  return 1;
}

int
mips_sc_probe ()
{
  struct cpuinfo_mips ci = cpu_data[c];
  if (mips_cm_is64)
    __asm__("" ::: "memory");
  if (d)
    return mips_sc_probe_cm3 ();
  if (ci.isa_level)
    switch (a)
      {
      case CPU_QEMU_GENERIC:
      case CPU_I6400:
      case CPU_R3000:
      case CPU_NEVADA:
      case CPU_BMIPS3300:
	break;
      default:
	__builtin_unreachable ();
      }
  switch (a)
    case CPU_R3000:
      e = b;
}
