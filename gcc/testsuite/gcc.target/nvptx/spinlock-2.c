/* { dg-do compile } */
void Foo ()
{
  __builtin_nvptx_lock (1);
  __builtin_nvptx_unlock (1);
}

/* { dg-final { scan-assembler-times ".atom.shared.cas.b32" 2 } } */
/* { dg-final { scan-assembler ".shared .u32 __shared_lock;" } } */
/* { dg-final { scan-assembler-not ".global .u32 __global_lock;" } } */
