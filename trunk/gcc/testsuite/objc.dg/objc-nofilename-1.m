/* APPLE LOCAL file 4327263 */
/* Test to make sure that file name does not appear in the binary. */
/* { dg-do compile } */

#import <Foundation/Foundation.h>
int main(int argc, char **argv)
{
     [NSObject class];
     fprintf(stderr, "foo\n");
     return 0;
}

/* { dg-final { scan-assembler-not "objc-nofilename-1.m" } } */
