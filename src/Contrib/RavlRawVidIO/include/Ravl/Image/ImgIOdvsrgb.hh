// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_IMGIODVSRGB_HEADER
#define RAVLIMAGE_IMGIODVSRGB_HEADER 1
////////////////////////////////////////////////////
//! file="Ravl/DVSFileFormat/ImgIOdvsrgb.hh"
//! lib=RavlRawVidIO
//! docentry="Ravl.API.Images.Video.Video IO"
//! example=exImgSeq.cc
//! rcsid="$Id: ImgIOdvsrgb.hh 5240 2005-12-06 17:16:50Z plugger $"
//! author="Simon Ennis"
//! date="13/05/2007"

#include "Ravl/DP/SPort.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/Stream.hh"
#include "Ravl/Assert.hh"
#include "Ravl/Image/Utilities.hh"
#include "Ravl/Image/NewGrabfileReader.hh"
#include "Ravl/Image/NewGrabfileWriter.hh"
#include "Ravl/IO.hh"
#include "Ravl/Image/WarpScale.hh"

using namespace RavlImageN;
using namespace std;
using namespace RavlN;

namespace RavlImageN {

  ///////////////////////////////////
  //! userlevel=Develop
  //: Basic information about a raw rgb file
  
  class DPImageDVSRGBBaseBodyC {
  public:  
    DPImageDVSRGBBaseBodyC(const Index2dC &size = Index2dC(576,720));
    //: Constructor.
    // This constructs with the basic yuv format.
    
    bool SetupForVariant(const StringC &suffix);
    //: Setup for cif variant.
    // str should be one of :
    // ??
    
    bool SetupForVariant(const Index2dC &size);
    //: Setup for cif variant.
    // Where size in the size of the Y image.
    
    inline UInt64T CalcOffset(UInt64T frameNo) const  {
      RavlAssert(frameSize > 0);
      return frameSize * frameNo; 
    }
    //: Calculate the offset of a frame.
    
    inline Int64T CalcOffset(Int64T frameNo) const {
      RavlAssert(frameSize > 0);
      return frameSize * frameNo; 
    }
    //: Calculate the offset of a frame.
        
    void SetSequenceSize(UIntT val) { seqSize = val; }
    //: Set the sequence size.
    
    UIntT SeqSize() const { return seqSize; }
    //: Get the sequence size.
    
    
  protected:
    static inline bool is_odd (IndexC x) {return bool (x%2 != 0);}
    // Is an index odd ?
    
    ImageRectangleC rect; // Size of RGB variant. Origin 0,0
    UInt64T frameSize; // Size of one frame in bytes.
    UIntT frameNo;   // Current frameno.
    UIntT seqSize;   // Number of frames in sequence, ((UIntT) -1) if unknown
    UIntT blockSize; // Size of blocks.
    UIntT padSize;   // Pad size..
    bool rawMode; // Raw IO mode ?
    bool doneHdr; // Written header yet ?
    int fd; // File descriptor.
  };
  
  ///////////////////////////////////
  //! userlevel=Develop
  //: Load a RGB image in raw RGB format.
  
  class DPIImageDVSRGBBodyC 
    : public DPISPortBodyC<ImageC<ByteRGBValueC> >,
      public DPImageDVSRGBBaseBodyC
  {
  public:  
    DPIImageDVSRGBBodyC(GrabfileReaderC& file_reader,const IStreamC &nStrm,const Index2dC &size = Index2dC(576,720));
    //: Constructor from stream 
    
    DPIImageDVSRGBBodyC(GrabfileReaderC& file_reader,const StringC &fn,bool useHdr = true);
    //: Constructor from filename
    
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
    
    virtual ImageC<ByteRGBValueC> Get();
    //: Get next image.
    
    virtual bool Get(ImageC<ByteRGBValueC> &buff);
    //: Get next image.
    
    virtual bool IsGetReady() const 
      {
	return file_read.Ok();
      }
  //: Is some data ready ?
    // TRUE = yes.
    // Defaults to !IsGetEOS().
    
    virtual bool IsGetEOS() const
      {
	return file_read.HaveMoreFrames();
      }
    //: Has the End Of Stream been reached ?
    // TRUE = yes.
    
  protected:
    IStreamC strm;
    GrabfileReaderC& file_read;
  };
  
  ///////////////////////////////////
  //! userlevel=Develop
  //: Save a RGB image in raw RGB format.
  
  class DPOImageDVSRGBBodyC 
    : public DPOSPortBodyC<ImageC<ByteRGBValueC> >,
      public DPImageDVSRGBBaseBodyC
  {
  public:
    DPOImageDVSRGBBodyC(GrabfileWriterC& file_write, const OStreamC &nStrm,const Index2dC &size  = Index2dC(576,720));
    //: Constructor from stream 
    
    DPOImageDVSRGBBodyC(GrabfileWriterC& file_write, const StringC &fn,bool useHdr = true);
    //: Constructor from filename
    
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
    
    bool Put(const ImageC<ByteRGBValueC> &Img);
    //: Put image to a stream.
    
    virtual bool IsPutReady() const 
      { return strm.good(); }
    //: Read to write some data ?
    // TRUE = yes.
    
    virtual void PutEOS() {}
    //: Put End Of Stream marker.
    
  protected:  
    OStreamC strm;
    GrabfileWriterC& file_writer;
    mutable ImageC<ByteRGBValueC> Imgcopy;
    mutable VideoModeT vmode;
    mutable ByteFormatT bformat;
    mutable IntT vbuffer;

  };
  
  //! userlevel=Normal
  //: Load a RGB image in RGB format.
  
  class DPIImageDVSRGBC 
    : public DPISPortC<ImageC<ByteRGBValueC> >
  {
  public:
    DPIImageDVSRGBC(GrabfileReaderC& file_reader,const StringC &fn);
    //: Constructor from filename.  
    
    DPIImageDVSRGBC(GrabfileReaderC& file_reader,const IStreamC &nStrm,const Index2dC &size = Index2dC(576,720))
      : DPEntityC(*new DPIImageDVSRGBBodyC(file_reader,nStrm,size))
      {}
    //: Constructor from stream 
    
  };
  
  //! userlevel=Normal
  //: Save a RGB image in raw RGB format.
  
  class DPOImageDVSRGBC 
    : public DPOSPortC<ImageC<ByteRGBValueC> >
  {
  public:
    DPOImageDVSRGBC(GrabfileWriterC& file_write,const StringC &fn);
    //: Constructor from filename.  
    
    DPOImageDVSRGBC(GrabfileWriterC& file_write,const OStreamC &nStrm,const Index2dC &size = Index2dC(576,720))
      : DPEntityC(*new DPOImageDVSRGBBodyC(file_write,nStrm,size))
      {}
    //: Constructor from stream 
    
  };
}
#endif
