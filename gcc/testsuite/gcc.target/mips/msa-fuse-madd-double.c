/* { dg-do run } */
/* { dg-options "-mhard-float -mmsa" } */
/* { dg-additional-options "-ffp-contract=fast" } */

#define VSIZE 8

typedef union
{ double d; long long unsigned i; } double_ull_u;

struct test_vec {
  double_ull_u a;
  double_ull_u b;
  double_ull_u c;
} test_bench[VSIZE] = {
  {{.i=0x2c27173b4c9b0904ull}, {.i=0x6aa7b75c1df029d3ull}, {.i=0x5675ff363dd15094ull}},
  {{.i=0x3a6f0e78379a5b56ull}, {.i=0x53b735d529784870ull}, {.i=0x4cdced4c10a30d9cull}},
  {{.i=0x12d2eee56cc2b66aull}, {.i=0x60cd438558be66cdull}, {.i=0x335e9e8d425c189bull}},
  {{.i=0x680d29830daea0c2ull}, {.i=0x4c5977b52c0d49efull}, {.i=0x7305e21c2165c647ull}},
  {{.i=0x4e4add4115ecbebull}, {.i=0x401d6aed0c821feeull}, {.i=0x300832736663b62ull}},
  {{.i=0x1f6f475265504cc9ull}, {.i=0x4e5785aa042408acull}, {.i=0x2ab32c6b25521f4aull}},
  {{.i=0xd09c440443b602dull}, {.i=0x5f618fbb1fe650a2ull}, {.i=0x295aa9221841d645ull}},
  {{.i=0x732612c95a91b01full}, {.i=0x268678105b8f78b5ull}, {.i=0x5973c32a350e1c23ull}},
};

int main (void)
{
  int i;
  double __attribute__((aligned(16))) av [VSIZE];
  double __attribute__((aligned(16))) bv[VSIZE];
  double __attribute__((aligned(16))) cv[VSIZE];
  double __attribute__((aligned(16))) res1[VSIZE];
  double __attribute__((aligned(16))) res2[VSIZE - 1];
  
  for (i = 0; i < VSIZE; i++)
    {
      av[i] = test_bench[i].a.d;
      bv[i] = test_bench[i].b.d;
      cv[i] = test_bench[i].c.d;
    }

  for (i = 0; i < VSIZE; i++)
    res1[i] = av[i] * bv[i] + cv[i];

  for (i = 0; i < VSIZE - 1; i++)
    res2[i] = av[i] * bv[i] + cv[i];

  for (i = 0; i < VSIZE - 1; i++)
    if (res2[i] != res1[i])
      return 1;

  return 0;
}
