// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlNet
//! file="Ravl/OS/Network/exNetPort.cc"
//! docentry="Ravl.API.OS.Network.NetPort"
//! userlevel=Normal
//! author="Charles Galambos"

//: Example of transporting a stream of integers across the network.

#include "Ravl/OS/NetIPort.hh"
#include "Ravl/OS/NetIPortServer.hh"
#include "Ravl/DP/ContainerIO.hh"
#include "Ravl/DP/Compose.hh"
#include "Ravl/OS/Date.hh"
#include "Ravl/OS/NetPortManager.hh"
#include "Ravl/Option.hh"

using namespace RavlN;

int main(int nargs,char **argv) {
  OptionC opts(nargs,argv);
  StringC address = opts.String("a","localhost:4046","Network address of server. ");
  StringC portName = opts.String("n","test","Name of port to connect to. ");
  bool client = opts.Boolean("c",false,"Create a client. ");
  bool wait = opts.Boolean("w",false,"Wait before exiting. ");
  opts.Check();
  
  // ********************** SERVER SIDE ************************************
  if(!client) {
    // Setup some data.
    DListC<IntT> lst;
    lst.InsLast(1);
    lst.InsLast(2);
    lst.InsLast(3);
    
    if(!NetPortOpen(address)) {
      std::cerr << "Failed to open netPortManager. \n";
      return __LINE__;
    }
    
    // Setup server IPort.
    DPIPortC<IntT> op = DPIContainer(lst);
    
    // Export the stream 'op' as test1
    if(!NetExport(portName,op)) {
      std::cerr << "Failed to export '" << portName << "' \n";
      return __LINE__;
    }
    std::cerr << "Server open, waiting for connections. (Use Ctrl-C to exit.)\n";
    while(1)
      Sleep(20); // Don't exit, wait for the client of connect!
  } else {
    // ********************** CLIENT SIDE ******************************
    
    // Make a connection to the server.
    NetISPortC<IntT>  isp (address,portName);
    
    // Should check it succeeded here.
    
    DListC<IntT> lst2;
    
    // Transfer data from server to lst2
    
    isp >> DPOContainer(lst2);
    
    std::cout << "Read data :" << lst2 << "\n";
    isp.Invalidate();
  }
  if(wait)
    Sleep(240);
  return 0;
}
