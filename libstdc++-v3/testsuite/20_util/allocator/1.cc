// Copyright (C) 2001, 2002 Free Software Foundation, Inc.
  // XXX These should work for various size allocation and
// libstdc++/8230
void test02()
{
  bool test __attribute__((unused)) = true;
  try 
    {
      std::allocator<int> alloc;
      const std::allocator<int>::size_type n = alloc.max_size();
      int* p = alloc.allocate(n + 1);
      p[n] = 2002;
    } 
  catch(const std::bad_alloc& e) 
    {
      // Allowed.
      test = true;
    }
  catch(...) 
    {
      test = false;
    }
  VERIFY( test );
}

  test02();
