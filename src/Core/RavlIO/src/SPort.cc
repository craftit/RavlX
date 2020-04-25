// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlIO
//! file="Ravl/Core/IO/SPort.cc"

#include "Ravl/DP/SPort.hh"
#include "Ravl/DP/AttributeValueTypes.hh"
#include "Ravl/StdConst.hh"

#define DPDEBUG 0
#if DPDEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  //: Seek to location in stream.
  
  bool DPSeekCtrlBodyC::Seek(UIntT) { 
    cerr << "DPSeekCtrlBodyC::Seek(UIntT),  WARNING: Not implemented. \n";
    return false; 
  }
  
  //: Delta Seek, goto location relative to the current one.
  
  bool DPSeekCtrlBodyC::DSeek(IntT off) {
    UIntT at = Tell();
    if(at == ((UIntT) (-1)))
      return false; // Tell failed.
    if(((IntT) at + off) < 0)
      return false; // Seek before beginning of file.
    return Seek((UIntT) ((IntT) at + off));
  }
  
  //: Find current location in stream.
  
  UIntT DPSeekCtrlBodyC::Tell() const { 
    cerr << "DPSeekCtrlBodyC::Tell() const, WARNING: Not implemented. \n";
    return ((UIntT)(-1)); 
  }
  
  //: Find the total size of the stream.
  // May return ((UIntT) (-1)) if not implemented.
  
  UIntT DPSeekCtrlBodyC::Size() const { 
    cerr << "DPSeekCtrlBodyC::Size() const, WARNING: Not implemented. \n";
    return ((UIntT)(-1)); 
  }
  
  UIntT DPSeekCtrlBodyC::Start() const { 
    return 0; 
  }
  
  //: Seek to location in stream.
  // Returns false, if seek failed. (Maybe because its
  // not implemented.)
  // if an error occurred (Seek returned False) then stream
  // position will not be changed.
  
  bool DPSeekCtrlBodyC::Seek64(StreamPosT off) {
    if (off == streamPosUnknown)
      return false;
    if(off < 0 || off > (Int64T) ((UIntT) -1)) {
      cerr << "WARNING: 64 bit seeks not supported.  Offset=" << off << " \n";
      return false;
    }
    return Seek((UIntT) off);
  }
  
  //: Delta Seek, goto location relative to the current one.
  // The default behaviour of this functions is :
  // Do some error checking then:
  //   Seek((UIntT)((IntT) Tell() + off));
  // if an error occurred (DSeek returned False) then stream
  // position will not be changed.
  
  bool DPSeekCtrlBodyC::DSeek64(StreamPosT off) {
    if (off == streamPosUnknown)
      return false;
    if(off < -((Int64T) ((UIntT) -1)) || off > (Int64T) ((UIntT) -1)) {
      cerr << "WARNING: 64 bit seeks not supported.  Offset=" << off << " \n";
      return false;
    }
    return DSeek(static_cast<UIntT>(off));
  }
  
  //: Find current location in stream.
  // Defined as the index of the next object to be written or read.
  // May return ((UIntT) (-1)) if not implemented.
  
  StreamPosT DPSeekCtrlBodyC::Tell64() const { 
    UIntT v = Tell();
    if(v == ((UIntT) -1))
      return streamPosUnknown;
    return v;
  }
  
  //: Find the total size of the stream. (assuming it starts from 0)
  // May return ((UIntT) (-1)) if not implemented.
  
  StreamPosT DPSeekCtrlBodyC::Size64() const 
  { 
    UIntT v = Size();
    if(v == ((UIntT) -1))
      return streamPosUnknown;
    return v;
  }
  
  //: Find the offset where the stream begins, normally zero.
  // Defaults to 0
  
  StreamPosT DPSeekCtrlBodyC::Start64() const 
  { return Start(); }
  
  //: Register sport attributes.
  // Currently size and start.
  
  bool DPSeekCtrlBodyC::RegisterSPortAttributes(AttributeCtrlBodyC &attrHandle) {
    attrHandle.RegisterAttribute(AttributeTypeNumC<IntT>("size","Size of stream. (-1 == unknown)",true,false,-1,RavlConstN::maxInt,1,-1));
    attrHandle.RegisterAttribute(AttributeTypeNumC<IntT>("start","Start of stream. ",true,false,0,RavlConstN::maxInt,1,0));
    attrHandle.RegisterAttribute(AttributeTypeNumC<IntT>("position","Position in stream. ",true,false,0,RavlConstN::maxInt,1,0));
    return true;
  }

  //: Register sport attributes.
  // Currently size and start.
  
  bool DPSeekCtrlBodyC::RegisterSPortAttributes(AttributeCtrlC &attrHandle) {
    attrHandle.RegisterAttribute(AttributeTypeNumC<IntT>("size","Size of stream. (-1 == unknown)",true,false,-1,RavlConstN::maxInt,1,-1));
    attrHandle.RegisterAttribute(AttributeTypeNumC<IntT>("start","Start of stream. ",true,false,0,RavlConstN::maxInt,1,0));
    attrHandle.RegisterAttribute(AttributeTypeNumC<IntT>("position","Position in stream. ",true,false,0,RavlConstN::maxInt,1,0));
    return true;
  }

  //////////////////////////////////////////////////////
  
  //: Default constructor.
  DPSeekCtrlStubBodyC::DPSeekCtrlStubBodyC() { 
    ONDEBUG(cerr << "DPSeekCtrlStubBodyC::DPSeekCtrlStubBodyC(), Called. \n"); 
  }

  //: Constructor.
  DPSeekCtrlStubBodyC::DPSeekCtrlStubBodyC(const DPPortC &npb)
    : pb(npb)
  { ONDEBUG(cerr << "DPSeekCtrlStubBodyC::DPSeekCtrlStubBodyC(), Called. \n"); }

  //: Seek to location in stream.
  // Returns false, if seek failed. (Maybe because its
  // not implemented.)
  // if an error occurred (Seek returned False) then stream
  // position will not be changed.
  bool DPSeekCtrlStubBodyC::Seek(UIntT) { 
    ONDEBUG(cerr << "DPSeekCtrlStubBodyC::Seek(UIntT), Called. \n");
    return false; 
  }
  
  //: Delta Seek, goto location relative to the current one.
  // The default behaviour of this functions is :
  // Do some error checking then:
  //   Seek((UIntT)((IntT) Tell() + off));
  // if an error occurred (DSeek returned False) then stream
  // position will not be changed.
  
  bool DPSeekCtrlStubBodyC::DSeek(IntT d) { 
    if(d < 0 || !pb.IsValid())
      return false;
    DPIPortBaseC ip(pb);
    if(!ip.IsValid())
      return false;
    for(;d > 0;d--)
      if(!ip.Discard())
	return false;
    return true;
  }
  
  //: Find current location in stream.
  // May return ((UIntT) (-1)) if not implemented.
  
  UIntT DPSeekCtrlStubBodyC::Tell() const 
  { return ((UIntT) -1); }

  //: Find the total size of the stream. (assume start from 0.)
  // May return ((UIntT) (-1)) if not implemented.
  
  UIntT DPSeekCtrlStubBodyC::Size() const
  { return ((UIntT) -1); }
  
  /////////////////////////////////////////////////////////////////
  
  //: Constructor.
  // If stubIt is true, a body the produces no
  // error message is used, otherwise its a noisy one.
  
  DPSeekCtrlC::DPSeekCtrlC(bool stubIt,const DPPortC &pb)
    : DPEntityC(true)
  {
    if(stubIt)
      (*this) = DPSeekCtrlC(*new DPSeekCtrlStubBodyC(pb));
    else
      (*this) = DPSeekCtrlC(*new DPSeekCtrlBodyC());
  }

  
  
}
