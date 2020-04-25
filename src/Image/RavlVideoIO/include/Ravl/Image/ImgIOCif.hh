// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_IMGIOCIF_HEADER
#define RAVLIMAGE_IMGIOCIF_HEADER 1
////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Image/VideoIO/ImgIOCif.hh"
//! lib=RavlVideoIO
//! example=exImgSeq.cc
//! docentry="Ravl.API.Images.Video.Video IO"
//! author="Bill Christmas, Charles Galambos"
//! date="08/04/1999"

#include "Ravl/DP/SPort.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteYUVValue.hh"
#include "Ravl/Stream.hh"
#include "Ravl/Assert.hh"

namespace RavlImageN {

  ///////////////////////////////////
  //! userlevel=Develop
  //: Basic information about a cif file
  
  class DPImageCifBaseBodyC
  {
  public:
    DPImageCifBaseBodyC(const StringC &suffix = "cif")
      : frameSize(0),
      frameNo(0),
      seqSize((UIntT) -1)
      { SetupForVariant(suffix); }
    //: Constructor.
    // This constructs with the basic cif format.
    
    DPImageCifBaseBodyC(const Index2dC &size)
      : frameSize(0),
      frameNo(0),
      seqSize((UIntT) -1)
      { SetupForVariant(size); }
    //: Constructor.
    // This constructs with the basic cif format.
    
    bool SetupForVariant(const StringC &suffix);
    //: Setup for cif variant.
    // str should be one of :
    // sqcif,qcif,cif,4cif,16cif
    
    bool SetupForVariant(const Index2dC &size);
    //: Setup for cif variant.
    // Where size in the size of the Y image.
    
    inline UIntT CalcOffset(UIntT frameNo) const  {
      RavlAssert(frameSize > 0);
      return frameSize * frameNo; 
    }
    //: Calculate the offset of a frame.
    
    inline IntT CalcOffset(IntT frameNo) const  {
      RavlAssert(frameSize > 0);
      return frameSize * frameNo; 
    }
    //: Calculate the offset of a frame.
  
    void SetSequenceSize(UIntT val) { seqSize = val; }
    //: Set the sequence size.
    
    UIntT SeqSize() const { return seqSize; }
    //: Get the sequence size.
    
  protected:
    ImageRectangleC rect; // Size of Cif variant. Origin 0,0
    UIntT frameSize; // Size of one frame in bytes.
    UIntT frameNo; // Current frameno.
    UIntT seqSize;  // Number of frames in sequence, ((UIntT) -1) if unknown
  };
  
  ///////////////////////////////////
  //! userlevel=Develop
  //: Load a YUV image in Cif format.
  
  class DPIImageCifBodyC 
    : public DPISPortBodyC<ImageC<ByteYUVValueC> >,
      public DPImageCifBaseBodyC
  {
  public:
    DPIImageCifBodyC(const IStreamC &nStrm,const StringC &suffix = "cif");
    //: Constructor from stream 
    
    DPIImageCifBodyC(const IStreamC &nStrm,const Index2dC &size);
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
    
    virtual ImageC<ByteYUVValueC> Get();
    //: Get next image.
    
    virtual bool Get(ImageC<ByteYUVValueC> &buff);
    //: Get next image.
    
    virtual bool IsGetReady() const 
      { return strm.good(); }
    //: Is some data ready ?
    // TRUE = yes.
    // Defaults to !IsGetEOS().
    
    virtual bool IsGetEOS() const
      { return strm.eof(); }
    //: Has the End Of Stream been reached ?
    // TRUE = yes.
    
  protected:
    IStreamC strm;
  };
  
  ///////////////////////////////////
  //! userlevel=Develop
  //: Save a YUV image in Cif format.
  
  class DPOImageCifBodyC 
    : public DPOSPortBodyC<ImageC<ByteYUVValueC> >,
      public DPImageCifBaseBodyC
  {
  public:
    DPOImageCifBodyC(const OStreamC &nStrm,const StringC &suffix = "cif");
    //: Constructor from stream.  
    
    DPOImageCifBodyC(const OStreamC &nStrm,const Index2dC &size);
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
    
    bool Put(const ImageC<ByteYUVValueC> &Img);
    //: Put image to a stream.
    
    virtual bool IsPutReady() const 
      { return strm.good(); }
    //: Read to write some data ?
    // TRUE = yes.
    
    virtual void PutEOS() {}
    //: Put End Of Stream marker.
    
  protected:  
    OStreamC strm;
  };
  
  //! userlevel=Normal
  //: Load a YUV image in Cif format.
  
  class DPIImageCifC 
    : public DPISPortC<ImageC<ByteYUVValueC> >
  {
  public:
    DPIImageCifC(const StringC &fn);
    //: Constructor from filename.  
    
    DPIImageCifC(const IStreamC &strm,const StringC &suffix = "cif")
      : DPEntityC(*new DPIImageCifBodyC(strm,suffix))
      {}
    //: Constructor from stream 
  
    DPIImageCifC(const IStreamC &nStrm,const Index2dC &size)
      : DPEntityC(*new DPIImageCifBodyC(nStrm,size))
      {}
    //: Constructor from stream 
    
  };
  
  //! userlevel=Normal
  //: Save a YUV image in Cif format.

  class DPOImageCifC 
    : public DPOSPortC<ImageC<ByteYUVValueC> >
  {
  public:
    DPOImageCifC(const StringC &fn);
    //: Constructor from filename.  
    
    DPOImageCifC(const OStreamC &strm,const StringC &suffix = "cif")
      : DPEntityC(*new DPOImageCifBodyC(strm,suffix))
      {}
    //: Constructor from stream.  
    
    DPOImageCifC(const OStreamC &nStrm,const Index2dC &size)
    : DPEntityC(*new DPOImageCifBodyC(nStrm,size))
      {}
    //: Constructor from stream 
  
  };
  
}



#endif
