// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_DMEDIAVLDATA_HEADER
#define RAVLIMAGE_DMEDIAVLDATA_HEADER 1
//////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Contrib/DMedia/RingData.hh"
//! lib=RavlDMedia
//! userlevel=Default
//! docentry="Ravl.API.Images.Video.Video IO.SGI DMedia"
//! author="Charles Galambos"
//! date="22/11/1999"

#include "Ravl/DMedia/NodePath.hh"
#include "Ravl/DMedia/RingMemory.hh"
#include "Ravl/Image/Image.hh"

namespace RavlImageN
{
  
  class VLRingDataBodyC 
    : public RCBodyVC
  {
  public:
    VLRingDataBodyC(const VLNodeRingMemoryC &nbuff,VLInfoPtr ptr,UIntT nsize,UIntT nseqNo)
      : buff(nbuff),
	data(ptr),
	seqNo(nseqNo),
	size(nsize),
	imgInfo(0)
      {}
    //: Constructor.
    
    ~VLRingDataBodyC();
    //: Destructor.
    
    VLInfoPtr InfoPtr() const
      { return data; }
    //: Get info ptr.
    
    const VLNodeRingMemoryC &Node()
      { return buff; }
    //: Access node.

    UIntT SeqNo() const
      { return seqNo; }
    //: Access sequence no.
    
    void Done();
    //: Done with data.
    // This will also invalidate the buffer handle.
    
    bool CopyFrom(VLRingDataC &oth);
    //: Copy data from another frame.
    
    void *RawData()
      { return vlGetActiveRegion(buff.Server(),buff.Buffer(),data); }
    //: Access raw data.

    UIntT BufferSize() const
      { return size; }
    //: Size of buffer.
    
  protected:
    VLNodeRingMemoryC buff;
    VLInfoPtr data;
    UIntT seqNo;
    UIntT size;
    DMImageInfo *imgInfo;
  };
  
  class VLRingDataC 
    : public RCHandleC<VLRingDataBodyC>
  {
  public:
    VLRingDataC()
      {}
    //: Default Constructor.
    // Creates an invalid handle.
    
    VLRingDataC(const VLNodeRingMemoryC &nbuff,VLInfoPtr ptr,UIntT nsize,UIntT nseqNo)
      : RCHandleC<VLRingDataBodyC>(*new VLRingDataBodyC(nbuff,ptr,nsize,nseqNo))
      {}
    //: Constructor.
    
    VLInfoPtr InfoPtr() const
      { return Body().InfoPtr(); }
    //: Get info ptr.

    const VLNodeRingMemoryC &Node()
      { return Body().Node(); }
    //: Access node.
    
    UIntT SeqNo() const
      { return Body().SeqNo(); }
    //: Access sequence no.
    
    void Done()
      { Body().Done(); }
    //: Done with data.
    
    bool CopyFrom(VLRingDataC &oth)
      { return Body().CopyFrom(oth); }
    //: Copy data from another frame.
    
    void *RawData()
      { return Body().RawData(); }
    //: Access raw data.
    
    UIntT BufferSize() const
      { return Body().BufferSize(); }
    //: Size of buffer.
    
    friend class VLNodeRingMemoryBodyC;
  };
  
}

#endif
