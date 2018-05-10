/* { dg-do compile } */
/* { dg-options "-ffat-lto-objects" } */

/* Test lightweight barriers with __atomic_load routines for each
   valid memory model.  */

extern void abort(void);

int v, count;

int
main ()
{
  v = 0;
  count = 0;

  __atomic_store_n (&v, count + 1, __ATOMIC_RELAXED);
  if (v != ++count)
    abort ();

  __atomic_store_n (&v, count + 1, __ATOMIC_RELEASE);
  if (v != ++count)
    abort ();

  __atomic_store_n (&v, count + 1, __ATOMIC_SEQ_CST);
  if (v != ++count)
    abort ();

  /* Now test the generic variant.  */
  count++;

  __atomic_store (&v, &count, __ATOMIC_RELAXED);
  if (v != count++)
    abort ();

  __atomic_store (&v, &count, __ATOMIC_RELEASE);
  if (v != count++)
    abort ();

  __atomic_store (&v, &count, __ATOMIC_SEQ_CST);
  if (v != count)
    abort ();

  return 0;
}

/* One release for each store with release semantics.  */
/* { dg-final { scan-assembler-times "\tsync\t18" 2 } } */
/* Two full barriers for each store with seq-cst semantics.  */
/* { dg-final { scan-assembler-times "\tsync\t16" 4 } } */
