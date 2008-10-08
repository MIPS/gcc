// { dg-options "-std=c++0x" }
#include <concepts>

namespace cool
{
  concept SinglePassCursor<typename Cursor> : std::CopyConstructible<Cursor>
  {
    typename key_type;

    key_type operator*(Cursor&);
    bool operator== (Cursor const&, Cursor const&);
    bool operator!= (Cursor const&, Cursor const&);
    Cursor& operator++(Cursor&);
  };
}

namespace cool
{
  concept ReadablePM<typename ReadPM, typename Key>
  {
    typename result_type;

    result_type operator()(ReadPM&, Key const&);
  };
}

namespace cool
{
  struct identity_pm
  {
    template <typename Key>
    Key& operator()(Key& key) const { return key; }
  };

  template <typename Key>
  concept_map ReadablePM<identity_pm, Key>
  {
    typedef Key& result_type;
  };
}

namespace cool
{
  auto concept EqualityComparable<typename T1, typename T2 = T1>
  {
    bool operator== (T1 const&, T2 const&);
    bool operator!= (T1 const&, T2 const&);
  };
}



namespace cool
{
  template <typename ReadPM, SinglePassCursor Cursor, typename Value>
  requires 
    cool::ReadablePM<ReadPM, SinglePassCursor<Cursor>::key_type>,
    cool::EqualityComparable<cool::ReadablePM<ReadPM, SinglePassCursor<Cursor>::key_type>::result_type, Value>
  Cursor find(ReadPM pm, Cursor begin, Cursor end, Value const& value)
  {


    while (begin != end, pm(*begin) != value)
      ++begin;
    return begin;
  }

  template <SinglePassCursor Cursor, typename Value>
  requires 
    cool::ReadablePM<cool::identity_pm, SinglePassCursor<Cursor>::key_type>,
    cool::EqualityComparable</*typename*/ cool::ReadablePM<cool::identity_pm, SinglePassCursor<Cursor>::key_type>::result_type, Value>
  Cursor find(Cursor begin, Cursor end, Value const& value)
  {
    return cool::find(cool::identity_pm(), begin, end, value);
  }

  template <SinglePassCursor Cursor, typename Value>
  requires
    cool::ReadablePM<cool::identity_pm, SinglePassCursor<Cursor>::key_type>,
    cool::EqualityComparable<cool::ReadablePM<cool::identity_pm, SinglePassCursor<Cursor>::key_type>::result_type, Value>
  Cursor find2(Cursor begin, Cursor end, Value const& value)
  {
    return cool::find(cool::identity_pm(), begin, end, value);
  }
}

