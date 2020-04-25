// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_IMGIOJS_HEADER
#define RAVLIMAGE_IMGIOJS_HEADER 1
////////////////////////////////////////////////////
//! docentry="Ravl.API.Images.Video.Video IO"
//! rcsid="$Id$"
//! author="Charles Galambos"
//! date="24/03/2002"
//! lib=RavlVideoIO
//! file="Ravl/Image/VideoIO/ImgIOjs.hh"

#include "Ravl/DP/SPort.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteYUV422Value.hh"
#include "Ravl/Stream.hh"
#include "Ravl/Assert.hh"

namespace RavlImageN {
  
  ///////////////////////////////////
  //! userlevel=Develop
  //: Basic information about a Jaleo js file

  class DPImageJSBaseBodyC {
  public:  
    DPImageJSBaseBodyC();
    //: Constructor.
    // This constructs with the basic yuv format.
    
    DPImageJSBaseBodyC(const StringC &filename,bool read);
    //: Constructor.

    DPImageJSBaseBodyC(DPISPortC<ByteT> inputPort);
    //: Constructor.

    DPImageJSBaseBodyC(DPOPortC<ByteT> outputPort);
    //: Constructor.

    bool ReadHeader();
    //: Read header from stream.
    
    inline StreamPosT CalcOffset(StreamPosT frameNo) const {
      RavlAssert(frameSize > 0);
      return offset + (frameSize * frameNo); 
    }
    //: Calculate the offset of a frame.

    void SetSequenceSize(Int64T val) { seqSize = val; }
    //: Set the sequence size.
    
    Int64T SeqSize() const { return seqSize; }
    //: Get the sequence size.
    
    void SetupIO();
    //: Setup paramiters needed for io.
    
  protected:     
    ImageRectangleC rect; // Size of YUV variant. Origin 0,0
    StreamPosT frameSize; // Size of one frame in bytes.
    StreamPosT framePadding;
    StreamPosT frameNo; // Current frameno.
    StreamPosT seqSize;  // Number of frames in sequence, streamPosUnknown if unknown
    StreamPosT blockSize;
    StreamPosT offset;  // Offset of start.
    
    DPISPortC<ByteT> m_inputStream;
    DPOPortC<ByteT> m_outputStream;
  };
  
  ///////////////////////////////////
  //! userlevel=Develop
  //: Load a Jaleo js file in YUV 422 format.
  
  class DPIImageJSBodyC 
    : public DPISPortBodyC<ImageC<ByteYUV422ValueC> >,
      public DPImageJSBaseBodyC
  {
  public:  
    DPIImageJSBodyC(const IStreamC &nStrm);
    //: Constructor from stream 

    DPIImageJSBodyC(const StringC &fileName);
    //: Constructor from a filename 
    
    DPIImageJSBodyC(DPISPortC<ByteT> inputPort);
    //: Constructor.

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

    virtual bool Seek64(StreamPosT off);
    //: Seek to location in stream.
    // Returns FALSE, if seek failed. (Maybe because its
    // not implemented.)
    // if an error occurered (Seek returned False) then stream
    // position will not be changed.

    virtual bool DSeek64(StreamPosT off);
    //: Delta Seek, goto location relative to the current one.

    virtual StreamPosT Tell64() const;
    //: Find current location in stream.

    virtual StreamPosT Size64() const;
    //: Find the total size of the stream.

    virtual ImageC<ByteYUV422ValueC> Get();
    //: Get next image.
    
    virtual bool Get(ImageC<ByteYUV422ValueC> &buff);
    //: Get next image.
    
    virtual bool IsGetReady() const 
    { return m_inputStream.IsValid() && m_inputStream.IsGetReady(); }
    //: Is some data ready ?
    // TRUE = yes.
    // Defaults to !IsGetEOS().
    
    virtual bool IsGetEOS() const
    { return m_inputStream.IsValid() && m_inputStream.IsGetEOS(); }
    //: Has the End Of Stream been reached ?
  // TRUE = yes.
  };
  
  ///////////////////////////////////
  //! userlevel=Develop
  //: Save a Jaleo js file in YUV 422 format.
  
  class DPOImageJSBodyC 
    : public DPOSPortBodyC<ImageC<ByteYUV422ValueC> >,
      public DPImageJSBaseBodyC
  {
  public:
    DPOImageJSBodyC(const OStreamC &nStrm);
    //: Constructor from stream 

    DPOImageJSBodyC(const StringC &nStrm);
    //: Constructor from stream 
    
    DPOImageJSBodyC(DPOPortC<ByteT> outputPort);
    //: Constructor.

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

    virtual bool Seek64(StreamPosT off);
    //: Seek to location in stream.

    virtual bool DSeek64(StreamPosT off);
    //: Delta Seek, goto location relative to the current one.

    virtual StreamPosT Tell64() const;
    //: Find current location in stream.

    virtual StreamPosT Size64() const;
    //: Find the total size of the stream.

    bool Put(const ImageC<ByteYUV422ValueC> &Img);
    //: Put image to a stream.
    
    virtual bool IsPutReady() const 
    { return m_outputStream.IsValid() && m_outputStream.IsPutReady(); }
    //: Read to write some data ?
    // TRUE = yes.
    
    virtual void PutEOS() {}
    //: Put End Of Stream marker.
    
  protected:  
    bool WriteHeader(const ImageRectangleC &wrect);
    //: Write js header.
    
    bool m_doneHeader;
    bool m_doFramePadding;
  };

  //! userlevel=Normal
  //: Load a Jaleo js file in YUV 422 format.
  
  class DPIImageJSC 
    : public DPISPortC<ImageC<ByteYUV422ValueC> >
  {
  public:
    DPIImageJSC(const StringC &fn);
    //: Constructor from filename.  
    
    DPIImageJSC(const IStreamC &nStrm)
    : DPEntityC(*new DPIImageJSBodyC(nStrm))
    {}
    //: Constructor from stream 
    
    DPIImageJSC(DPISPortC<ByteT> inputPort)
    : DPEntityC(*new DPIImageJSBodyC(inputPort))
    {}
    //: Constructor.
  };
  
  //! userlevel=Normal
  //: Save a js file in YUV 422 format.
  
  class DPOImageJSC 
    : public DPOSPortC<ImageC<ByteYUV422ValueC> >
  {
  public:
    DPOImageJSC(const StringC &fn);
    //: Constructor from filename.  
    
    DPOImageJSC(const OStreamC &nStrm)
    : DPEntityC(*new DPOImageJSBodyC(nStrm))
    {}
    //: Constructor from stream 
    
    DPOImageJSC(DPOPortC<ByteT> outputPort)
    : DPEntityC(*new DPOImageJSBodyC(outputPort))
    {}
    //: Constructor.
  };

}

#endif
