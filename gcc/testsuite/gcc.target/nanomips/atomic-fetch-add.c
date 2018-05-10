/* { dg-do compile } */
/* { dg-options "-ffat-lto-objects" } */

/* Test lightweight barriers for __atomic_fetch_add builtins
   with each valid memory model.  */

extern void abort(void);

int v, count, res;
const int init = ~0;

/* The fetch_op routines return the original value before the operation.  */

void
test_fetch_add ()
{
  v = 0;
  count = 1;

  if (__atomic_fetch_add (&v, count, __ATOMIC_RELAXED) != 0)
    abort ();

  if (__atomic_fetch_add (&v, 1, __ATOMIC_CONSUME) != 1)
    abort ();

  if (__atomic_fetch_add (&v, count, __ATOMIC_ACQUIRE) != 2)
    abort ();

  if (__atomic_fetch_add (&v, 1, __ATOMIC_RELEASE) != 3)
    abort ();

  if (__atomic_fetch_add (&v, count, __ATOMIC_ACQ_REL) != 4)
    abort ();

  if (__atomic_fetch_add (&v, 1, __ATOMIC_SEQ_CST) != 5)
    abort ();
}

/* One acquire fence for each RWM: consume, acquire, acq-rel */
/* { dg-final { scan-assembler-times "\tsync\t17" 3 } } */
/* One release fence for each RWM: acquire,  acq-rel */
/* { dg-final { scan-assembler-times "\tsync\t18" 2 } } */
/* RWM with SEQ_CST semantics gets 2 full barriers.  */
/* { dg-final { scan-assembler-times "\tsync\t16" 2 } } */
