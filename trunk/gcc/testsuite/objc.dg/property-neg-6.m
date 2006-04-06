/* APPLE LOCAL file radar 4498373 */
/* Test for 'copies' attribute */
/* { dg-do compile { target *-*-darwin* } } */
/* { dg-options "-fobjc-abi-version=2" } */
/* { dg-skip-if "" { powerpc*-*-darwin* } { "-m64" } { "" } } */

#include <objc/Object.h>
@interface Person : Object
@property (readonly) const char *name;
@end

@implementation Person
@property(copies, readonly, getter=_name) const char *name; /* { dg-error "property \\'name\\' has conflicting \\'copies\\' attribute with its interface version" } */

- (const char*)_name {
    return "MyName";
}
@end
