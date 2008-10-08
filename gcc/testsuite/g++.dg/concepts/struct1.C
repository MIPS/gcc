// From Dietmar Kuhl
// { dg-options "-std=c++0x" }
#include <concepts>

namespace cool
{
  auto concept SinglePassCursor<typename Cursor>
  {
    bool operator== (Cursor const&, Cursor const&);
    bool operator!= (Cursor const&, Cursor const&);
    Cursor& operator++(Cursor&);
  };
}

namespace cool
{
  auto concept RandomAccessCursor<typename Cursor>:
    SinglePassCursor<Cursor>
  {
    std::size_t operator- (Cursor const&, Cursor const&);
  };
}

namespace cool
{
  template <SinglePassCursor Cursor>
  int distance(Cursor begin, Cursor end)
  {
    //    std::cout << "default implementation\n";
    int count = 0;
    for (; begin != end; ++begin)
      ++count;
    return count;
  }

  template <RandomAccessCursor Cursor>
  int distance(Cursor begin, Cursor end);

#if 0
  {
    //    std::cout << "random access implementation\n";
    return end - begin;
  }
#endif

}

struct simple_single_cursor
{
  simple_single_cursor& operator++();
};

bool operator== (simple_single_cursor const&, simple_single_cursor const&);
bool operator!= (simple_single_cursor const&, simple_single_cursor const&);

int main()
{
  cool::distance(simple_single_cursor(), simple_single_cursor());
}
