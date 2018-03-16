/* { dg-do compile } */
/* { dg-options "-ffat-lto-objects" } */

/* Test lightweight barriers with __atomic_*_fence routines for each
   valid memory model.  */

int
main ()
{
  __atomic_thread_fence (__ATOMIC_RELAXED);
  __atomic_thread_fence (__ATOMIC_CONSUME);
  __atomic_thread_fence (__ATOMIC_ACQUIRE);
  __atomic_thread_fence (__ATOMIC_RELEASE);
  __atomic_thread_fence (__ATOMIC_ACQ_REL);
  __atomic_thread_fence (__ATOMIC_SEQ_CST);

  __atomic_signal_fence (__ATOMIC_RELAXED);
  __atomic_signal_fence (__ATOMIC_CONSUME);
  __atomic_signal_fence (__ATOMIC_ACQUIRE);
  __atomic_signal_fence (__ATOMIC_RELEASE);
  __atomic_signal_fence (__ATOMIC_ACQ_REL);
  __atomic_signal_fence (__ATOMIC_SEQ_CST);

  return 0;
}

/* One acquire fence for each: acquire, consume.  */
/* { dg-final { scan-assembler-times "\tsync\t17" 2 } } */
/* One release fence for each: release.  */
/* { dg-final { scan-assembler-times "\tsync\t18" 1 } } */
/* One full barrier for each: acq-rel, sec-cst.  */
/* { dg-final { scan-assembler-times "\tsync\t16" 2 } } */
