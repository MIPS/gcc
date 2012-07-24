/* { dg-do run { target *-octeon-* } } */
/* { dg-options "-O2" } */

unsigned long long offset = 7; 
unsigned long long block_id = 1; 

int main()
{
  unsigned long long r = 0x80016F0010000508ull + (offset&7)*32 +
    (block_id&1)*0x100000000000ull;
  unsigned long long r2 = 0x80016F0010000508ull + (7&7)*32 +
    (1&1)*0x100000000000ull;
  return r == r2 ? 0 : 1;
} 
