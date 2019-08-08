typedef signed char v16i8 __attribute__ ((vector_size(16)));
typedef short v8i16 __attribute__ ((vector_size(16)));
typedef int v4i32 __attribute__ ((vector_size(16)));
typedef long long v2i64 __attribute__ ((vector_size(16)));
typedef unsigned char v16u8 __attribute__ ((vector_size(16)));
typedef unsigned short v8u16 __attribute__ ((vector_size(16)));
typedef unsigned int v4u32 __attribute__ ((vector_size(16)));
typedef unsigned long long v2u64 __attribute__ ((vector_size(16)));
typedef float v4f32 __attribute__ ((vector_size(16)));
typedef double v2f64 __attribute__ ((vector_size(16)));


#define ITERATE_FOR_ALL_TYPES(FUNC) \
  FUNC (v16i8, 16) \
  FUNC (v8i16, 8) \
  FUNC (v4i32, 4) \
  FUNC (v2i64, 2) \
  FUNC (v16u8, 16) \
  FUNC (v8u16, 8) \
  FUNC (v4u32, 4) \
  FUNC (v2u64, 2) \
  FUNC (v4f32, 4) \
  FUNC (v2f64, 2)
