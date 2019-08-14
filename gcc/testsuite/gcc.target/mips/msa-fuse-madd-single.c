/* { dg-do run } */
/* { dg-options "-mhard-float -mmsa" } */
/* { dg-additional-options "-ffp-contract=fast" } */

#define VSIZE 8

typedef union { float f; unsigned long i; } float_ul_u;

struct test_vec {
  float_ul_u a;
  float_ul_u b;
  float_ul_u c;
} test_bench[VSIZE] = {
  {{.i=0x42963e5aul}, {.i=0xa0382c5ul}, {.i=0x8f2b15eul}},
  {{.i=0x1c695decul}, {.i=0x3fcfaed9ul}, {.i=0xf856867ul}},
  {{.i=0x116ae494ul}, {.i=0x3494b2fbul}, {.i=0xb13a31ul}},
  {{.i=0x683caad3ul}, {.i=0x313c7c99ul}, {.i=0x519eb94cul}},
  {{.i=0x4a9554feul}, {.i=0x392edbe4ul}, {.i=0x3d1a2dd9ul}},
  {{.i=0x4c4fff5bul}, {.i=0x51b76675ul}, {.i=0x59a4ba71ul}},
  {{.i=0x17cfc87dul}, {.i=0x5d66dc65ul}, {.i=0x30bb2b99ul}},
  {{.i=0x61c66e3ul}, {.i=0x69321f16ul}, {.i=0x2d96b714ul}},
};

int main (void)
{
  int i;
  float __attribute__((aligned(16))) av [VSIZE];
  float __attribute__((aligned(16))) bv[VSIZE];
  float __attribute__((aligned(16))) cv[VSIZE];
  float __attribute__((aligned(16))) res1[VSIZE];
  float __attribute__((aligned(16))) res2[VSIZE - 1];
  
  for (i = 0; i < VSIZE; i++)
    {
      av[i] = test_bench[i].a.f;
      bv[i] = test_bench[i].b.f;
      cv[i] = test_bench[i].c.f;
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
