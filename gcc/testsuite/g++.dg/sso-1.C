// { dg-do compile }

int i;

#ifdef __ORDER_LITTLE_ENDIAN__
struct __attribute__((scalar_storage_order("big-endian"))) Rec
{
  int *p;
};
#else
struct __attribute__((scalar_storage_order("little-endian"))) Rec
{
  int *p;
};
#endif

struct Rec r = { &i };
