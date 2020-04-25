// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#if !defined(RAVL_GRABFILEREADER_HEADER)
#define RAVL_GRABFILEREADER_HEADER
//! lib=RavlRawVidIO
//! author="Simon Tredwell"
//! docentry="Ravl.API.Images.Video.Video IO.RawVid"

#include "Ravl/Image/GrabfileCommon.hh"
#include "Ravl/Image/Utilities.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/DArray1d.hh"
#include "Ravl/Buffer.hh"

namespace RavlImageN {
using namespace RavlImageN;
//class DVSBufferC;

//! userlevel=Normal
//: Read raw video files.
// This is the base class for Grabfile readers. It should not contain
// any pure virtual functions except for Version().
class GrabfileReaderC {
public:
  GrabfileReaderC()
    :
    m_frame_number(-1),
    m_frames_loaded(0)
  {
    // Do nothing
  }
  //: Default constuctor

  virtual ~GrabfileReaderC() {}
  //: Destructor

  //==========================================================================//

  typedef GrabfileCommonN::frame_number_t frame_number_t;
  //: Type to store the frame number.

  // Virtual functions

  virtual bool Open(const char* const filename) {return false;}  //, CardModeC& mode) {return false;}
  //: Open file and read file header.

  virtual void Close() {}
  //: Close file.

  virtual bool Ok() const {return false;}
  //: Are there any problems with the IO?

  virtual bool HaveMoreFrames() {return false;}
  //: Are there more frames to read?

  virtual bool Seek(UInt64T off) {return false;}
  //: Seek to position in file.

  virtual bool GetNextFrame(BufferC<char> &bu,UIntT &vsize,UIntT &asize) {return false;}
  //: Read the next frame to a buffer.

  virtual BufferC<char> GetNextFrame() {return BufferC<char>();}
  //: Read the next frame to a buffer.

  virtual BufferC<UInt16T> GetNextFrame16T() {return BufferC<UInt16T>();}
  //: Read the next frame to a buffer.
 
  virtual bool CanGetByFrameNumber() {return false;}
  //: Is the reader able to read a specific frame?

  virtual frame_number_t NumberOfFrames() {return 0;}
  //: Returns the number of frames in the sequence.
  // The method will return 0 if the number of frames is unknown.

  virtual int Version() const = 0;
  //: The version of the reader.

  virtual IntT VideoMode() {return -1;}
  //: Return the video mode.

  virtual IntT ColourMode() {return -1;}
  //: Return the colour mode.

  virtual ByteFormatT getByteFormat() {return ByteFormatT();}
  //: Return the byte format.

  virtual ColourModeT getColourMode() {return ColourModeT();}
  //: Return the colour mode.

  virtual RealT FrameRate() {return 0;}
  //: return the frame rate.

  // Non-virtual functions

  virtual frame_number_t FramesLoaded() const { return m_frames_loaded; }
  //: Return the number of frames loaded.

  virtual frame_number_t FrameNumber() const {return m_frame_number;}
  //: The frame number of the most recently loaded frame.

  virtual VideoOffsetsT getVideoOffsets() {return videoOffsets;}
  //: Return video offsets.

  virtual AudioOffsetsT getAudioOffsets() {return audioOffsets;}
  //: Return audio offsets.

  virtual IntT getVideoBufSize() {return 0;}
  //: Return video offsets.

  virtual IntT getAudioBufSize() {return 0;}
  //: Return audio offsets.

  // Static functions

  static const bool IsGrabfile(const char* const filename);
  //: Check to see whether the file in question is a grabfile.

  static const int FileVersion(const char* const filename);
  //: Get the file version number of a DVS grabfile.
  // This will return the version of the grabfile, or 0 if the file is
  // not a versioned grabfile.

protected:
  static const bool IsGrabfile(std::ifstream& infile);
  //: Checks to see whether the stream points to a grabfile.

  static const int FileVersion(std::ifstream& infile);
  //: Used to retrieve the version number of grabfile stream.
  // This will return the version of the grabfile, or 0 if the file is
  // not a versioned grabfile.

  frame_number_t m_frame_number;
  //: The frame number of the most recently loaded frame.

  frame_number_t m_frames_loaded;
  //: The number of frames loaded.

  VideoOffsetsT videoOffsets;
  AudioOffsetsT audioOffsets;
  //: Offsets.
};

}

#endif // RAVL_GRABFILEREADER_HEADER
