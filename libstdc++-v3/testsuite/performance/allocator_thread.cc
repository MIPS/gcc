using __gnu_cxx::__mt_alloc;
int iterations;
  calibrate_iterations()
  {
    int try_iterations = iterations = 100000;

    __gnu_test::time_counter timer;
    timer.start();
    do_loop<Container>();
    timer.stop();

    double tics = timer.real_time();
    double iterpc = iterations / tics; //iterations per clock
    double xtics = 100; // works for linux 2gig x86
    iterations = static_cast<int>(xtics * iterpc);
  }

template<typename Container>
  void
  calibrate_iterations<vector<test_type> >();
  calibrate_iterations<list<test_type> >();
