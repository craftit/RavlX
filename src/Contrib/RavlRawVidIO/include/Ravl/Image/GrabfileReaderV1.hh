// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#if !defined(RAVL_GRABFILEREADERV1_HEADER)
#define RAVL_GRABFILEREADERV1_HEADER
//! lib=RavlRawVidIO
//! author="Simon Tredwell"
//! docentry="Ravl.API.Images.Video.Video IO.RawVid"

#include <fstream>
#include "Ravl/Image/Utilities.hh"
#include "Ravl/Image/GrabfileReader.hh"
#include "Ravl/DArray1d.hh"
#include "Ravl/Buffer.hh"

namespace RavlImageN {

//class DVSBufferC;

//! userlevel=Normal
//: Read raw video files with header version 1.
// This class will read Version 1 grabfiles.
class GrabfileReaderV1C : public GrabfileReaderC {
public:
  GrabfileReaderV1C()
    :
    GrabfileReaderC(),
    m_infile(),
    m_video_buffer_size(0),
    m_audio_buffer_size(0),
    m_frame_number(0),
    m_frames_loaded(0)
  {
    // Do nothing
  }
  //: Default constuctor

  virtual ~GrabfileReaderV1C();
  //: Destructor

  //==========================================================================//
  typedef GrabfileCommonN::frame_number_t frame_number_t;
  //: Type to store the frame number.

  // Virtual functions

  virtual bool Open(const char* const filename);
  //: Open file and read file header.

  virtual void Close();
  //: Close file.

  virtual bool Ok() const;
  //: Are there any problems with the IO?

  virtual bool HaveMoreFrames();

  virtual bool Seek(UInt64T off);

  virtual bool DSeek(UInt64T off);

  virtual bool GetNextFrame(BufferC<char> &bu,UIntT &vsize,UIntT &asize);
  //: Read the next frame to a buffer.

  virtual BufferC<char> GetNextFrame();
  //: Read the next frame to a buffer.

  virtual BufferC<UInt16T> GetNextFrame16T();
  //: Read the next frame to a buffer.

  virtual int Version() const {return m_version_number;}
  //: The version of the reader.

  virtual IntT VideoMode() {return videomode;}
  //: Return the video mode.

  virtual IntT ColourMode() {return colourmode;}
  //: Return the colour mode.

  virtual ByteFormatT getByteFormat() { 
  return IdToByteFormat((UIntT)byteformat);}
  //: Return the byte format.

  virtual ColourModeT getColourMode() { 
  return IdToColourMode((UIntT)colourmode);}
  //: Return the colour mode.

  virtual RealT FrameRate() {return m_frame_rate;}
  //: Return the frame rate.

  virtual VideoOffsetsT getVideoOffsets() {return videoOffsets;}
  //: Return video offsets.

  virtual AudioOffsetsT getAudioOffsets() {return audioOffsets;}
  //: Return audio offsets.

  virtual IntT getVideoBufSize() {return m_video_buffer_size;}
  //: Return video offsets.

  virtual IntT getAudioBufSize() {return m_audio_buffer_size;}
  //: Return audio offsets.

  //virtual IntT getFrameNumber() {return m_frames_loaded;}

  virtual frame_number_t FramesLoaded() const { return m_frames_loaded; }
  //: Return the number of frames loaded.

  virtual frame_number_t FrameNumber() const {return m_frame_number;}
  //: The frame number of the most recently loaded frame.



protected:
  std::ifstream m_infile;
  //: Stream we are reading from.

  IntT m_video_buffer_size;
  //: The video buffer size in bytes.

  IntT m_audio_buffer_size;
  //: The audio buffer size in bytes.

  IntT chansize;
  //: Audio offset size.

  RealT m_frame_rate;
  //: The frame rate.

  IntT videomode,byteformat,colourmode;
  //: The card mode.

  frame_number_t m_frame_number;
  //: The frame number of the most recently loaded frame.

  frame_number_t m_frames_loaded;
  //: The number of frames loaded.

  VideoOffsetsT videoOffsets;
  AudioOffsetsT audioOffsets;
  //: Offsets.
private:
  static const int m_version_number = 1;
  //: The version number.

  static const IntT header_size = 31;
  //: Size of the header in bytes.
};

}

#endif // RAVL_GRABFILEREADERV1_HEADER
