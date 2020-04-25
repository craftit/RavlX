// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_IMGIOMPEG2DEC_HEADER
#define RAVLIMAGE_IMGIOMPEG2DEC_HEADER 1
//! rcsid="$Id$"
//! lib=RavlMPEG2
//! author="Charles Galambos"
//! docentry="Ravl.API.Images.Video.Video IO.MPEG2"
//! file="Ravl/Contrib/MPEG/mpeg2dec/ImgIOmpeg2dec.hh"

#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteYUVValue.hh"
#include "Ravl/OS/ChildOSProcess.hh"
#include "Ravl/Image/ImgIOPNMB.hh"

namespace RavlImageN {

  //! userlevel=Develop
  //: MPEG2 input base class.
  
  class ImgIOmpeg2decBaseC 
    : public DPImageIOPNMBaseC
  {
  public:
    ImgIOmpeg2decBaseC(const StringC &filename,bool transportStream = false);
    //: Constructor.
    
    ~ImgIOmpeg2decBaseC();
    //: Destructor.
    
    bool NextFrame(ImageC<ByteYUVValueC> &img);
    //: Get next frame of video.

    
  protected:
    ChildOSProcessC decodeProg;
    Array2dC<ByteT> rawBuffer;
    bool ok;
  };

  //! userlevel=Develop
  //: MPEG2 input class.
  
  template<class PixelT>
  class DPIImagempeg2decBodyC 
    : public DPIPortBodyC<ImageC<PixelT> >,
      public ImgIOmpeg2decBaseC
  {
  public:
    DPIImagempeg2decBodyC(const StringC &filename,bool transportStream = false)
      : ImgIOmpeg2decBaseC(filename,transportStream)
    {}
    //: Constructor.
    
    
    virtual bool GetAttr(const StringC &attrName,StringC &attrValue){
      if(attrName == "framerate") {
	attrValue = StringC("25");
	return true;
      }
      // Just pass request back.
      return DPPortBodyC::GetAttr(attrName,attrValue);
    }
    
    //: Get a stream attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling stream attributes such as frame rate, and compression ratios.
    
    virtual bool SetAttr(const StringC &attrName,const StringC &attrValue) {
      if(attrName == "framerate") {
	return (attrValue == StringC("25"));
      }     
      // Just pass request back.
      return DPPortBodyC::SetAttr(attrName,attrValue);
    }
									  
    //: Set a stream attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling stream attributes such as frame rate, and compression ratios.
    
    virtual ImageC<PixelT> Get() {
      ImageC<PixelT> ret;
      if(!NextFrame(ret))
	throw DataNotReadyC("No video frame available.");
      return ret;
    }
    //: Get next image.
    
    virtual bool Get(ImageC<PixelT> &buff) 
    { return NextFrame(buff); }
    //: Get next image.
    
    virtual bool IsGetReady() const
    { return ok; }
    //: Is some data ready ?
    // true = yes.
    // Defaults to !IsGetEOS().
    
    virtual bool IsGetEOS() const
    { return !ok; }
    //: Has the End Of Stream been reached ?
    // true = yes.

  };

  //! userlevel=Develop
  //: MPEG2 input class.
  
  template<class PixelT>
  class DPIImagempeg2decC 
    : public DPIPortC<ImageC<PixelT> >
  {
  public:
    DPIImagempeg2decC(const StringC &filename,bool transportStream = false)
      : DPEntityC(*new DPIImagempeg2decBodyC<PixelT>(filename,transportStream))
    {}
    //: Constructor.
    

  };
  
  
}
#endif
