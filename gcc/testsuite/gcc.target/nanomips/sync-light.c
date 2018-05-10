/* { dg-do compile } */
/* { dg-options "-ffat-lto-objects" } */

/* Test the __sync interface for light-weight barriers.  */

signed char sc;
unsigned char uc;
unsigned int ui;

unsigned test (void)
{
  /* 21 RMW operations -> 21 acquires & 21 releases  */
  ui = __sync_fetch_and_add (&ui, 11);
  (void) __sync_fetch_and_add (&ui, 1);
  (void) __sync_fetch_and_sub (&ui, 1);
  (void) __sync_fetch_and_or (&ui, 1);
  (void) __sync_fetch_and_xor (&ui, 1);
  (void) __sync_fetch_and_and (&ui, 1);
  (void) __sync_fetch_and_nand (&ui, 1);
  ui = __sync_fetch_and_add (&ui, 11);
  ui = __sync_fetch_and_sub (&ui, 11);
  ui = __sync_fetch_and_or (&ui, 11);
  ui = __sync_fetch_and_xor (&ui, 11);
  ui = __sync_fetch_and_and (&ui, 11);
  ui = __sync_fetch_and_nand (&ui, 11);
  ui = __sync_add_and_fetch (&ui, uc);
  ui = __sync_sub_and_fetch (&ui, uc);
  ui = __sync_or_and_fetch (&ui, uc);
  ui = __sync_xor_and_fetch (&ui, uc);
  ui = __sync_and_and_fetch (&ui, uc);
  ui = __sync_nand_and_fetch (&ui, uc);
  ui = __sync_val_compare_and_swap (&ui, uc, sc);
  ui = __sync_bool_compare_and_swap (&ui, uc, sc);
  /* RMW operation with acquire only   */
  ui = __sync_lock_test_and_set (&ui, 1);
  /* Store with release only  */
  __sync_lock_release (&ui);
  /* Full barrier  */
  __sync_synchronize ();
  return ui;
}

/* Total acquire operations  21 + 1  */
/* { dg-final { scan-assembler-times "\tsync\t17" 22 } } */
/* Total release operations =  21 + 1  */
/* { dg-final { scan-assembler-times "\tsync\t18" 22 } } */
/* Full barrier  */
/* { dg-final { scan-assembler-times "\tsync\t16" 1 } } */
