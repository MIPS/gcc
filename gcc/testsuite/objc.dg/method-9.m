/* Check if finding multiple signatures for a method is handled gracefully
   when method lookup succeeds (see also method-7.m).  */
/* Contributed by Ziemowit Laski <zlaski@apple.com>  */
/* { dg-do compile } */

#include <objc/Object.h>

@protocol MyObject
- (id)initWithData:(Object *)data;
@end

@protocol SomeOther
- (id)initWithData:(int)data;
@end

@protocol MyCoding
- (id)initWithData:(id<MyObject, MyCoding>)data;
@end

@interface NTGridDataObject: Object <MyCoding>
{
    Object<MyCoding> *_data;
}
+ (NTGridDataObject*)dataObject:(id<MyObject, MyCoding>)data;
@end

@implementation NTGridDataObject
- (id)initWithData:(id<MyObject, MyCoding>)data {
  return data;
}
+ (NTGridDataObject*)dataObject:(id<MyObject, MyCoding>)data
{
    NTGridDataObject *result = [[NTGridDataObject alloc] initWithData:data];
     /* { dg-warning "multiple methods named .\\-initWithData:. found" "" { target *-*-* } 33 } */
     /* { dg-warning "could be using .\\-\\(id\\)initWithData:\\(Object \\*\\)data." "" { target *-*-* } 9 } */
     /* { dg-warning "or .\\-\\(id\\)initWithData:\\(id <MyObject, MyCoding>\\)data." "" { target *-*-* } 17 } */
     /* { dg-warning "or .\\-\\(id\\)initWithData:\\(int\\)data." "" { target *-*-* } 13 } */
     /* { dg-warning "\\(Messages matching multiple method signatures" "" { target *-*-* } 33 } */
     /* { dg-warning "will be assumed to return .id. and accept" "" { target *-*-* } 33 } */
     /* { dg-warning ".\.\.\.. as arguments\.\\)" "" { target *-*-* } 33 } */

    return result;
}
@end
