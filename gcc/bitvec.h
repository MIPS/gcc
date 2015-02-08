

#ifndef BITVEC_H_
#define BITVEC_H_

#include "vec.h"

class stack_bitvec;

class bitvec
{
private:
  typedef uint64_t elt_type;
  static const uint8_t bits_per_elt = 64;

public:

  bitvec () {}
  explicit bitvec (size_t elts)
    { m_bits.safe_grow_cleared (needed_elts (elts)); }

  struct bit_ref
  {
    bit_ref (uint64_t *word, uint8_t bit) : m_word (word), m_bit (bit) {}

    operator bool() const { return *m_word & (((uint64_t) 1) << m_bit); }
    bit_ref &operator= (bool b)
      {
	if (b)
	  *m_word |= ((uint64_t) 1) << m_bit;
	else
	  *m_word &= ~(((uint64_t) 1) << m_bit);

	return *this;
      }

private:
    uint64_t *m_word;
    uint8_t m_bit;
  };

  bool
  operator[] (size_t elt) const
    { return m_bits[word (elt)] & bit_mask (elt); }
  bit_ref operator[] (size_t elt)
    { return bit_ref (&m_bits[word (elt)], elt % bits_per_elt); }

  bitvec &
  operator|= (const bitvec &other)
  {
    if (other.m_bits.length () > m_bits.length ())
      m_bits.safe_grow_cleared (other.m_bits.length ());

    for (size_t i = 0; i < other.m_bits.length (); i++)
      m_bits[i] |= other.m_bits[i];

    return *this;
  }

  bool
  operator== (const bitvec &other) const
    {
      if (m_bits.length () != other.m_bits.length ())
	return false;

      for (size_t i = 0; i < m_bits.length (); i++)
	if (m_bits[i] != other.m_bits[i])
	  return false;

      return true;
    }

  size_t begin () const { return 0; }
  size_t end () const { return m_bits.length () * bits_per_elt; }

  size_t size () const { return m_bits.length (); }

  ssize_t first_set_bit () const;
  bool any_set_bits () const { return first_set_bit () >= 0; }

  bitvec (const bitvec &other) : m_bits (other.m_bits) {}

  void clear_and_release () { m_bits.truncate (0); }

  void clear ()
    {
      memset (m_bits.address (), 0, sizeof (uint64_t) * m_bits.length ());
    }

  void set ()
    {
      for (size_t i = 0; i < m_bits.length (); i++)
	m_bits[i] = ~(uint64_t) 0;
    }

  void
  grow (size_t bits)
  {
    m_bits.safe_grow_cleared (needed_elts (bits));
  }

  void dump (FILE *) const;

protected:
  static size_t word (size_t elt) { return elt / bits_per_elt; }
  static size_t bit_mask (size_t elt) { return ((uint64_t) 1) << (elt % bits_per_elt); }
  static size_t
  needed_elts (size_t bits)
  {
    return (bits + bits_per_elt - 1) / bits_per_elt;
  }

  void copy (const bitvec &other) { m_bits = other.m_bits; }

  friend stack_bitvec operator| (const bitvec &, const bitvec &);

  auto_vec<uint64_t, 0> m_bits;
};

class stack_bitvec : public bitvec
{
public:
  stack_bitvec () { init (); }

  explicit stack_bitvec (size_t bits)
  {
    if (needed_elts (bits) <= inline_elts)
      {
	m_auto.embedded_init (inline_elts, 0, 1);
      m_bits.m_vec = &m_auto;
    }

    m_bits.safe_grow_cleared (needed_elts (bits));
      }

  stack_bitvec (const stack_bitvec &other) : bitvec ()
  {
    init ();
      copy (other);
  }

  stack_bitvec (const bitvec &other)
    {
      init ();
      copy (other);
    }

  stack_bitvec &
    operator= (const bitvec &other)
      {
	clear_and_release ();
	init ();
	copy (other);
	return *this;
      }

  stack_bitvec &
    operator= (const stack_bitvec &other)
      {
	clear_and_release ();
	init ();
	copy (other);
	return *this;
      }

private:
  static const uint8_t inline_elts = 4;

  void
  init ()
    {
      m_auto.embedded_init (inline_elts, 0, 1);
      m_bits.m_vec = &m_auto;
    }

  vec<uint64_t, va_heap, vl_embed> m_auto;
  uint64_t m_data[inline_elts - 1];
};

inline stack_bitvec
operator| (const bitvec &a, const bitvec &b)
{
  stack_bitvec both (a);
	size_t len2 = b.m_bits.length ();
	if (len2 > both.m_bits.length ())
	  both.m_bits.safe_grow_cleared (len2);

	for (size_t i = 0; i < len2; i++)
	  both.m_bits[i] |= b.m_bits[i];

	return both;
      }

#endif
