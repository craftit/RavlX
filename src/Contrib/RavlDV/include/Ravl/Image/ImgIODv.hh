// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_IMGIODV_HEADER
#define RAVLIMAGE_IMGIODV_HEADER 1
//////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Contrib/DV/ImgIODv.hh"
//! lib=RavlDV
//! author="Kieron Messer"
//! docentry="Ravl.API.Images.Video.Video IO.DV"
//! date="15/05/2002"

#include "Ravl/DP/SPort.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/Stream.hh"
#include "Ravl/Image/DvDecode.hh"

namespace RavlImageN {

  using namespace RavlN;

  ///////////////////////////////////
  //! userlevel=Develop
  //: Basic information about a cif file
  
  class DPImageDvBaseBodyC
  {
  public:
    DPImageDvBaseBodyC(const StringC &suffix = "dv")
      : rect(0, 575, 0, 719),
	frameSize(144000),
	frameNo(0),
	seqSize((UIntT) -1)
    {  }
    //: Constructor.
    // This constructs with the basic cif format.
    
    inline streampos CalcOffset(UIntT frameNo) const {
      RavlAssert(frameSize > 0);
      return ((streampos) frameSize) * ((streampos) frameNo); 
    }
    //: Calculate the offset of a frame.
    
    inline streampos CalcOffset(streampos frameNo) const {
      RavlAssert(frameSize > 0);
      return ((streampos) frameSize) * frameNo; 
    }
    //: Calculate the offset of a frame.
    
    void SetSequenceSize(UIntT val) { seqSize = val; }
    //: Set the sequence size.
    
    UIntT SeqSize() const { return seqSize; }
    //: Get the sequence size in frames.
    
  protected:
    ImageRectangleC rect; // Size of Dv variant. Origin 0,0
    UIntT frameSize; // Size of one frame in bytes.
    UIntT frameNo; // Current frameno.
    UIntT seqSize;  // Number of frames in sequence, ((UIntT) -1) if unknown
  };
  
  ///////////////////////////////////
  //! userlevel=Develop
  //: Load a YUV image in Dv format.
  
  class DPIImageDvBodyC 
    : public DPISPortBodyC<AVFrameC >,
      public DPImageDvBaseBodyC
  {
  public:
    DPIImageDvBodyC(const IStreamC &nStrm,const StringC &suffix = "dv");
    //: Constructor from stream 
    
    virtual bool Seek(UIntT off);
    //: Seek to location in stream.
    // Returns false, if seek failed. (Maybe because its
    // not implemented.)
    // if an error occurered (Seek returned False) then stream
    // position will not be changed.
    
    virtual bool DSeek(IntT off);
    //: Delta Seek, goto location relative to the current one.
    
    virtual UIntT Tell() const; 
    //: Find current location in stream.
    
    virtual UIntT Size() const; 
    //: Find the total size of the stream (in frames)
    
    virtual AVFrameC Get();
    //: Get next image.
    
    virtual bool Get(AVFrameC &buff);
    //: Get next image.
    
    virtual bool IsGetReady() const 
    { return strm.good(); }
    //: Is some data ready ?
    // true = yes.
    // Defaults to !IsGetEOS().
    
    virtual bool IsGetEOS() const
    { return strm.good(); }
    //: Has the End Of Stream been reached ?
    // true = yes.
    
    virtual bool BuildAttributes ( AttributeCtrlBodyC & attributes ) ; 
    //: Builds the list of attributes 

  virtual bool GetAttr(const StringC &attrName,StringC &attrValue);
    //: Get a stream attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling stream attributes such as frame rate, and compression ratios.
  
  virtual bool GetAttr(const StringC & attrName, IntT & attrValue ) ; 
  //: Get a stream attribute.
  // Returns false if the attribute name is unknown.
  // This is for handling stream attributes such as frame rate, and compression ratios.

  virtual bool GetAttr(const StringC & attrName, RealT & attrValue ) ; 
    //: Get a stream attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling stream attributes such as frame rate, and compression ratios.

    virtual bool SetAttr(const StringC &attrName,const StringC &attrValue);
    //: Set a stream attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling stream attributes such as frame rate, and compression ratios.

    
    // TimeCodeC getTimeCode() const
    //  { return tcGrab; }
    //: return current grabbed timecode
    
  protected:
    IStreamC strm;
    //PalFrameC frame;
    DvDecodeC dv;
    //TimeCodeC tcGrab;
    //: The DV converter
  };

  
  
  class DPIImageDvC 
    : public DPISPortC<AVFrameC >
  {
  public:
    DPIImageDvC(const StringC &fn);
    //: Constructor from filename.  
    
    DPIImageDvC(const IStreamC &strm,const StringC &suffix = "dv")
      : DPEntityC(*new DPIImageDvBodyC(strm,suffix))
    {}
    //: Constructor from stream 
    
  protected:
    
    inline
    DPIImageDvBodyC & Body() 
    { return dynamic_cast<DPIImageDvBodyC &>(DPEntityC::Body()); }
    //: Access body.
    // This isn't really needed, they're just to ensure
    // all derived classes work properly.
    
    inline
    const DPIImageDvBodyC &Body() const
    { return dynamic_cast<const DPIImageDvBodyC &>(DPEntityC::Body()); }
    //: Constant access body.
    // This isn't really needed, they're just to ensure
    // all derived classes work properly.
    
  public:
    // TimeCodeC getTimeCode() const
    //  { return Body().getTimeCode(); }
    //: return current grabbed timecode
    
    
  };
  
} // end namespace RavlImageN

#endif
