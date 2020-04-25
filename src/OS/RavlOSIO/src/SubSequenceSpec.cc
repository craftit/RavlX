// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlOSIO
//! file="Ravl/OS/IO/SubSequenceSpec.cc"

#include "Ravl/DP/SubSequenceSpec.hh"
#include "Ravl/BinStream.hh"

namespace RavlN {
  
  SubSequenceSpecBodyC::SubSequenceSpecBodyC()
   : channelId(0),
     single(false)
  {}

  //: Constructor.
  
  SubSequenceSpecBodyC::SubSequenceSpecBodyC(const StringC &nfn,
                                             IndexC nfirstFrame,
                                             IndexC nlastFrame,
                                             IntT nchannelId,
                                             const StringC &fmt,
                                             bool nsingle)
    : fn(nfn),
      fileFormat(fmt),
      range(nfirstFrame,nlastFrame),
      channelId(nchannelId),
      single(nsingle)
  {}

  //: Copy from another spec
  
  SubSequenceSpecBodyC::SubSequenceSpecBodyC(const SubSequenceSpecC &oth)
  {
    if(!oth.IsValid())
      return ;
    fn = oth.Filename();
    range = oth.Range();
    channelId = oth.ChannelId();
  }

  //: BinStream constructor.
  
  SubSequenceSpecBodyC::SubSequenceSpecBodyC(BinIStreamC &is)  
  { is >> fn >> fileFormat >> range.Min() >> range.Max() >> channelId; }
  
  //: Stream constructor.
  
  SubSequenceSpecBodyC::SubSequenceSpecBodyC(istream &is)
  { is >> fn >> fileFormat >> range.Min() >> range.Max() >> channelId; }
  
  //: Save to a stream.
  
  bool SubSequenceSpecBodyC::Save(ostream &os) const {
    os << fn << ' ' << fileFormat << ' ' << range.Min() << ' ' << range.Max() << ' ' << channelId;
    return true;
  }
  
  //: Save to a BinStream.
  
  bool SubSequenceSpecBodyC::Save(BinOStreamC &os) const {
    os << ((const StringC &)fn) << fileFormat << ' ' << range.Min() << range.Max() << channelId;
    return true;
  }
  
  //: Output to ostream.
  
  BinOStreamC &operator<<(BinOStreamC &strm,const SubSequenceSpecC &str) {
    str.Save(strm);
    return strm;
  }
  
  //: Input from istream.
  BinIStreamC &operator>>(BinIStreamC &strm,SubSequenceSpecC &str) {
    str = SubSequenceSpecC(strm);
    return strm;    
  }

  //: Set to single frame.
  void SubSequenceSpecBodyC::SetSingle(bool nsing) {
    single = nsing;
    if(single) 
      range.Max() = range.Min(); // Make the length 1.
  }

}

