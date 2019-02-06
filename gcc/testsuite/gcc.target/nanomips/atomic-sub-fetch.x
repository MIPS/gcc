/* Test lightweight barriers for __atomic_sub_fetch builtins
   with each valid memory model.  */

extern void abort(void);

/* The fetch_op routines return the result of the operation.  */

void
test_sub_fetch (int op)
{
  _Atomic int v;
  __atomic_store_n (&v, 0, __ATOMIC_RELAXED);

  if (__atomic_sub_fetch (&v, op, __ATOMIC_RELAXED) != -1)
    abort ();

  if (__atomic_sub_fetch (&v, 1, __ATOMIC_CONSUME) != -2)
    abort ();

  if (__atomic_sub_fetch (&v, op, __ATOMIC_ACQUIRE) != -3)
    abort ();

  if (__atomic_sub_fetch (&v, 134, __ATOMIC_RELEASE) != -137)
    abort ();

  if (__atomic_sub_fetch (&v, op, __ATOMIC_ACQ_REL) != -138)
    abort ();

  if (__atomic_sub_fetch (&v, 14567, __ATOMIC_SEQ_CST) != -14705)
    abort ();

  if (__atomic_load_n (&v, __ATOMIC_RELAXED) != -14705)
    abort ();
}
