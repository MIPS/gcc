/* { dg-do compile } */

int (*bar)();

void foo(){
  bar();
}

/* { dg-final { cleanup-tree-dump "gimple" } } */
