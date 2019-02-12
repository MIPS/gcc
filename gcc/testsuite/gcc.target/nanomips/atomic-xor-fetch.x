/* Test lightweight barriers for __atomic_xor_fetch builtins
   with each valid memory model.  */

extern void abort(void);

/* The fetch_op routines return the result of the operation.  */
void
test_xor_fetch (int op)
{
  _Atomic unsigned int v;
  __atomic_store_n (&v, 0, __ATOMIC_RELAXED);

  if (__atomic_xor_fetch (&v, op, __ATOMIC_RELAXED) != 2)
    abort ();

  if (__atomic_xor_fetch (&v, 1, __ATOMIC_CONSUME) != 3)
    abort ();

  if (__atomic_xor_fetch (&v, op, __ATOMIC_ACQUIRE) != 1)
    abort ();

  if (__atomic_xor_fetch (&v, 255, __ATOMIC_RELEASE) != 254)
    abort ();

  if (__atomic_xor_fetch (&v, op, __ATOMIC_ACQ_REL) != 252)
    abort ();

  if (__atomic_xor_fetch (&v, 0xfff00000, __ATOMIC_SEQ_CST) != 0xfff000fc)
    abort ();

  if (__atomic_load_n (&v, __ATOMIC_RELAXED) != 0xfff000fc)
    abort ();
}
