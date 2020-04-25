// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_NETIPORTSERVER_HEADER
#define RAVL_NETIPORTSERVER_HEADER 1
/////////////////////////////////////////////////////////////////
//! author="Charles Galambos"
//! lib=RavlNet
//! docentry="Ravl.API.OS.Network.NetPort"
//! example=exNetPort.cc
//! file="Ravl/OS/Network/NetIPortServer.hh"

#include "Ravl/OS/NetPort.hh"
#include "Ravl/OS/NetEndPoint.hh"
#include "Ravl/DP/SPort.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/DP/SPortAttach.hh"
#include "Ravl/OS/NetAttributeCtrlServer.hh"
#include "Ravl/DP/TypeInfo.hh"
#include "Ravl/Threads/Signal.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Base class for NetIPorts.
  
  class NetISPortServerBaseBodyC 
    : public NetAttributeCtrlServerBodyC
  {
  public:
    NetISPortServerBaseBodyC(const AttributeCtrlC &attrCtrl,  // Attributes
                             const DPIPortBaseC &nIPortBase,  // Port for IO.
                             const DPSeekCtrlC &seekCtrl,     // Seek control
                             const StringC &portName          // Name of port.
                             );
    //: Constructor.
    
    ~NetISPortServerBaseBodyC();
    //: Destructor.
    
    const StringC &Name() const
    { return portName; }
    //: Access port name.
    
    void SetName(const StringC &newName)
    { portName = newName; }
    //: Set a new port name.
    
    virtual StringC PortType();
    //: Get the port type.
    
    bool Connect(NetEndPointC &ep);
    //: Connect to an end point.
    // Returns false if port is already in use..
    
    virtual bool ReqData(Int64T &pos);
    //: Request information on the stream..

    virtual bool ReqDataArray(Int64T& pos, Int64T& size);
    //: Request information on the stream..

    bool Disconnect();
    //: Disconnect end point.
    
    NetEndPointC &NetEndPoint()
    { return ep; }
    //: Access net end point.
    
    Signal0C &SigConnectionClosed()
    { return sigConnectionClosed; }
    //: Access connection closed signal.

  protected:
    virtual bool Init();
    //: Initalise stream.
    
    bool ReqStats();
    //: Request information on the stream.. 
    
    StringC portName;
    DPSeekCtrlC seekCtrl;
    //Int64T at;
    UIntT at;
    
    DPTypeInfoC typeInfo;
    DPIPortBaseC iportBase;
    Signal0C sigConnectionClosed;
  };
  
  //! userlevel=Develop
  //: Input port body
  
  template<class DataT>
  class NetISPortServerBodyC
    : public NetISPortServerBaseBodyC
  {
  public:
    NetISPortServerBodyC(const DPISPortC<DataT> &niport,const StringC &portName)
      : NetISPortServerBaseBodyC(niport,niport,niport,portName),
	iport(niport)
    {}
    //: Constructor.
    
    ~NetISPortServerBodyC() 
    {}
    //: Destructor
    
    virtual StringC PortType()
    { return TypeName(typeid(DataT)); }
    //: Get the port type.
    
  protected:    
    DPISPortC<DataT> iport;
  };
  
  //! userlevel=Advanced
  //: Input port server base.

  class NetISPortServerBaseC
    : public NetAttributeCtrlServerC
  {
  public:
    NetISPortServerBaseC()
    {}
    //: Default constructor.
    // Creates an invalid handle.

    NetISPortServerBaseC(const AttributeCtrlC &attrCtrl,  // Attributes
                         const DPIPortBaseC &nIPortBase,  // Port for IO.
                         const DPSeekCtrlC &seekCtrl,     // Seek control
                         const StringC &portName          // Name of port.
                         )
      : NetAttributeCtrlServerC(*new NetISPortServerBaseBodyC(attrCtrl,nIPortBase,seekCtrl,portName))
    {}
    //: Construct a non templated server.
    
  protected:
    NetISPortServerBaseC(NetISPortServerBaseBodyC &bod)
      : NetAttributeCtrlServerC(bod)
    {}
    //: Body constructor.
    
    NetISPortServerBaseBodyC &Body()
    { return static_cast<NetISPortServerBaseBodyC &>(NetAttributeCtrlServerC::Body()); }
    //: Access body.
    
    const NetISPortServerBaseBodyC &Body() const
    { return static_cast<const NetISPortServerBaseBodyC &>(NetAttributeCtrlServerC::Body()); }
    //: Access body.
    
  public:
    const StringC &Name() const
    { return Body().Name(); }
    //: Access port name.

    void SetName(const StringC &newName)
    { Body().SetName(newName); }
    //: Set a new port name.
    
    bool Connect(NetEndPointC &ep)
    { return Body().Connect(ep); }
    //: Connect to an end point.
    // Returns false if port is already in use..
    
    bool Disconnect()
    { return Body().Disconnect(); }
    //: Disconnect end point.
    
    StringC PortType()
    { return Body().PortType(); }
    //: Get the port type.
    
    NetEndPointC &NetEndPoint()
    { return Body().NetEndPoint(); }
    //: Access net end point.
    
    Signal0C &SigConnectionClosed()
    { return Body().SigConnectionClosed(); }
    //: Access connection closed signal.

  };
  
  //! userlevel=Advanced
  //: Input port.
  // This class manages the information needed to serve a particular port.
  
  template<class DataT>
  class NetISPortServerC
    : public NetISPortServerBaseC
  {
  public:
    NetISPortServerC()
    {}
    //: Default constructor.
    // creates an invalid handle.
    
    NetISPortServerC(const DPISPortC<DataT> &iport,const StringC &portName)
      : NetISPortServerBaseC(*new NetISPortServerBodyC<DataT>(iport,portName))
    {}
    //: Constructor.
    
  protected:
    NetISPortServerC(NetISPortServerBodyC<DataT> &bod)
      : NetISPortServerBaseC(bod)
    {}
    //: Body constructor.

    NetISPortServerBodyC<DataT> &Body()
    { return static_cast<NetISPortServerBodyC<DataT> &>(NetISPortServerBaseC::Body()); }
    //: Access body.
    
    const NetISPortServerBodyC<DataT> &Body() const
    { return static_cast<const NetISPortServerBodyC<DataT> &>(NetISPortServerBaseC::Body()); }
    //: Access body.
    
    bool ReqData(Int64T &pos)
    { return Body().ReqData(pos); }
    //: Request information on the stream..

    bool ReqDataArray(Int64T& pos, Int64T& size)
    { return Body().ReqDataArray(pos, size); }
    //: Request information on the stream..

    friend class NetISPortServerBodyC<DataT>;
  };

  ///////////////////////////////////////////////////

  bool NetExportBase(const StringC &name,NetISPortServerBaseC &isp);
  //! userlevel=Develop
  //: Export  port.
  
  template<class DataT>
  bool NetExport(const StringC &portName,DPISPortC<DataT> &port) {
    NetISPortServerC<DataT> ips(port,portName);
    return NetExportBase(portName,ips);
  }
  //! userlevel=Normal 
  //: Export an SPort with a given portName.
  
  template<class DataT>
  bool NetExport(const StringC &portName,DPIPortC<DataT> &port) {
    NetISPortServerC<DataT> ips(SPort(port),portName);
    return NetExportBase(portName,ips);
  }
  //! userlevel=Normal 
  //: Export an SPort with a given portName.
  
}

#endif
