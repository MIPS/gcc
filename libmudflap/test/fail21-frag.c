int foo = 5;
int *bar = & foo;
/* Make an access here to get &foo into the lookup cache.  */
*bar = 5;
__mf_watch (& foo, sizeof(foo));
/* This access should trigger the watch violation.  */
*bar = 10;
