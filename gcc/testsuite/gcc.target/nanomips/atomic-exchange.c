/* { dg-do compile } */
/* { dg-options "-ffat-lto-objects" } */

/* Test lightweight barriers with __atomic_exchange routines for each
   valid memory model.  */

extern void abort(void);

int v, count, ret;

int
main ()
{
  v = 0;
  count = 0;

  if (__atomic_exchange_n (&v, count + 1, __ATOMIC_RELAXED) != count)
    abort ();
  count++;

  if (__atomic_exchange_n (&v, count + 1, __ATOMIC_ACQUIRE) != count)
    abort ();
  count++;

  if (__atomic_exchange_n (&v, count + 1, __ATOMIC_RELEASE) != count)
    abort ();
  count++;

  if (__atomic_exchange_n (&v, count + 1, __ATOMIC_ACQ_REL) != count)
    abort ();
  count++;

  if (__atomic_exchange_n (&v, count + 1, __ATOMIC_SEQ_CST) != count)
    abort ();
  count++;

  /* Now test the generic version.  */

  count++;

  __atomic_exchange (&v, &count, &ret, __ATOMIC_RELAXED);
  if (ret != count - 1 || v != count)
    abort ();
  count++;

  __atomic_exchange (&v, &count, &ret, __ATOMIC_ACQUIRE);
  if (ret != count - 1 || v != count)
    abort ();
  count++;

  __atomic_exchange (&v, &count, &ret, __ATOMIC_RELEASE);
  if (ret != count - 1 || v != count)
    abort ();
  count++;

  __atomic_exchange (&v, &count, &ret, __ATOMIC_ACQ_REL);
  if (ret != count - 1 || v != count)
    abort ();
  count++;

  __atomic_exchange (&v, &count, &ret, __ATOMIC_SEQ_CST);
  if (ret != count - 1 || v != count)
    abort ();
  count++;

  return 0;
}

/* One acquire fence for each: acquire, acq_rel.  */
/* { dg-final { scan-assembler-times "\tsync\t17" 4 } } */
/* One release fence for each: release, acq_rel.  */
/* { dg-final { scan-assembler-times "\tsync\t18" 4 } } */
/* Each RMW with sequentially consistent semantics gets 2 full barriers.  */
/* { dg-final { scan-assembler-times "\tsync\t16" 4 } } */
