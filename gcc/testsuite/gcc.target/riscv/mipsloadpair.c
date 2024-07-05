/* { dg-do compile } */
/* { dg-options "-fpointer-chasing -march=rv64gc_xmipslsp -mtune=mips-p8700 -mno-double-align" { target { rv64 } } } */
/* { dg-skip-if "" { *-*-* } { "-O0" "-Os" "-O1" "-Og" "-g" "-Oz" } } */

typedef struct mylist
{
  struct mylist *next;
  long value;
} MYLIST;

MYLIST *test1(MYLIST *start, long value)
{
  while (start && start->value != value)
    start = start->next;
 
  return start;
}

/* { dg-final { scan-assembler-times "ldp" 1 } } */
