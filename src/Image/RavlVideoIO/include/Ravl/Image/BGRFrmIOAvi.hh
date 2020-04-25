// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_BGRFRMIOAVI_HEADER
#define RAVLIMAGE_BGRFRMIOAVI_HEADER 1
////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Image/VideoIO/BGRFrmIOAvi.hh"
//! lib=RavlVideoIO
//! example=exImgSeq.cc
//! docentry="Ravl.API.Images.Video.Video IO"
//! author="Daniele Muntoni"
//! date="20/03/2003"

#include "Ravl/DP/SPort.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteBGRValue.hh"
#include "Ravl/Stream.hh"
#include "Ravl/Image/AviStream.hh"

namespace RavlImageN {

  ///////////////////////////////////
  //! userlevel=Develop
  //: Load a BGR image from an avi file.
  
  class DPISBGRAviFrameBodyC 
    : public DPISPortBodyC<ImageC<ByteBGRValueC> >
  {
  public:
    DPISBGRAviFrameBodyC(const IStreamC &nStrm);
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
    
    virtual ImageC<ByteBGRValueC> Get();
    //: Get next image.
    
    virtual bool Get(ImageC<ByteBGRValueC> &buff);
    //: Get next image.
    
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
  
  ///////////////////////////////////
  //! userlevel=Develop
  //: Save a BGR image in a Avi frame format.
  
  class DPOSBGRAviFrameBodyC 
    : public DPOSPortBodyC<ImageC<ByteBGRValueC> >
  {
  public:
    DPOSBGRAviFrameBodyC(const OStreamC &nStrm);
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
    
    bool Put(const ImageC<ByteBGRValueC> &Img);
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
  //: Load a BGR image in Avi frame format.
  
  class DPISBGRAviFrameC 
    : public DPISPortC<ImageC<ByteBGRValueC> >
  {
  public:
    DPISBGRAviFrameC(const StringC &fn);
    //: Constructor from filename.  
    
    DPISBGRAviFrameC(const IStreamC &strm)
      : DPEntityC(*new DPISBGRAviFrameBodyC(strm))
      {}
    //: Constructor from stream 
  
  };
  
  //! userlevel=Normal
  //: Save a BGR image in Avi frame format.

  class DPOSBGRAviFrameC 
    : public DPOSPortC<ImageC<ByteBGRValueC> >
  {
  public:
    DPOSBGRAviFrameC(const StringC &fn);
    //: Constructor from filename.  
    
    DPOSBGRAviFrameC(const OStreamC &nStrm)
    : DPEntityC(*new DPOSBGRAviFrameBodyC(nStrm))
      {}
    //: Constructor from stream 
  
  };
  
}



#endif
