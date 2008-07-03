/* { dg-do run } */
/* { dg-options "-w" } */
#define vector __attribute__((vector_size(sizeof(int)*4) ))

/* Check to make sure that we extract and insert the vector at the same
   location for vector subscripting and that vectors layout are the same
   as arrays. */

struct TV4
{
    vector int v;
};

typedef struct TV4 MYV4;
template<int i>
static inline int *f(MYV4 *a)
{
  return &(a->v[i]);
}
static inline MYV4 myfunc2( int x, int y, int z, int w )
{
    MYV4 temp;
    *f<0>(&temp) = x;
    *f<1>(&temp) = y;
    *f<2>(&temp) = z;
    *f<3>(&temp) = w;
    return temp;
}

MYV4 val3;

__attribute__((noinline)) void modify (void) 
{
    val3 = myfunc2( 1, 2, 3, 4 );
}

int main( int argc, char* argv[] )
{
  int a[4];
  int i;
  
  modify();
  
  if (*f<0>(&val3) != 1)
    __builtin_abort ();
  if (*f<1>(&val3) != 2)
    __builtin_abort ();
  if (*f<2>(&val3) != 3)
    __builtin_abort ();
  if (*f<3>(&val3) != 4)
    __builtin_abort ();
    
  __builtin_memcpy(a, &val3, 16);
  for(i = 0; i < 4; i++)
    if (a[i] != i+1)
      __builtin_abort ();
  
  
  return 0;
}
