/* Test support of scalar_storage_order attribute */

/* { dg-do compile } */

struct S1
{
  int i;
};

typedef struct S1 __attribute__((scalar_storage_order("big-endian"))) NS1; /* { dg-warning "attribute ignored" } */

struct S2
{
  int i;
};

typedef struct S2 __attribute__((scalar_storage_order("little-endian"))) NS2; /* { dg-warning "attribute ignored" } */

struct S3 { int i; };

typedef struct S3 __attribute__((scalar_storage_order("other"))) NS3; /* { dg-warning "attribute ignored" } */
