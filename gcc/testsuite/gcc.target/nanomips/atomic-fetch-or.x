/* Test lightweight barriers for __atomic_fetch_or builtins
   with each valid memory model.  */

extern void abort(void);

/* The fetch_op routines return the original value before the operation.  */
void
test_fetch_or (int op)
{
  _Atomic unsigned int v;
  __atomic_store_n (&v, 0, __ATOMIC_RELAXED);

  if (__atomic_fetch_or (&v, op, __ATOMIC_RELAXED) != 0)
    abort ();

  if (__atomic_fetch_or (&v, 1, __ATOMIC_CONSUME) != 2)
    abort ();

  if (__atomic_fetch_or (&v, op<<1, __ATOMIC_ACQUIRE) != 3)
    abort ();

  if (__atomic_fetch_or (&v, 255, __ATOMIC_RELEASE) != 7)
    abort ();

  if (__atomic_fetch_or (&v, op, __ATOMIC_ACQ_REL) != 255)
    abort ();

  if (__atomic_fetch_or (&v, 0xfff00000, __ATOMIC_SEQ_CST) != 255)
    abort ();

  if (__atomic_load_n (&v, __ATOMIC_RELAXED) != 0xfff000ff)
    abort ();
}
