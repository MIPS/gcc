int foo[10];
int *bar = & foo[3];
/* Watching occurs at the object granularity, which is in this case
   the entire array.  */
__mf_watch (& foo[1], sizeof(foo[1]));
__mf_unwatch (& foo[6], sizeof(foo[6]));
*bar = 10;
