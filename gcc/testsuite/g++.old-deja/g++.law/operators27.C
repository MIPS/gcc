void * operator new(size_t, const std::nothrow_t&) throw()         { FLAG=1; return 0; }
    K * pK = new (std::nothrow) K( 10);
