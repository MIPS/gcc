/* { dg-do run } */
/* { dg-options "-std=gnu99" } */

extern void abort (void);

#define OPERATE(OPRD1,OPRT,OPRD2,RLT)         \
do                                    \
{                                     \
  if (( (OPRD1) OPRT (OPRD2) )!= RLT)         \
    abort ();                                 \
}while(0)

#define DECIMAL_LOGICAL_OR(OPRD)      \
do                                    \
{                                     \
  OPRD = 1.0;                                 \
  OPERATE(1,||,OPRD,1);               \
  OPERATE(0,||,OPRD,1);               \
  OPERATE(OPRD,||,1,1);               \
  OPERATE(OPRD,||,0,1);               \
  OPRD = 0.0;                                 \
  OPERATE(1,||,OPRD,1);               \
  OPERATE(0,||,OPRD,0);               \
  OPERATE(OPRD,||,1,1);               \
  OPERATE(OPRD,||,0,0);               \
}while(0)

#define DECIMAL_LOGICAL_AND(OPRD)     \
do                                    \
{                                     \
  OPRD = 1.0;                                 \
  OPERATE(1,&&,OPRD,1);               \
  OPERATE(0,&&,OPRD,0);               \
  OPERATE(OPRD,&&,1,1);               \
  OPERATE(OPRD,&&,0,0);               \
  OPRD = 0.0;                                 \
  OPERATE(1,&&,OPRD,0);               \
  OPERATE(0,&&,OPRD,0);               \
  OPERATE(OPRD,&&,1,0);               \
  OPERATE(OPRD,&&,0,0);               \
}while(0)

#define DECIMAL_COMPOUND_ASSIGNMENT(TYPE, OPRD)       \
do                                                    \
{                                                     \
  _Decimal##TYPE _d##TYPE = OPRD;                     \
  /* Compound assignment of addition.  */             \
  OPERATE(OPRD,+=,1,(_d##TYPE+1));                    \
  _d##TYPE = OPRD;                                    \
  OPERATE(OPRD,+=,0,(_d##TYPE));                      \
  _d##TYPE = OPRD;                                    \
  OPERATE(OPRD,+=,-1,(_d##TYPE-1));                   \
  _d##TYPE = OPRD;                                    \
  OPERATE(OPRD,+=,1.25,(_d##TYPE+1.25));              \
  _d##TYPE = OPRD;                                    \
  OPERATE(OPRD,+=,1.2df,(_d##TYPE+1.2df));            \
  _d##TYPE = OPRD;                                    \
  OPERATE(OPRD,+=,2.8dd,(_d##TYPE+2.8dd));            \
  _d##TYPE = OPRD;                                    \
  OPERATE(OPRD,+=,4.7dl,(_d##TYPE+4.7dl));            \
  _d##TYPE = OPRD;                                    \
  /* Compound assignment of subtraction.  */          \
  OPERATE(OPRD,-=,1,(_d##TYPE-1));                    \
  _d##TYPE = OPRD;                                    \
  OPERATE(OPRD,-=,0,(_d##TYPE));                      \
  _d##TYPE = OPRD;                                    \
  OPERATE(OPRD,-=,-1,(_d##TYPE+1));                   \
  _d##TYPE = OPRD;                                    \
  OPERATE(OPRD,-=,1.25,(_d##TYPE-1.25));              \
  _d##TYPE = OPRD;                                    \
  OPERATE(OPRD,-=,1.2df,(_d##TYPE-1.2df));            \
  _d##TYPE = OPRD;                                    \
  OPERATE(OPRD,-=,2.8dd,(_d##TYPE-2.8dd));            \
  _d##TYPE = OPRD;                                    \
  OPERATE(OPRD,-=,4.7dl,(_d##TYPE-4.7dl));            \
}while(0)

int main()
{
  _Decimal32 d32;
  _Decimal64 d64;
  _Decimal128 d128;

  /* C99 Section 6.5.{13,14} Logical operator.  Constraints Each of the
     operands shall have scalar type.  DFP types would obey this.  */
  DECIMAL_LOGICAL_OR(d32);
  DECIMAL_LOGICAL_AND(d32);

  DECIMAL_LOGICAL_OR(d64);
  DECIMAL_LOGICAL_AND(d64);

  DECIMAL_LOGICAL_OR(d128);
  DECIMAL_LOGICAL_AND(d128);

  /* C99 Section 6.5.16.2 Compound assignment.  Verify the comound
     assignment operator for DFP types.  */
  DECIMAL_COMPOUND_ASSIGNMENT(32, d32);
  DECIMAL_COMPOUND_ASSIGNMENT(64, d64);
  DECIMAL_COMPOUND_ASSIGNMENT(128, d128);

  return 0;
}
