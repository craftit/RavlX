// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! author="Charles Galambos"
//! rcsid="$Id$"
//! lib=RavlNet
//! file="Ravl/OS/Network/NetOPortServer.cc"

#include "Ravl/OS/NetOPortServer.hh"
#include "Ravl/OS/NetPortManager.hh"
#include "Ravl/OS/NetMsgOPortData.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  
  //: Constructor.
  
  NetOSPortServerBaseBodyC::NetOSPortServerBaseBodyC(const AttributeCtrlC &attrCtrl,
                                                     const DPOPortBaseC &noportBase,
                                                     const DPSeekCtrlC &nSeekCtrl,
                                                     const StringC &nPortName) 
    : NetAttributeCtrlServerBodyC(attrCtrl),
      portName(nPortName),
      oportBase(noportBase),
      seekCtrl(nSeekCtrl),
      at(0),
      sigConnectionClosed(true)
  {
    ONDEBUG(cerr << "NetOSPortServerBaseBodyC::NetOSPortServerBaseBodyC(), Called. Name=" << portName << " \n");
  }
  
  //: Destructor.
  
  NetOSPortServerBaseBodyC::~NetOSPortServerBaseBodyC() {
    oportBase.Invalidate(); // Let handle to port go.
    seekCtrl.Invalidate();  // Let handle to seek ctrl go.
    NetAttributeCtrlServerBodyC::Close();
    sigConnectionClosed(); // Signal close.
    ONDEBUG(cerr << "NetOSPortServerBaseBodyC::~NetOSPortServerBaseBodyC(), Called. Name=" << portName << " \n");
  }
  
  //: Get the port type.
  
  StringC NetOSPortServerBaseBodyC::PortType() { 
    if(oportBase.IsValid())
      return TypeName(oportBase.OutputType());
    return TypeName(typeid(void)); 
  }

  //: Called on a new incoming connection.
  // 'nep' is the NetEndPoint associated with the new connection.
  
  bool NetOSPortServerBaseBodyC::Connect(NetEndPointC &nep) {
    ONDEBUG(cerr << "NetOSPortServerBaseBodyC::Connect(), Called \n");
    RavlAssert(nep.IsValid());
    if(!NetAttributeCtrlServerBodyC::Connect(nep)) {
      ONDEBUG(cerr << "NetOSPortServerBaseBodyC::Connect(), ERROR: AttributeCtrl Connect Failed! \n");
      return false; // Already connected!
    }
    ONDEBUG(cerr << "NetOSPortServerBaseBodyC::Connect(), Local init \n");
    if(!Init()) {
      cerr << "NetOSPortServerBaseBodyC::Connect(), Failed. \n";
      return false;
    }
    ONDEBUG(cerr << "NetOSPortServerBaseBodyC::Connect(), Done. \n");
    return true;
  }
  
  //: Disconnect to an end point.
  
  bool NetOSPortServerBaseBodyC::Disconnect() {
    ONDEBUG(cerr << "NetOSPortServerBaseBodyC::Disconnect(), Called. \n");
    return NetAttributeCtrlServerBodyC::Disconnect();
  }
  
  //: Initialise stream.
  
  bool NetOSPortServerBaseBodyC::Init() {
    ONDEBUG(cerr << "NetOSPortServerBaseBodyC::Init(), Called. \n");
    ep.RegisterR(NPMsg_ReqInfo,"ReqInfo",*this,&NetOSPortServerBaseBodyC::ReqStats);
    if(oportBase.IsValid())
    {
      ep.Register(NetMsgOPortDataC(NPMsg_Data,"Data",oportBase,seekCtrl));
      ep.Register(NetMsgOPortDataC(NPMsg_DataArrayPut,"DataArray",oportBase,seekCtrl,true));
    }
    return true;
  }
  
  //: Request stream stats.
  
  bool NetOSPortServerBaseBodyC::ReqStats() {
    ONDEBUG(cerr << "NetOSPortServerBaseBodyC::ReqStats(), Called. \n");
    Int64T start = 0;
    Int64T size = ((UIntT) -1);
    if(seekCtrl.IsValid()) {
      Int64T lat = seekCtrl.Tell64();
      if (lat != streamPosUnknown)
        at = lat;
      start = seekCtrl.Start64();
      size = seekCtrl.Size64();
    }
    ep.Send(NPMsg_StreamInfo,at,start,size);
    ONDEBUG(cerr << "NetOSPortServerBaseBodyC::ReqStats(), Sent: At=" << at << " Start=" << start << " Size=" << size << "\n");
    return true;
  }

  //////////////////////////////////////////////////////////////////////////////////
  
  //! userlevel=Develop
  //: Export ISPort.
  
  bool NetExportBase(const StringC &name,NetOSPortServerBaseC &isp) {
    GlobalNetPortManager().Register(name,isp);
    return true;
  }

}
