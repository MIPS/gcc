/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-ivopts-details -fdump-tree-optimized" } */

typedef unsigned long long uint64_t;
typedef unsigned char uint8_t;

uint8_t *data;
unsigned length;
long last_data_offset;

f ()
{
  uint8_t *input_pointer = data, *input_limit = data + length;
  uint64_t next_byte, byte;
  byte = *input_pointer;
  for (input_pointer++; input_pointer!=input_limit;
       input_pointer++, byte=next_byte)  {
    next_byte = *input_pointer;
    g (byte, next_byte);
  }
}

/* { dg-final { scan-tree-dump "number of iterations \[^\\n\]+ (- 2B?|\\+ 2B?\\))\\n" "ivopts" } }  */
/* { dg-final { scan-tree-dump-times "input_limit_\[0-9\]* != input_pointer_\[0-9\]*" 2 "optimized" } }  */
/* { dg-final { cleanup-tree-dump "ivopts" } }  */
/* { dg-final { cleanup-tree-dump "optimized" } }  */
