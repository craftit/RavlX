// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLNETPACKET_HEADER
#define RAVLNETPACKET_HEADER 1
/////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/OS/Network/Packet.hh"
//! lib=RavlNet
//! author="Charles Galambos"
//! date="13/12/2000"
//! docentry="Ravl.API.OS.Network"

#include "Ravl/BinStream.hh"
#include "Ravl/BufStream.hh"

namespace RavlN
{
  class NetMessageC;
  
  //! userlevel=Advanced
  //: Packet of stream encoded data.
  
  class NetPacketC
  {
  public:
    NetPacketC()
      {}
    //: Default constructor.
    
    NetPacketC(IntT size);
    //: Construct a packet of 'size' bytes.

    NetPacketC(SArray1dC<char> &arr)
      : data(arr)
      {}
    //: Construct a packet from an array
    
    inline NetPacketC(BinIStreamC &strm);
    //: Construct a packet of 'size' bytes.
    
    SArray1dC<char> &Data()
      { return data; }
    //: Access data in packet.

    const SArray1dC<char> &Data() const
      { return data; }
    //: Access data in packet.

    IStreamC DecodeStream() const
    { return BufIStreamC(data); }
    //: Create a decoding stream.
    
    //BinOStreamC EncodeStream()
    //      { return BinOStreamC(OStrStreamC(data)); }
    //: Create an encoding stream.
    
    inline
    bool Transmit(BinOStreamC &strm) const;
    //: Transmit packet.
    
    bool IsValid() const
      { return data.Size() > 0; }
    //: Is packet valid ?

    void Dump(ostream &out) const;
    //: Dump packet in human readable form.
    
    UIntT Size() const
    { return data.Size(); }
    //: Get the size of the packet.
  protected:
    SArray1dC<char> data;
  };
  
  //: Construct a packet of 'size' bytes.
  
  inline NetPacketC::NetPacketC(BinIStreamC &strm) {
    UIntT size;
    strm >> size;
    if(!strm.Stream() || size <= 0)
      return ;
    if(size > strm.ArraySizeLimit())
      throw ExceptionOutOfRangeC("Incoming packet size exceeds limit for stream.");      
    SArray1dC<char> buf(size);
    strm.IBuff(&buf[0],size);
    data = buf;
  }

  //: Transmit packet.
  
  inline
  bool NetPacketC::Transmit(BinOStreamC &strm) const {
    strm << data.Size();
    strm.OBuff(&data[0],data.Size());
    return true;
  }
    
  
  inline
  BinOStreamC &operator<<(BinOStreamC &strm,const NetPacketC &pkt) {
    pkt.Transmit(strm);
    return strm;
  }
  
  inline
  BinIStreamC &operator>>(BinIStreamC &strm,NetPacketC &pkt) {
    strm >> pkt.Data();
    return strm;
  }
  
  
};
  
#endif
