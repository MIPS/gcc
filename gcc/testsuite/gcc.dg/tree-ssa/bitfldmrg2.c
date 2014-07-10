/* Check whether use of -fmerge-bitfields results in presence of
   overlapping unions results in incorrect code.  */
/* { dg-options "-O2 -fmerge-bitfields" }  */
/* { dg-do run } */
extern void abort (void);

struct s1
{
   unsigned f1:4;
   unsigned f2:4;
   unsigned f3:4;
};

struct s2
{
   unsigned char c;
   unsigned f1:4;
   unsigned f2:4;
   unsigned f3:4;
};

struct s3
{
   unsigned f1:3;
   unsigned f2:3;
   unsigned f3:3;
};

struct s4
{
   unsigned f0:3;
   unsigned f1:3;
   unsigned f2:3;
   unsigned f3:3;
};

union un_1
{
   struct s1 a;
   struct s2 b;
};

union un_2
{
   struct s3 a;
   struct s4 b;
};

void f1 (union un_1 *p1, union un_1 *p2)
{
   p2->a.f3 = p1->b.f3;
   p2->a.f2 = p1->b.f2;
   p2->a.f1 = p1->b.f1;

   if (p1->a.f1 != 3)
     abort ();
}

void f2 (union un_2 *p1, union un_2 *p2)
{
   p2->b.f1 = p1->a.f1;
   p2->b.f2 = p1->a.f2;
   p2->b.f3 = p1->a.f3;

   if (p2->b.f1 != 0 || p2->b.f2 != 0 || p2->b.f3 != 0)
     abort ();
}

int main ()
{
   union un_1 u1;
   union un_2 u2;

   u1.b.f1 = 1;
   u1.b.f2 = 2;
   u1.b.f3 = 3;
   u1.b.c = 0;

   f1 (&u1, &u1);

   u2.b.f0 = 0;
   u2.b.f1 = 1;
   u2.b.f2 = 2;
   u2.b.f3 = 3;

   f2 (&u2, &u2);

   return 0;
}
