/* { dg-options "-std=gnu99 -O0" } */

/* N1150 5.1 Conversion between decimal floating types and integer.
   C99 6.3.1.4(1a) New.  */

extern void abort (void);
int failures;

#ifdef DBG
extern int printf (const char *, ...);
#define FAILURE { printf ("failed at line %d\n", __LINE__); failures++; }
#else
#define FAILURE abort ();
#endif

_Decimal32 d32;
_Decimal64 d64;
_Decimal128 d128;
unsigned int ui;
unsigned long ul;
unsigned long long ull;
int si;
long sl;
long long sll;
_Bool b;

void
init_dfp_1 (void)
{
  d32 = 456.789df;
  d64 = 23.556789dd;
  d128 = 1234.5678dl;
}
void
init_dfp_2 (void)
{
  d32 = 1.83df;
  d64 = -3.6dd;
  d128 = 0.00003dl;
}

void
init_dfp_3 (void)
{
  d32 = 0.0DF;
  d64 = 0.0DD;
  d128 = 0.0DL;
}

void
init_dfp_4 (void)
{
  /* Values between INT_MAX and UINT_MAX.  */
  d32 = 3456789000.df;
  d64 = 3456789012.dd;
  d128 = 3456789012.dl;
}

void
init_dfp_5 (void)
{
  /* Values between LLONG_MAX and ULLONG_MAX.  */
  d32 = 12345670000000000000.df;
  d64 = 12345678901234500000.dd;
  d128 = 12345678901234567891.dl;
}

void
init_unsigned_int_1 (void)
{
  ui = 987U;
  ul = 345678UL;
  ull = 1234567ULL;
}

void
init_unsigned_int_2 (void)
{
  /* Value between INT_MAX and UINT_MAX.  */
  ui = 3456789000U;
  /* Value between LLONG_MAX and ULLONG_MAX.  */
  ull = 12345670000000000000ULL;
}

void
init_signed_int (void)
{
  si = -987;
  sl = -345678;
  sll = -1234567;
}

int
main ()
{
  /* C99 Section 6.7.2 Type specifiers.  Type _Bool is 
     mentioned in this section.  Conversions between 
     _Bool and DFP types.  */

  /* Decimal float to unsigned integer.  */
  init_dfp_1 ();

  ui = d32;
  if (ui != 456U)
    FAILURE
  ul = d32;
  if (ul != 456UL)
    FAILURE
  ull = d32;
  if (ull != 456ULL)
    FAILURE

  ui = d64;
  if (ui != 23U)
    FAILURE
  ul = d64;
  if (ul != 23UL)
    FAILURE
  ull = d64;
  if (ull != 23ULL)
    FAILURE

  ui = d128;
  if (ui != 1234U)
    FAILURE
  ul = d128;
  if (ul != 1234UL)
    FAILURE
  ull = d128;
  if (ull != 1234ULL)
    FAILURE

  init_dfp_4 ();
  ui = d32;
  if (ui != 3456789000U)
    FAILURE
  ui = d64;
  if (ui != 3456789012U)
    FAILURE
  ui = d128;
  if (ui != 3456789012U)
    FAILURE

  init_dfp_5 ();
  ull = d32;
  if (ull != 12345670000000000000ULL)
    FAILURE
  ull = d64;
  if (ull != 12345678901234500000ULL)
    FAILURE
  ull = d128;
  if (ull != 12345678901234567891ULL)
    FAILURE

  /* Decimal float to signed integer.  */

  init_dfp_1 ();

  si = d32;
  if (si != 456)
    FAILURE
  sl = d32;
  if (sl != 456L)
    FAILURE
  sll = d32;
  if (sll != 456LL)
    FAILURE

  si = d64;
  if (si != 23)
    FAILURE
  sl = d64;
  if (sl != 23L)
    FAILURE
  sll = d64;
  if (sll != 23LL)
    FAILURE

  d64 = -d64;
  si = d64;
  if (si != -23)
    FAILURE
  sl = d64;
  if (sl != -23L)
    FAILURE
  sll = d64;
  if (sll != -23LL)
    FAILURE

  si = d128;
  if (si != 1234)
    FAILURE
  sl = d128;
  if (sl != 1234L)
    FAILURE
  sll = d128;
  if (sll != 1234LL)
    FAILURE

  d128 = -d128;
  si = d128;
  if (si != -1234)
    FAILURE
  sl = d128;
  if (sl != -1234L)
    FAILURE
  sll = d128;
  if (sll != -1234LL)
    FAILURE

  /* Decimal float to _Bool.  */
  init_dfp_2 ();

  b = d32;
  if (!b)
    FAILURE
  b = d64;
  if (!b)
    FAILURE
  b = d128;
  if (!b)
    FAILURE

  /* Unsigned integer to decimal float.  */
  init_unsigned_int_1 ();

  d32 = ui;
  if (d32 != 987.0df)
    FAILURE
  d32 = ul;
  if (d32 != 345678.0df)
    FAILURE
  d32 = ull;
  if (d32 != 1234567.df)
    FAILURE

  d64 = ui;
  if (d64 != 987.0dd)
    FAILURE
  d64 = ul;
  if (d64 != 345678.0dd)
    FAILURE
  d64 = ull;
  if (d64 != 1234567.dd)
    FAILURE

  d128 = ui;
  if (d128 != 987.0dl)
    FAILURE
  d128 = ul;
  if (d128 != 345678.0dl)
    FAILURE
  d128 = ull;
  if (d128 != 1234567.dl)
    FAILURE

  init_unsigned_int_2 ();
  d32 = ui;
  if (d32 != 3456789.e3df)
    FAILURE
  d32 = ull;
  if (d32 != 1234567.e13df)
    FAILURE

  d64 = ui;
  if (d64 != 3456789.e3dd)
    FAILURE
  d64 = ull;
  if (d64 != 1234567.e13dd)
    FAILURE

  d128 = ui;
  if (d128 != 3456789.e3dl)
    FAILURE
  d128 = ull;
  if (d128 != 1234567.e13dl)
    FAILURE

  /* Signed integer to decimal float.  */
  init_signed_int ();

  d32 = si;
  if (d32 != -987.0df)
    FAILURE
  d32 = sl;
  if (d32 != -345678.0df)
    FAILURE
  d32 = sll;
  if (d32 != -1234567.df)
    FAILURE

  d64 = si;
  if (d64 != -987.0dd)
    FAILURE
  d64 = sl;
  if (d64 != -345678.0dd)
    FAILURE
  d64 = sll;
  if (d64 != -1234567.dd)
    FAILURE

  d128 = si;
  if (d128 != -987.0dl)
    FAILURE
  d128 = sl;
  if (d128 != -345678.0dl)
    FAILURE
  d128 = sll;
  if (d128 != -1234567.dl)
    FAILURE

  /* _Bool to decimal float.  */
  init_dfp_3 ();
  
  b = d32;
  if (b)
    FAILURE
  b = d64;
  if (b)
    FAILURE
  b = d128;
  if (b)
    FAILURE

  if (failures != 0)
    abort ();

  return 0;
}
