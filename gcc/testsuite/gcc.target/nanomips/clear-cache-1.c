/* { dg-do compile } */
/* { dg-options "-march=32r6" } */
/* { dg-final { scan-assembler "\tsynci\t" } } */
/* { dg-final { scan-assembler "\tjrc?.hb\t" } } */
/* { dg-final { scan-assembler-not "_flush_cache|mips_sync_icache|_cacheflush" } } */

void f()
{
  int size = 40;
  char *memory = __builtin_alloca(size);
  __builtin___clear_cache(memory, memory + size);
}

