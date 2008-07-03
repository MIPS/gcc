/* { dg-do compile } */
// { dg-options "" }

#define vector __attribute__((vector_size(16) ))

template<int i>
static inline int f(vector int a)
{
  return (a[i]);
}
int myfunc2( int x, int y, int z, int w )
{
    vector int temp = (vector int){x, y, z, w};
    return f<0>(temp);
}
