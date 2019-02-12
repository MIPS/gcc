/* Test lightweight barriers for __atomic_fetch_xor builtins
   with each valid memory model.  */

extern void abort(void);

/* The fetch_op routines return the original value before the operation.  */
void
test_fetch_xor (int op)
{
  _Atomic unsigned int v;
  __atomic_store_n (&v, 0, __ATOMIC_RELAXED);

  if (__atomic_fetch_xor (&v, op, __ATOMIC_RELAXED) != 0)
    abort ();

  if (__atomic_fetch_xor (&v, 1, __ATOMIC_CONSUME) != 2)
    abort ();

  if (__atomic_fetch_xor (&v, op, __ATOMIC_ACQUIRE) != 3)
    abort ();

  if (__atomic_fetch_xor (&v, 255, __ATOMIC_RELEASE) != 1)
    abort ();

  if (__atomic_fetch_xor (&v, op, __ATOMIC_ACQ_REL) != 254)
    abort ();

  if (__atomic_fetch_xor (&v, 0xfff00000, __ATOMIC_SEQ_CST) != 252)
    abort ();

  if (__atomic_load_n (&v, __ATOMIC_RELAXED) != 0xfff000fc)
    abort ();
}
