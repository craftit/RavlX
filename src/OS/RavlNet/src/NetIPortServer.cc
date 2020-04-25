// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! author="Charles Galambos"
//! lib=RavlNet
//! file="Ravl/OS/Network/NetIPortServer.cc"

#include "Ravl/OS/NetIPortServer.hh"
#include "Ravl/OS/NetPortManager.hh"
#include "Ravl/SysLog.hh"
#include "Ravl/StrStream.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {

  //: Constructor.
  
  NetISPortServerBaseBodyC::NetISPortServerBaseBodyC(const AttributeCtrlC &attrCtrl,  // Attributes
                                                     const DPIPortBaseC &nIPortBase,  // Port for IO.
                                                     const DPSeekCtrlC &nSeekCtrl,     // Seek control
                                                     const StringC &nPortName          // Name of port.
                                                     )
    : NetAttributeCtrlServerBodyC(attrCtrl),
      portName(nPortName),
      seekCtrl(nSeekCtrl),
      at(0),
      typeInfo(TypeInfo(nIPortBase.InputType())),
      iportBase(nIPortBase),
      sigConnectionClosed(true)
  {
    RavlAssert(typeInfo.IsValid());
    RavlAssert(seekCtrl.IsValid());
    at = seekCtrl.Tell64();
  }
  
  //: Destructor.
  
  NetISPortServerBaseBodyC::~NetISPortServerBaseBodyC() {
    iportBase.Invalidate(); // Let handle to port go.
    seekCtrl.Invalidate();  // Let handle to seek ctrl go.
    NetAttributeCtrlServerBodyC::Close();
    sigConnectionClosed();
    ONDEBUG(RavlDebug("NetISPortServerBaseBodyC::~NetISPortServerBaseBodyC(), Called. Name=5s ",portName.c_str()));
  }
  
  //: Get the port type.
  
  StringC NetISPortServerBaseBodyC::PortType() { 
    if(typeInfo.IsValid())
      return TypeName(typeInfo.TypeInfo());
    return TypeName(typeid(void)); 
  }
  
  //: Called on a new incoming connection.
  // 'nep' is the NetEndPoint associated with the new connection.
  
  bool NetISPortServerBaseBodyC::Connect(NetEndPointC &nep) {
    ONDEBUG(RavlDebug("NetISPortServerBaseBodyC::Connect(), Called "));
    RavlAssert(nep.IsValid());
    if(!NetAttributeCtrlServerBodyC::Connect(nep))
      return false; // Already connected!
    if(!Init()) {
      RavlError("NetISPortServerBaseBodyC::Connect(), Failed. ");
      return false;
    }
    return true;
  }
  
  //: Disonnect to an end point.
  
  bool NetISPortServerBaseBodyC::Disconnect() {
    ONDEBUG(RavlDebug("NetISPortServerBaseBodyC::Disconnect(), Called. "));
    return NetAttributeCtrlServerBodyC::Disconnect();
  }
  
  //: Initalise stream.
  
  bool NetISPortServerBaseBodyC::Init() {
    ONDEBUG(RavlDebug("NetISPortServerBaseBodyC::Init(), Called. "));
    ep.LocalInfo().ProtocolName("IPortServer");
    ep.LocalInfo().ProtocolVersion("1.1");
    ep.RegisterR(NPMsg_ReqInfo,"ReqState",*this,&NetISPortServerBaseBodyC::ReqStats);
    ep.RegisterR(NPMsg_ReqData,"ReqData",*this,&NetISPortServerBaseBodyC::ReqData);
    ep.RegisterR(NPMsg_ReqDataArray,"ReqDataArray",*this,&NetISPortServerBaseBodyC::ReqDataArray);
    return true;
  }

  //: Request information on the stream.. 
  
  bool NetISPortServerBaseBodyC::ReqData(Int64T &pos) {
    ONDEBUG(RavlDebug("NetISPortServerBaseBodyC::ReqData pos=%s at=%s",RavlN::StringOf(pos).c_str(),RavlN::StringOf(at).c_str()));
    if(!iportBase.IsValid() || !typeInfo.IsValid()) {
      ONDEBUG(cerr << "NetISPortServerBaseBodyC::ReqData port base valid=" << (iportBase.IsValid() ? "Y" : "N") << " type info valid=" << (typeInfo.IsValid() ? "Y" : "N") << endl);
      ep.Send(NPMsg_ReqFailed,1); // Report end of stream.
      return true;
    }

    if(seekCtrl.IsValid()) {
      if(at != pos && pos != (UIntT)(-1)) {
        seekCtrl.Seek64(pos);
        at = pos;
      }
    }
    
    // Compose a message to send.
    BufOStreamC os;
    BinOStreamC bos(os);
    bos.UseBigEndian(ep.UseBigEndianBinStream());
    bos.SetCompatibilityMode32Bit(ep.Use32BitMode());

    bos << NPMsg_Data << (at+1);
    if(typeInfo.GetAndWrite(iportBase,bos)) {
      at++;
      ep.Transmit(NetPacketC(os.Data()));
    } else { // Failed to get data.
      if(iportBase.IsGetEOS())
      {
        ONDEBUG(RavlDebug("NetISPortServerBaseBodyC::ReqData EOS"));
      	ep.Send(NPMsg_ReqFailed,1); // End of stream.
      }
      else
      {
        ONDEBUG(RavlDebug("NetISPortServerBaseBodyC::ReqData get failed"));
        ep.Send(NPMsg_ReqFailed,2); // Just get failed.
      }
    }
    return true;
  }



  bool NetISPortServerBaseBodyC::ReqDataArray(Int64T& pos, Int64T& size)
  {
    ONDEBUG(RavlDebug("NetISPortServerBaseBodyC::ReqDataArray() pos=%s  at=%s size=%s",
        RavlN::StringOf(pos).c_str(),RavlN::StringOf(at).c_str(),RavlN::StringOf(size).c_str()));
    if (!iportBase.IsValid() || !typeInfo.IsValid())
    {
      ONDEBUG(cerr << "NetISPortServerBaseBodyC::ReqDataArray port base valid=" << (iportBase.IsValid() ? "Y" : "N") << " type info valid=" << (typeInfo.IsValid() ? "Y" : "N") << endl);
      ep.Send(NPMsg_ReqFailed, 1); // Report end of stream.
      return true;
    }

    if (seekCtrl.IsValid())
    {
      if (at != pos && pos != (UIntT)(-1))
      {
        seekCtrl.Seek64(pos);
        at = pos;
      }
    }

    BufOStreamC os;
    BinOStreamC bos(os);
    bos.SetCompatibilityMode32Bit(ep.Use32BitMode());
    bos.UseBigEndian(ep.UseBigEndianBinStream());

    bos << NPMsg_DataArrayGet;
    Int64T dataRead = typeInfo.GetAndWriteArray(iportBase, size, bos);
    if (dataRead > 0)
    {
      at += dataRead;
      bos << static_cast<Int64T>(at);
      ONDEBUG(cerr << "NetISPortServerBaseBodyC::ReqDataArray() read=" << dataRead << " pos=" << at << endl);
      ep.Transmit(NetPacketC(os.Data()));
    }
    else
    { // Failed to get data.
      if (iportBase.IsGetEOS())
      {
        ONDEBUG(RavlDebug("NetISPortServerBaseBodyC::ReqDataArray EOS"));
        ep.Send(NPMsg_ReqFailed, 1); // End of stream.
      }
      else
      {
        ONDEBUG(RavlDebug("NetISPortServerBaseBodyC::ReqDataArray get failed"));
        ep.Send(NPMsg_ReqFailed, 2); // Just get failed.
      }
    }
    
    return true;
  }
  
  //: Request stream stats.
  
  bool NetISPortServerBaseBodyC::ReqStats() {
    ONDEBUG(RavlDebug("NetISPortServerBaseBodyC::ReqStats(), Called. "));
    Int64T lat = 0;
    Int64T start = 0;
    Int64T size = ((UIntT) -1);
    if(seekCtrl.IsValid()) {
      lat = seekCtrl.Tell64();
      start = seekCtrl.Start64();
      size = seekCtrl.Size64();
      at = lat;
    }
    ep.Send(NPMsg_StreamInfo,lat,start,size);
    ONDEBUG(RavlDebug("NetISPortServerBaseBodyC::ReqStats(), Sent: At=%s Start=%s Size=%s ",
        RavlN::StringOf(at).c_str(),RavlN::StringOf(start).c_str(),RavlN::StringOf(size).c_str()));
    return true;
  }

  //////////////////////////////////////////////////////////////////////////////////
  
  //! userlevel=Develop
  //: Export ISPort.
  
  bool NetExportBase(const StringC &name,NetISPortServerBaseC &isp) {
    GlobalNetPortManager().Register(name,isp);    
    return true;
  }

}
