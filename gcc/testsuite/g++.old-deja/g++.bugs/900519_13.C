  enum { base_member }; // ERROR - candidate (26, 30)
  int base_member;      // ERROR - candidate (26, 34)
  int base_member ();   // ERROR - candidate (30, 34)
