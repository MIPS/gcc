/* Test lightweight barriers for __atomic_and_fetch builtins
   with each valid memory model.  */

extern void abort(void);

/* The op_fetch routines return the result of the operation.  */
void
test_and_fetch (unsigned int op)
{
  _Atomic unsigned int v;
  __atomic_store_n (&v, ~0, __ATOMIC_RELAXED);

  if (__atomic_and_fetch (&v, op, __ATOMIC_RELAXED) != ~2)
    abort ();

  if (__atomic_and_fetch (&v, ~1, __ATOMIC_CONSUME) != ~3)
    abort ();

  if (__atomic_and_fetch (&v, op << 1, __ATOMIC_ACQUIRE) != ~7)
    abort ();

  if (__atomic_and_fetch (&v, ~255, __ATOMIC_RELEASE) != ~255)
    abort ();

  if (__atomic_and_fetch (&v, op, __ATOMIC_ACQ_REL) != ~255)
    abort ();

  if (__atomic_and_fetch (&v, 0xfff00000, __ATOMIC_SEQ_CST) != 0xfff00000)
    abort ();

  if (__atomic_load_n (&v, __ATOMIC_RELAXED) != 0xfff00000)
    abort ();
}
