// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_DVFRMIOAVI_HEADER
#define RAVLIMAGE_DVFRMIOAVI_HEADER 1
////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Image/VideoIO/DVFrmIOAvi.hh"
//! lib=RavlVideoIO
//! example=exImgSeq.cc
//! docentry="Ravl.API.Images.Video.Video IO"
//! author="Daniele Muntoni"
//! date="20/03/2003"

#include "Ravl/DP/SPort.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/DVFrame.hh"
#include "Ravl/Stream.hh"
#include "Ravl/Image/AviStream.hh"

namespace RavlImageN {

  ///////////////////////////////////
  //! userlevel=Develop
  //: Load a DV frame from an AVI frame sequence.
  
  class DPISDVAviFrameBodyC 
    : public DPISPortBodyC<DVFrameC>
  {
  public:
    DPISDVAviFrameBodyC(const IStreamC &nStrm);
    //: Constructor from stream 
    
    virtual bool Seek(UIntT off);
    //: Seek to location in stream.
    // Returns FALSE, if seek failed. (Maybe because its
    // not implemented.)
    // if an error occurered (Seek returned False) then stream
    // position will not be changed.
    
    virtual bool DSeek(IntT off);
    //: Delta Seek, goto location relative to the current one.
    
    virtual UIntT Tell() const; 
    //: Find current location in stream.
    
    virtual UIntT Size() const; 
    //: Find the total size of the stream.
    
    virtual DVFrameC Get();
    //: Get next frame.
    
    virtual bool Get(DVFrameC &buff);
    //: Get next frame.
    
    virtual bool IsGetReady() const 
      { return aviStrm.Stream().good(); }
    //: Is some data ready ?
    // TRUE = yes.
    // Defaults to !IsGetEOS().
    
    virtual bool IsGetEOS() const
      { return aviStrm.Stream().good(); }
    //: Has the End Of Stream been reached ?
    // TRUE = yes.
    
  protected:
    AviIStreamC aviStrm;
    UIntT currentframeNo;
    BitmapInfoHST strf;
  };
  
  
  class DPOSDVAviFrameBodyC 
    : public DPOSPortBodyC<DVFrameC > 
  {
  public:
    DPOSDVAviFrameBodyC(const OStreamC &nStrm);
    //: Constructor from stream 
    
    virtual bool Seek(UIntT off);
    //: Seek to location in stream.
    // Returns FALSE, if seek failed. (Maybe because its
    // not implemented.)
    // if an error occurered (Seek returned False) then stream
    // position will not be changed.
    
    virtual bool DSeek(IntT off);
    //: Delta Seek, goto location relative to the current one.
    
    virtual UIntT Tell() const; 
    //: Find current location in stream.
    
    virtual UIntT Size() const; 
    //: Find the total size of the stream.
    
    bool Put(const DVFrameC &Frm);
    //: Put image to a stream.
    
    virtual bool IsPutReady() const 
    { return aviStrm.Stream().good(); }
    //: Read to write some data ?
    // TRUE = yes.
    
    virtual void PutEOS() {}
    //: Put End Of Stream marker.
    
  protected:  
    AviOStreamC aviStrm;
    UIntT currentframeNo;
    int framesAdded;
  };
  
  
  //! userlevel=Normal
  //: Save a DV frame in Avi frame format.
  
  class DPOSDVAviFrameC 
    : public DPOSPortC<DVFrameC >
  {
  public:
    DPOSDVAviFrameC(const StringC &fn);
    //: Constructor from filename.  
    
    DPOSDVAviFrameC(const OStreamC &nStrm)
      : DPEntityC(*new DPOSDVAviFrameBodyC(nStrm))
    {}
    //: Constructor from stream 
    
  };
  
  class DPISDVAviFrameC 
    : public DPISPortC<DVFrameC>
  {
  public:
    DPISDVAviFrameC(const StringC &fn);
    //: Constructor from filename.  
    
    DPISDVAviFrameC(const IStreamC &strm)
      : DPEntityC(*new DPISDVAviFrameBodyC(strm))
    {}
    //: Constructor from stream 
    
  };
  

}
#endif
