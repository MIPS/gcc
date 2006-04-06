/* APPLE LOCAL file radar 4498373 */
/* Test for 'copies' attribute */
/* { dg-do compile { target *-*-darwin* } } */
/* { dg-options "-fobjc-abi-version=2" } */
/* { dg-skip-if "" { powerpc*-*-darwin* } { "-m64" } { "" } } */

#include <objc/Object.h>
@interface Person : Object
@property (copies, readonly) const char *name;
@end

@implementation Person
@property(copies, readonly, getter=_name) const char *name;

- (const char*)_name {
    return "MyName";
}
@end
/* { dg-final { scan-assembler ".long\t8\n\t.long\t1\n\t.long\t.*\n\t.long\t.*" } } */
/* { dg-final { scan-assembler ".ascii \"name\\\\0\"" } } */
/* { dg-final { scan-assembler ".ascii \"tr\\*,r,c,g_name\\\\0\"" } } */
