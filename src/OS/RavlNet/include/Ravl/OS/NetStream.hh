// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here

#include "Ravl/config.h"
#if !RAVL_COMPILER_VISUALCPP

#ifndef RAVL_NETSTREAM_HEADER
#define RAVL_NETSTREAM_HEADER 1
/////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlNet
//! author="Charles Galambos"
//! example=exSocket.cc
//! docentry="Ravl.API.OS.Network"
//! file="Ravl/OS/Network/NetStream.hh"
//! date="15/02/2000"

#include "Ravl/Stream.hh"
#include "Ravl/OS/Socket.hh"

namespace RavlN {
  ////////////////////////////
  //! userlevel=Normal
  //: Output stream to network port.
  // It uses the standard ostream/istream for writing data over a network

  class NetOStreamC 
    : public OStreamC
  {
  public:
    NetOStreamC()
      {}
    //: Default constructor.
    
    NetOStreamC(const StringC &address,bool buffered=true,bool server = false);
    //: Open net connection for output.
    //!param: address -  has the format  `host:port' where `host' may be a host name or its IP address (e.g. 122.277.96.255) and `port' is the number of the port to use.
    //!param: server - true if server; false if client
    
    NetOStreamC(const SocketC &socket,bool buffered=true);
    //: Use a socket to build new handle.
    
    SocketC &Socket() 
      { return sock; }
    //: Access socket.
    
  protected:
    SocketC sock;
  };
  
  ////////////////////////////
  //! userlevel=Normal
  //: Input stream from network port.
  // It uses the standard ostream/istream for reading data over a network
  
  class NetIStreamC 
    : public IStreamC
  {
  public:
    NetIStreamC()
      {}
    //: Default constructor
    
    NetIStreamC(const StringC &address,bool buffered=true,bool server = false);
    //: Constructor to open net connection for input
    //!param: address -  has the format  `host:port' where `host' may be a host name or its IP address (e.g. 122.277.96.255) and `port' is the number of the port to use.
    //!param: server - true if server; false if client

    NetIStreamC(const SocketC &socket,bool buffered=true);
    //: Constructor using a socket to build new handle.
    
    bool WaitForData(RealT timeout = -1);
    //: Wait for data to arrive.
    // Returns true if data is available, on a timeout or other interrupt
    // false is returned.
    
    SocketC &Socket() 
      { return sock; }
    //: Access socket.
    
  protected:
    SocketC sock;
  };
  
}


#endif
#endif

