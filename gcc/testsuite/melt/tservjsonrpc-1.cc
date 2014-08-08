// file tservjsonrpc-1.cc
// in the public domain, by Basile Starynkevitch
// inspired by some example from http://www.tntnet.org/cxxtools.html
#include <unistd.h>
#include <iostream>
#include <cxxtools/arg.h>
#include <cxxtools/log.h>
#include <cxxtools/json/rpcserver.h>
#include <cxxtools/eventloop.h>

long add(long a1, long a2)
{
  long r = a1+a2;
  std::clog << "add a1=" << a1 << " a2=" << a2 << " giving r=" << r << std::endl;
  return r;
}

int main(int argc, char* argv[])
{
  try {
    // option -i <ip-address> defines the ip address of the interface, where the
    // server waits for connections. Default is empty, which tells the server to
    // listen on all local interfaces
    cxxtools::Arg<std::string> ip(argc, argv, 'i');

    // option -p <number> specifies the port, where jsonrpc requests are expected.
    // The default port is 8088 here.
    cxxtools::Arg<unsigned short> port(argc, argv, 'p', 8088);
    // create an event loop
    cxxtools::EventLoop loop;

    ////////////////////////////////////////////////////////////////////////
    // Json rpc

    // for the json rpc server we define a json server
    cxxtools::json::RpcServer jsonServer(loop, ip, port);

    // and register the functions in the server
    jsonServer.registerFunction("add", add);

    ////////////////////////////////////////////////////////////////////////
    // Run

    std::clog << __FILE__ << ":" << __LINE__ << " running pid #" << (int)getpid()
	      << " ip=" << ip << " port=" << port << std::endl;
    
    // now start the servers by running the event loop
    loop.run();


  } catch (const std::exception& e)
    {
      std::cerr << e.what() << std::endl;
    }
  std::clog << __FILE__ << " end" << std::endl;
  return 0;
}

/**** for emacs
  ++ Local Variables: ++
  ++ compile-command: "g++ -v -Wall -O -g tservjsonrpc-1.cc $(cxxtools-config  --cxxflags --libs) -lcxxtools-json -o ./tservjsonrpc-1" ++
  ++ End: ++
****/
