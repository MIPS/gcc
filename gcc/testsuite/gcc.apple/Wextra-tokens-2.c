/* APPLE LOCAL file -Wextra-tokens */

/* { dg-do preprocess } */
/* { dg-options "-fno-show-column -Wextra-tokens -std=c99 -pedantic" } */

/* Tests that -Wextra-tokens, specially after #elif. Radar 3909961. */

int
main()
{
#ifdef  hi there  /* { dg-warning "extra tokens" } */
#endif
#ifndef hi there  /* { dg-warning "extra tokens" } */
#elif   hi there  /* { dg-warning "extra tokens" } */
#endif  hi there  /* { dg-warning "extra tokens" } */
#undef  hi there  /* { dg-warning "extra tokens" } */
  return 0;
}
