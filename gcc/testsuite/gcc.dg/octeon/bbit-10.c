/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler-times "bbit\[01\]|bgez" 2 } } */
/* { dg-final { scan-assembler-not "dext" } } */

void abort (void);
void exit (int);

typedef unsigned long long ulong64;
 
typedef struct bitfield_s {
  ulong64 a:1;
  ulong64 b:29;
  ulong64 c:1;
  ulong64 d:15;
  ulong64 f:18;
} bitfield_t;
 
bitfield_t bar;
 
main()
{
  bar2(&bar);
  if (bar.a != 0x1)
    abort ();
  else if (!bar.c)
    abort ();
  else
    exit (0);
}
