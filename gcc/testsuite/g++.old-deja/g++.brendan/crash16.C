// Build don't link: 
// GROUPS passed old-abort
// crash test - XFAIL *-*-*
class Graph {
public:
      unsigned         char N;
      Graph(void) {}; // ERROR - previously defined here
}

Graph::Graph(void) // ERROR -  return type.*
{    N = 10;
}

