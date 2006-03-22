/* APPLE LOCAL file radar 4436866 */
/* This program checks for proper use of 'readonly' attribute. */
/* { dg-do compile { target *-*-darwin* } } */

@interface Bar
{
  int iVar;
}
@property (readonly) int FooBar;
@end

@implementation Bar
@property int FooBar; /* { dg-error "property \\'FooBar\\' has conflicting \\'readyonly\\' attribute with its interface version" } */

@end
