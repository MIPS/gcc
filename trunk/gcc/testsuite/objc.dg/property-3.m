/* APPLE LOCAL file radar 4436866 */
/* This program tests use of properties . */
/* { dg-options "-framework Foundation -fobjc-exceptions" } */
/* { dg-do run { target *-*-darwin* } } */

#include <Foundation/Foundation.h>

@interface Person : NSObject
{
}
@property NSString *firstName, *lastName;
@property(readonly) NSString *fullName;

@end

@interface Group : NSObject
{
}

@property Person *techLead, *runtimeGuru, *propertiesMaven;

@end

@implementation Group

@property Person *techLead, *runtimeGuru, *propertiesMaven;
- init {
  techLead = [[Person alloc] init];
  runtimeGuru = [[Person alloc] init];
  propertiesMaven = [[Person alloc] init];
  return self;
}

@end

@implementation Person

@property NSString *firstName, *lastName;
@property(readonly, getter = fullName) NSString *fullName;

- (NSString*)fullName { // computed getter
    return [NSString stringWithFormat:@"%@ %@", firstName, lastName];
}

@end

NSString *playWithProperties()
{
  Group *g = [[Group alloc] init] ;

  g.techLead.firstName = @"Blaine";
  g.techLead.lastName = @"Garst";
  g.runtimeGuru.firstName = @"Greg";
  g.runtimeGuru.lastName = @"Parker";
  g.propertiesMaven.firstName = @"Patrick";
  g.propertiesMaven.lastName = @"Beard";

  return [NSString stringWithFormat:@"techlead %@ runtimeGuru %@ propertiesMaven %@",
                        g.techLead.fullName, g.runtimeGuru.fullName, g.propertiesMaven.fullName];
}

main()
{
    char buf [256];
    NSAutoreleasePool* pool  = [[NSAutoreleasePool alloc] init];
    sprintf(buf, "%s", [playWithProperties() cString]);
    [pool release];
    return strcmp (buf, "techlead Blaine Garst runtimeGuru Greg Parker propertiesMaven Patrick Beard");
}

