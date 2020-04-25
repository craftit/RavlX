// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU
// General Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! file = "Ravl/Contrib/DVDRead/ImgIOMPEG2DVD.hh"
//! lib=RavlDVDRead
//! author = "Warren Moore"
//! docentry = "Ravl.API.Images.Video.Video IO.LibDVDRead"

#ifndef RAVL_IMGIOMPEG2DVD_HEADER
#define RAVL_IMGIOMPEG2DVD_HEADER 1

#include "Ravl/Image/ImgIOMPEG2.hh"
#include "Ravl/Image/MPEG2Demux.hh"
#include "Ravl/DVDRead.hh"

namespace RavlImageN
{
  
  class ImgILibMPEG2DVDBodyC :
    public ImgILibMPEG2BodyC
  {
  public:
    ImgILibMPEG2DVDBodyC(MPEG2DemuxC &demux, DVDReadC &dvd);
    //: Constructor.
    
    virtual ~ImgILibMPEG2DVDBodyC() {}
    //: Destructor.
    
    virtual bool Get(ImageC<ByteRGBValueC> &img);
    //: Get next frame.
    
    virtual bool Seek(UIntT off);
    //: Seek to location in stream.
    
    virtual UIntT Size() const;
    //: Get the size of the file in frames (-1 if not known)
    
    virtual bool Seek64(StreamPosT off);
    //: Seek to location in stream.
    
    virtual StreamPosT Size64() const;
    //: Get the size of the file in frames (-1 if not known)
    
    bool Reset();
    //: Reset the decoder
    
  protected:
    MPEG2DemuxC m_demux;                          // Demultiplexer object
    DVDReadC m_dvd;                               // DVD reader
    StreamPosT m_frames;                          // Frame total
    StreamPosT m_seekBoundary;                    // DVD seek boundary
  };
  
  class ImgILibMPEG2DVDC :
    public ImgILibMPEG2C
  {
  public:
    ImgILibMPEG2DVDC() :
      DPEntityC(true)
    {}
    //: Default constructor.
    // Creates an invalid handle.

    ImgILibMPEG2DVDC(MPEG2DemuxC &demux, DVDReadC &dvd) :
      DPEntityC(*new ImgILibMPEG2DVDBodyC(demux, dvd))
    {}
    //: Constructor.
    
    bool Reset()
    { return Body().Reset(); }
    //: Reset the decoder
    
  protected:
    ImgILibMPEG2DVDBodyC &Body()
    { return static_cast<ImgILibMPEG2DVDBodyC &>(ImgILibMPEG2C::Body()); }
    //: Access body.

    const ImgILibMPEG2DVDBodyC &Body() const
    { return static_cast<const ImgILibMPEG2DVDBodyC &>(ImgILibMPEG2C::Body()); }
    //: Access body.
  };
}

#endif
