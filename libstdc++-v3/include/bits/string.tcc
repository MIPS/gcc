    _CharT 
    typename basic_string<_CharT, _Traits, _Alloc>::size_type 
    const basic_string<_CharT, _Traits, _Alloc>::size_type
    basic_string<_CharT, _Traits, _Alloc>::size_type
  // NB: This is specialized for the standard char_traits<char>
  // specialization to use the same optimizations as strchr.

  // Specialization for char, definitions in src/string-inst.cc.
  template<>
    const char* 
    string::_S_find(const char* __beg, const char* __end, char __c);

  // Specialization for wchar_t.
#ifdef _GLIBCPP_USE_WCHAR_T
  template<>
    const wchar_t* 
    wstring::_S_find(const wchar_t* __beg, const wchar_t* __end, wchar_t __c);
#endif
