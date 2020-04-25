// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlNet
//! file="Ravl/OS/Network/SocketStrm.cc"

#include "Ravl/config.h"

#include "Ravl/StreamType.hh"
#include "Ravl/OS/NetStream.hh"
#include "Ravl/OS/Filename.hh"

namespace RavlN {
  void StreamType_NetStream_MustLink()
  {}
  
#if !RAVL_COMPILER_VISUALCPP
  //: Networked streams.
  
  static class StreamType_NetStreamC 
    : public StreamTypeC
  {
  public:
    StreamType_NetStreamC()
    {}
    //: Default constructor.
    
    virtual const char *TypeName()
    { return "net"; }
    //: Get type of stream.
    
    virtual IStreamC OpenI(const StringC &strm,bool binary,bool buffered)
    { return NetIStreamC(strm,binary,buffered); }
    //: Open input stream.
    
    virtual OStreamC OpenO(const StringC &strm,bool binary,bool buffered)
    { return NetOStreamC(strm,binary,buffered); }
    //: Open ouput stream.
    
  } Inst_StreamType_NetStream;

#endif
}
