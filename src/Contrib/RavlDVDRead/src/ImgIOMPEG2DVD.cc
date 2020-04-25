// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU
// General Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! file = "Ravl/Contrib/DVDRead/ImgIOMPEG2DVD.cc"
//! lib=RavlDVDRead
//! author = "Warren Moore"

#include <stdint.h>
#include "Ravl/ImgIOMPEG2DVD.hh"
#include "Ravl/Image/MPEG2Demux.hh"
#include "Ravl/Threads/Signal.hh"

#define DODEBUG 0

#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlImageN
{
  using namespace RavlN;
  
  ImgILibMPEG2DVDBodyC::ImgILibMPEG2DVDBodyC(MPEG2DemuxC &demux, DVDReadC &dvd) :
    ImgILibMPEG2BodyC(false),
    m_demux(demux),
    m_dvd(dvd),
    m_frames(-1),
    m_seekBoundary(5)
  {
    RavlAssertMsg(m_demux.IsValid(), "ImgILibMPEG2DVDBodyC::ImgILibMPEG2DVDBodyC invalid MPEG2 demultiplexer");
    RavlAssertMsg(m_dvd.IsValid(), "ImgILibMPEG2DVDBodyC::ImgILibMPEG2DVDBodyC invalid DVD reader");

    // Attach the signals
    Signal0C signalFlush = m_dvd.SignalFlush();
    Connect(signalFlush, m_demux, &MPEG2DemuxC::Reset);
    ConnectRef(signalFlush, *this, &ImgILibMPEG2DVDBodyC::Reset);

    // Get the frame size
    StringC frames;
    m_dvd.GetAttr("frames", frames);
    m_frames = frames.Int64Value();
    
    // Get the FPS
    StringC fps;
    m_dvd.GetAttr("framerate", frames);
    m_seekBoundary = Floor(frames.RealValue() / 2);
    ONDEBUG(cerr << "ImgILibMPEG2DVDBodyC::ImgILibMPEG2DVDBodyC frames(" << m_frames << ") seek boundary(" << m_seekBoundary << ")" << endl;)
  }
  
  bool ImgILibMPEG2DVDBodyC::Get(ImageC<ByteRGBValueC> &img)
  {
    ONDEBUG(cerr << "ImgILibMPEG2DVDBodyC::Get (" << m_frameNo << ")" << endl;)
    
    bool ok = true;
    Tuple2C< ImageC<ByteRGBValueC>, IntT > dat;
    while (!m_imageCache.LookupR(m_frameNo, dat))
    {
      // If the last read failed, don't bother again
      if (!ok)
      {
        cerr << "ImgILibMPEG2DVDBodyC::Get decoder failed for frame (" << m_frameNo << ")" << endl;
        return false;
      }

      // Keep on truckin' til we find the frame
      ok = DecodeGOP(m_frameLast);

      // If at EOS, stop
      if (IsGetEOS())
      {
        cerr << "ImgILibMPEG2DVDBodyC::Get at end of stream before frame (" << m_frameNo << ")" << endl;
        return false;
      }
    }
    
    // Extract the cached image
    img = dat.Data1();
    m_lastFrameType = dat.Data2();
    
    // Move counter to the next frame
    m_frameNo++;
    
    return true;
  }
  
  bool ImgILibMPEG2DVDBodyC::Seek(UIntT off)
  {
    // Dop't seek the DVD if we don't have to
    ONDEBUG(cerr << "ImgILibMPEG2DVDBodyC::Seek frame(" << off << ") offset(" << (off - m_frameNo) << ")" << endl;)
    if ((off - m_frameNo < 0) || (off - m_frameNo) > m_seekBoundary)
      // Find the closest dvd frame
      m_dvd.SeekFrame(off, m_frameLast);
    
    // Store the seek frame
    m_frameNo = off;
    
    // Recheck for end
    m_endFound = false;
    
    return true;
  }
    
  UIntT ImgILibMPEG2DVDBodyC::Size() const
  {
    return m_frames;
  }
    
  bool ImgILibMPEG2DVDBodyC::Seek64(StreamPosT off)
  {
    // Dop't seek the DVD if we don't have to
    ONDEBUG(cerr << "ImgILibMPEG2DVDBodyC::Seek64 offset(" << (off - m_frameNo) << ")" << endl;)
    if ((off - m_frameNo < 0) || (off - m_frameNo) > m_seekBoundary)
      // Find the closest dvd frame
      m_dvd.SeekFrame(off, m_frameLast);
    
    // Store the seek frame
    m_frameNo = off;
    
    // Recheck for end
    m_endFound = false;
    
    return true;
  }
    
  StreamPosT ImgILibMPEG2DVDBodyC::Size64() const
  {
    return m_frames;
  }

  bool ImgILibMPEG2DVDBodyC::Reset()
  {
    return ImgILibMPEG2BodyC::Reset();
  }
  
}

