/* Check if finding multiple signatures for a method is handled gracefully
   when method lookup succeeds (see also method-7.m).  */
/* Contributed by Ziemowit Laski <zlaski@apple.com>  */
/* { dg-do compile } */

#include <objc/Object.h>

@protocol MyObject
- (id)initWithData:(Object *)data;
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
    return result;
}
@end
