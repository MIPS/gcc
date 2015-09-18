/* { dg-do compile } */
void Foo ()
{
  __builtin_nvptx_lock (0);
  __builtin_nvptx_unlock (0);
}


/* { dg-final { scan-assembler-times ".atom.global.cas.b32" 2 } } */
/* { dg-final { scan-assembler ".global .u32 __global_lock;" } } */
/* { dg-final { scan-assembler-not ".shared .u32 __shared_lock;" } } */
