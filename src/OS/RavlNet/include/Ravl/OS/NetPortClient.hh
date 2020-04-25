// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_NETPORTCLIENT_HEADER
#define RAVL_NETPORTCLIENT_HEADER 1
//////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! author="Charles Galambos"
//! lib=RavlNet
//! docentry="Ravl.API.OS.Network.NetPort"
//! file="Ravl/OS/Network/NetPortClient.hh"

#include "Ravl/OS/NetEndPoint.hh"
#include "Ravl/OS/NetPortManager.hh"

namespace RavlN {
  
  class NetPortClientC;
  class NetPortManagerC;
  
  //! userlevel=Develop
  //: NetPortClient.
  // This class is a server side object that manages incoming connection
  // requests on a per client basis
  
  class NetPortClientBodyC 
    : public NetEndPointBodyC
  {
  public:
    NetPortClientBodyC(SocketC &skt,NetPortManagerC &manager);
    //: Construct from open socket.
    
    ~NetPortClientBodyC();
    //: Destructor.
    
  protected:
    bool Init();
    //: Initalise connection.
    
    bool MsgConnectTo(StringC &port,StringC &datatype,bool isIPort);
    //: Handle connect to message.
    // This will lookup the relavent NetIPortServer class
    // and complete the connection.
    
    bool MsgClose();
    //: Handle close message.
    // Close down an established connection.
    
    NetPortManagerC manager;
    
    StringC connectionName;

    bool isIPort;
    friend class NetPortClientC;
  };
  
  //! userlevel=Advanced
  //: NetPortClient.
  // This class is a server side object that manages incoming connection
  // requests on a per client basis
  
  class NetPortClientC 
    : public NetEndPointC
  {
  public:
    NetPortClientC()
    {}
    //: Default constructor.
    
    NetPortClientC(SocketC &skt,NetPortManagerC &manager)
      : NetEndPointC(*new NetPortClientBodyC(skt,manager))
    { Body().Init(); }
    //: Socket constructor.
    
  protected:
    NetPortClientC(NetPortClientBodyC &bod)
      : NetEndPointC(bod)
    {}
    //: Body constructor.
    
    NetPortClientBodyC &Body()
    { return static_cast<NetPortClientBodyC &>(NetEndPointC::Body()); }
    //: Access body.

    const NetPortClientBodyC &Body() const
    { return static_cast<const NetPortClientBodyC &>(NetEndPointC::Body()); }
    //: Access body.
    
  public:
    
    friend class NetPortClientBodyC;
  };


}

#endif
