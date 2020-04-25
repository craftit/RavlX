// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_DMEDIARINGMEMORY_HEADER 
#define RAVLIMAGE_DMEDIARINGMEMORY_HEADER  1
///////////////////////////////////////////
//! rcsid=$Id$
//! file="Ravl/Contrib/DMedia/RingMemory.hh"
//! lib=RavlDMedia
//! userlevel=Default
//! docentry="Ravl.API.Images.Video.Video IO.SGI DMedia"
//! author="Charles Galambos"
//! date="22/11/1999"

#include "Ravl/DMedia/NodePath.hh"
#include "Ravl/HSet.hh"

namespace RavlImageN
{
  class VLRingDataC;
  class VLRingDataBodyC;
  
  //! userlevel=Develop
  //: RingMemory buffer node body.
  
  class VLNodeRingMemoryBodyC 
    : public VLNodeBodyC 
  {
  public:
    VLNodeRingMemoryBodyC(const VLServerC &srv,int type,UIntT noFrames);
    //: Constructor.
    // type  = VL_SRC,VL_DRN

    ~VLNodeRingMemoryBodyC();
    //: Destructor.
    
    VLRingDataC Next(); 
    //: Get next piece of data.
    
    void DoneWith(VLRingDataBodyC &dat);
    //: Tell buffer we're done with this data.
    
    void DoneWith(VLRingDataC &dat);
    //: Tell buffer we're done with this data.
    
    bool SkipFrames() const
      { return skipFrames; }
    //: Skip frames if timeing out ?

    void SetSkipFrames(bool val)
      { skipFrames = val; }
    //: Skip frames if timeing out ?
    
    VLBuffer Buffer()
      { return buffer; }
    //: Access buffer.
    
  protected:
    virtual bool Init();
    //: Initalise componentes.
    
    UIntT noFrames;
    VLBuffer buffer;
    UIntT seqCount;
    UIntT nextFree; // id for next free.
    HSetC<UIntT> toFree;
    bool skipFrames; // If true, latest frame will alway be returned from src.
    UIntT transferSize;
  };
  
  //! userlevel=Normal
  //: RingMemory buffer node.
  
  class VLNodeRingMemoryC 
    : public VLNodeC 
  {
  public:
    VLNodeRingMemoryC(const VLServerC &srv,int type,UIntT noFrames = 10)
      : VLNodeC(*new VLNodeRingMemoryBodyC(srv,type,noFrames))
      {}
    //: Constructor.
    // type  = VL_SRC,VL_DRN
    
  protected:
    VLNodeRingMemoryC(VLNodeRingMemoryBodyC &bod)
      : VLNodeC(bod)
      {}
    //: Body constructor.
    
    VLNodeRingMemoryBodyC &Body()
      { return static_cast<VLNodeRingMemoryBodyC &>(VLComponentC::Body()); }
    //: Access body.

    const VLNodeRingMemoryBodyC &Body() const
      { return static_cast<const VLNodeRingMemoryBodyC &>(VLComponentC::Body()); }
    //: Access body.
    
  public:
    
    void DoneWith(VLRingDataBodyC &dat)
      { Body().DoneWith(dat); }
    //: Tell buffer we're done with this data.
    
    void DoneWith(VLRingDataC &dat)
      { Body().DoneWith(dat); }
    //: Tell buffer we're done with this data.
    
    bool SkipFrames() const
      { return Body().SkipFrames(); }
    //: Skip frames if timeing out ?

    void SetSkipFrames(bool val)
      { Body().SetSkipFrames(val); }
    //: Skip frames if timeing out ?
    
    VLRingDataC Next();
    //: Get next piece of data.
    
    VLBuffer Buffer()
      { return Body().Buffer(); }
    //: Access buffer.
    
    friend class VLNodeRingMemoryBodyC;
  };

}

#endif
