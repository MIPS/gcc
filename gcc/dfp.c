#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "toplev.h"
#include "real.h"
#include "tm_p.h"
#include "dfp.h"

/* The order of the following headers is important for making sure 
   decNumber structure is large enough to hold decimal128 digits. */

#include "decimal128.h"
#include "decimal64.h"
#include "decimal32.h"
#include "decNumber.h"
#include "decDPD.h"


/* Not thread safe, but useful otherwise. */
static decNumber dn;
static decContext set;

/* Create decimal encoded r from string. */
static void
decimal_from_string (REAL_VALUE_TYPE *r, const char *s)
{
  memset (r, 0, sizeof(r));

  decContextDefault(&set, DEC_INIT_DECIMAL128);
  set.traps=0;
  decNumberFromString(&dn, (char *)s, &set);

  /* Encoding as a decimal128 is more compact but this is a HUGE HACK. */
  decimal128FromNumber((decimal128 *)r->sig, &dn, &set);

  /* Get unbiased exponent. */
  /* FIXME: check whether this really always fits.*/
  SET_REAL_EXP(r, dn.exponent);
  if (decNumberIsNegative(&dn))
    r->sign = 1;
  r->cl = rvc_normal;
  if (decNumberIsZero(&dn))
    r->cl = rvc_zero;
  if (decNumberIsNaN(&dn))
    r->cl = rvc_nan;
  if (decNumberIsInfinite(&dn))
    r->cl = rvc_inf;
}

/* Wrapper to handle decimal strings too.  
   FIXME:  Should be able to accomiplish this with 
   fmt->real_from_sting(). */
void
decimal_real_from_string (REAL_VALUE_TYPE *r, const char *s, 
			  enum machine_mode mode)
{
  if (mode != SDmode && mode != DDmode && mode != TDmode) {
      real_from_string (r, s);
  } else
    decimal_from_string (r, s);
}

void 
encode_decimal32 (const struct real_format *fmt ATTRIBUTE_UNUSED,
		  long *buf, const REAL_VALUE_TYPE *r)
{
  decimal32 d32;

  decContextDefault(&set, DEC_INIT_DECIMAL128);
  set.traps=0;

  decimal128ToNumber((decimal128 *)r->sig, &dn);

  /* Still in intermediate representation, so sign
     is seperate from encoding. */
  /* FIXME: This is WRONG place.  Should come through
     REAL_NEGATE path. */
  if (r->sign != decNumberIsNegative(&dn))
    decNumberNegate(&dn);

  decimal32FromNumber(&d32, &dn, &set);

  buf[0] = *(long *)d32.bytes;
}

void decode_decimal32 (const struct real_format *fmt ATTRIBUTE_UNUSED,
		       REAL_VALUE_TYPE *r ATTRIBUTE_UNUSED,
		       const long *buf ATTRIBUTE_UNUSED)
{  
  /* FIXME: Do something. */
}
void encode_decimal64 (const struct real_format *fmt ATTRIBUTE_UNUSED,
		       long *buf, const REAL_VALUE_TYPE *r)
{
  decimal64 d64;

  decContextDefault(&set, DEC_INIT_DECIMAL128);
  set.traps=0;

  decimal128ToNumber((decimal128 *)r->sig, &dn);

  /* Still in intermediate representation, so sign
     is seperate from encoding. */
  /* FIXME: This is WRONG place.  Should come through
     REAL_NEGATE path. */
  if (r->sign != decNumberIsNegative(&dn))
    decNumberNegate(&dn);

  decimal64FromNumber(&d64, &dn, &set);

  if (FLOAT_WORDS_BIG_ENDIAN)
    {
      buf[0] = *(long *)&d64.bytes[0];
      buf[1] = *(long *)&d64.bytes[4];
    }
  else
    {
      buf[1] = *(long *)&d64.bytes[0];
      buf[0] = *(long *)&d64.bytes[4];
    }
}

void decode_decimal64 (const struct real_format *fmt ATTRIBUTE_UNUSED,
		       REAL_VALUE_TYPE *r ATTRIBUTE_UNUSED,
		       const long *buf ATTRIBUTE_UNUSED)
{
  /* FIXME: Do something. */
}

void encode_decimal128 (const struct real_format *fmt ATTRIBUTE_UNUSED,
			long *buf, const REAL_VALUE_TYPE *r)
{
  decimal128 *d128;
  /* Already in decimal128 form. */
  d128 = (decimal128 *)r->sig;

  /* Still in intermediate representation, so sign
     is seperate from encoding. */

  /* FIXME: This is WRONG place.  Should come through
     REAL_NEGATE path. */
  if (r->sign != decimal128Sign(d128)) {
    d128->bytes[0] ^= 1 << 7;  /* Flip high bit */
  }

  if (FLOAT_WORDS_BIG_ENDIAN)
    {
      buf[0] = *(long *)&d128->bytes[0];
      buf[1] = *(long *)&d128->bytes[4];
      buf[2] = *(long *)&d128->bytes[8];
      buf[3] = *(long *)&d128->bytes[12];
    }
  else
    {
      buf[0] = *(long *)&d128->bytes[12];
      buf[1] = *(long *)&d128->bytes[8];
      buf[2] = *(long *)&d128->bytes[4];
      buf[3] = *(long *)&d128->bytes[0];
    }
}

void decode_decimal128 (const struct real_format *fmt ATTRIBUTE_UNUSED,
			REAL_VALUE_TYPE *r ATTRIBUTE_UNUSED,
			const long *buf ATTRIBUTE_UNUSED)
{
  /* FIXME: Do something. */
}
