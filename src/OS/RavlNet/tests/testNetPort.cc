// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlNet
//! file="Ravl/OS/Network/testNetPort.cc"
//! docentry="Ravl.API.OS.Network.NetPort"
//! userlevel=Normal
//! author="Charles Galambos"

#include "Ravl/OS/NetIPort.hh"
#include "Ravl/OS/NetIPortServer.hh"
#include "Ravl/OS/NetOPort.hh"
#include "Ravl/OS/NetOPortServer.hh"
#include "Ravl/DP/ContainerIO.hh"
#include "Ravl/DP/Compose.hh"
#include "Ravl/OS/Date.hh"
#include "Ravl/OS/NetPortManager.hh"
#include "Ravl/Option.hh"
#include "Ravl/DList.hh"

using namespace RavlN;

int testNetIPort(const char *name);
int testNetOPort();

StringC g_server;

int main(int nargs,char **argv) 
{
  OptionC opts(nargs,argv);
  g_server = opts.String("h","localhost:4048","Network address to use. ");
  bool iport = opts.Boolean("i",true,"Test NetIPort.");
  bool iport2 = opts.Boolean("i2",false,"Test NetIPort again.");
  bool oport = opts.Boolean("o",true,"Test NetOPort.");
  opts.Check();
  int ln;
  
  cerr << "testNetPort(), Start port server. \n";
  if(!NetPortOpen(g_server)) {
    cerr << "Failed to open netPortManager. \n";
    return __LINE__;
  }
  
  if(iport)  {
    if((ln = testNetIPort("ia")) != 0) {
      cerr << "Test failed on line :" << ln << "\n";
      return 1;
    }
    // Need to give read threads time to shutdown.
  }
  if(iport2) {
    if((ln = testNetIPort("ib")) != 0) {
      cerr << "Test failed on line :" << ln << "\n";
      return 1;
    }
    // Need to give read threads time to shutdown.
  }
  if(oport) {
    if((ln = testNetOPort()) != 0) {
      cerr << "Test failed on line :" << ln << "\n";
      return 1;
    }
    // Need to give read threads time to shutdown.
  }
  cout << "Waiting for cleanup. \n";
  NetPortClose();
  RavlN::Sleep(RealT(6));
  cout << "Test passed ok. \n";
  return 0;
}

int testNetIPort(const char *name) 
{
  cerr << "testNetPort(), Test started. \n";

  // ********************** SERVER SIDE ************************************
  // Setup some data.
  DListC<IntT> lst;
  
  lst.InsLast(1);
  lst.InsLast(2);
  lst.InsLast(3);
  
  // Setup server IPort.
  cerr << "testNetPort(), Setup server IPort. \n";
  DPIPortC<IntT> op = DPIContainer(lst);

  // Export the stream 'op' as test1
  if(!NetExport(name,op)) {
    cerr << "Failed to export 'test1' \n";
    return __LINE__;
  }
  
  // ********************** CLIENT SIDE ******************************
  
  cerr << "testNetPort(), Setup  NetIPort. \n";
  
  // Make a connection to the server.
  NetISPortC<IntT>  isp (g_server,name);
  
  // Should check it succeeded here.
  DListC<IntT> list2;  
  
  // Transfer data from server to lst2
  
  cerr << "testNetPort(), Transfer data. \n";
  
  isp >> DPOContainer(list2);
  
  // ********************** CHECK IT WORKED ************************
  
  cerr << "testNetPort(), Check data.\n";
  cerr << "List1=" << lst << endl;
  cerr << "List2=" << list2 << endl;
  // Check the results.
  if(list2.Size() != lst.Size()) return __LINE__;
  if(list2.First() != lst.First()) return __LINE__;
  if(list2.Last() != lst.Last()) return __LINE__;
  return 0;
}




int testNetOPort() 
{
  cerr << "testNetPort(), Test started. \n";

  // ********************** SERVER SIDE ************************************
  // Setup some data.
  DListC<IntT> lst;
  
  // Setup server IPort.
  cerr << "testNetPort(), Setup server OPort. \n";
  DPOPortC<IntT> op = DPOContainer(lst);
  
  // Export the stream 'op' as test2
  if(!NetExport("test2",op)) {
    cerr << "Failed to export 'test2' \n";
    return __LINE__;
  }
  
  // ********************** CLIENT SIDE ******************************
  
  cerr << "testNetPort(), Setup  NetOPort. \n";

  // Make a connection to the server.
  NetOSPortC<IntT>  osp (g_server,"test2");
  
  // Should check it succeeded here.
  
  RavlN::DListC<int> list2;
   
  list2.InsLast(1);
  list2.InsLast(2);
  list2.InsLast(3);
  
  // Transfer data from server to lst2
  
  cerr << "testNetPort(), Transfer data. \n";
  
  DPIContainer(list2) >> osp;
  RavlN::Sleep(1); // Wait for data to be sent.
  
  // ********************** CHECK IT WORKED ************************
  
  cerr << "testNetPort(), Check data.\n";
  cerr << "List1=" << lst << endl;
  cerr << "List2=" << list2 << endl;
  // Check the results.
  if(list2.Size() != lst.Size())  return __LINE__;
  if(list2.First() != lst.First())  return __LINE__;
  if(list2.Last() != lst.Last())  return __LINE__;
  return 0;
}
