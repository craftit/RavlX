// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2005, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlNet

#include "Ravl/OS/NetMsgOPortData.hh"
#include "Ravl/OS/NetPort.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  
  //: Constructor.
  
  NetMsgOPortDataBodyC::NetMsgOPortDataBodyC(UIntT nid,
                                             const StringC &nname,
                                             const DPOPortBaseC &portBase,
                                             const DPSeekCtrlC &nSeekCtrl,
                                             bool ndecodeArray
                                             )
    : NetMsgRegisterBodyC(nid,nname),
      at(streamPosUnknown),
      oportBase(portBase),
      seekCtrl(nSeekCtrl),
      dataType(TypeInfo(portBase.OutputType())),
      decodeArray(ndecodeArray)
  {
    RavlAssert(dataType.IsValid());
  }
  

  //: Decode and execute a packet.
  
  bool NetMsgOPortDataBodyC::Decode(NetEndPointC &ep,BinIStreamC &pkt) {
    if(!oportBase.IsValid()) {
      ONDEBUG(cerr << "NetOSPortServerBodyC<DataT>::ReqData(), Invalid output port.\n");
      ep.Send(NPMsg_ReqFailed,1); // Report end of stream.
      return true;
    }
    
    // Decode position info.
    Int64T pos;
    pkt >> pos;
    
    ONDEBUG(cerr << "NetMsgOPortDataBodyC::Decode Seeking: Pos=" << pos << " At=" << at << " SeekCtrl=" << (seekCtrl.IsValid() ? "Y" : "N") << endl);
    if (at != pos && \
        pos != static_cast<UIntT>(-1) && \
        pos != streamPosUnknown && \
        seekCtrl.IsValid())
    {
      seekCtrl.Seek64(pos);
      at = pos;
    }

    bool decodeOk = true;
    if (decodeArray)
    {
      Int64T size = 0;
      pkt >> size;
      ONDEBUG(cerr << "NetMsgOPortDataBodyC::Decode Writing: Size=" << size << endl);

      if (dataType.ReadAndPutArray(size, pkt, oportBase))
        at += size;
      else
        decodeOk = false;
    }
    else
    {
      if (dataType.ReadAndPut(pkt,oportBase))
        at++;
      else
        decodeOk = false;
    }

    if (!decodeOk)
    { // Failed to get data.
      //cerr << "NetOSPortServerBodyC<DataT>::ReqData(), Put failed. \n";
      if(!oportBase.IsPutReady())
        ep.Send(NPMsg_ReqFailed,1); // Ug
      else
        ep.Send(NPMsg_ReqFailed,2); // Just get failed.
    }
    return true;
  }

}
