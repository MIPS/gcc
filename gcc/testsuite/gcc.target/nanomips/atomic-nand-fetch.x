/* Test lightweight barriers for __atomic_nand_fetch builtins
   with each valid memory model.  */

extern void abort(void);

/* The fetch_op routines return the result of the operation.  */
void
test_nand_fetch (int op)
{
  _Atomic int v;
  __atomic_store_n (&v, 0, __ATOMIC_RELAXED);

  if (__atomic_nand_fetch (&v, op, __ATOMIC_RELAXED) != ~0)
    abort ();

  if (__atomic_nand_fetch (&v, ~1, __ATOMIC_CONSUME) != 1)
    abort ();

  if (__atomic_nand_fetch (&v, op, __ATOMIC_ACQUIRE) != ~0)
    abort ();

  if (__atomic_nand_fetch (&v, ~255, __ATOMIC_RELEASE) != 255)
    abort ();

  if (__atomic_nand_fetch (&v, op, __ATOMIC_ACQ_REL) != ~2)
    abort ();

  if (__atomic_nand_fetch (&v, 0xfff00000, __ATOMIC_SEQ_CST) != 0xfffff)
    abort ();

  if (__atomic_load_n (&v, __ATOMIC_RELAXED) != 0xfffff)
    abort ();
}
