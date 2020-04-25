// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001-15, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#if !defined(RAVL_GRABFILEWRITERV1_HEADER)
#define RAVL_GRABFILEWRITERV1_HEADER
//! rcsid="$Id: $"
//! author="Simon Tredwell"
//! lib=RavlRawVidIO

#include "Ravl/Image/GrabfileWriter.hh"

#include <fstream>
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/Image/Utilities.hh"
#include <bitset>

namespace RavlImageN {

//class DVSBufferC;
//class DVSCardC;
//class CardModeC;

//! userlevel=Normal
//: Read raw video files with header version 1.
// The class will write Version 1 grabfiles.
class GrabfileWriterV1C : public GrabfileWriterC {
public:
  GrabfileWriterV1C()
    :
    m_outfile(),
    m_frame_number(0),
    m_frames_written(0)
  {
    // Do nothing
  }

  virtual ~GrabfileWriterV1C();
  typedef GrabfileCommonN::frame_number_t frame_number_t;

  //==========================================================================//

  virtual bool Open(const char* const filename,
		    VideoModeT videomode, ByteFormatT byteformat, ColourModeT colourmode, IntT videobuffersize = 0, IntT audiobuffersize = 0);
  //: Open file and write file header.

  virtual void Close();
  //: Close file.
  
  virtual bool Ok() const;
  //: Are there any problems with the IO?

//  virtual bool PutFrame(const DVSBufferC &buffer);
  //: Write frame.

  virtual bool PutFrame(BufferC<char> &fr,UIntT &fc);
  //: Write a frame.

  virtual bool PutFrame(SArray1dC<char> &re);
  //: Write frame.

  virtual void Reset(VideoModeT vmode,ByteFormatT bformat, IntT vbuf);
  //: Reset video buffer properties.

  virtual int VideoBuffer();
  //: Return video buffer size.

  virtual int Version() const {return m_version_number;}
  //: The version of the writer.

  virtual frame_number_t FramesWritten() const { return m_frames_written; }
  //: The number of frames written to the file.

protected:
  std::ofstream m_outfile;
  //: The output stream.

  int m_video_buffer_size;
  //: The video buffer size in bytes.

  int m_audio_buffer_size;
  //: The audio buffer size in bytes.

  int m_byte_format;
  //: The byte format.

  int pos;
  //: Position of the number of frames in the outputfile used to rewrite the number of frames upon closing the file.

  frame_number_t m_frame_number;
  //: The frame number of the most recently written frame.

  frame_number_t m_frames_written;
  //: The number of frames written.

private:
  static const int m_version_number = 1;
  //: The version number.

  static const int vbpos = 12;
  //: Position of videobuffer data in output file.

  static const int vmpos = 29;
  //: Position of video mode data in output file.

  static const int bfpos = 30;
  //: Position of video byte format data in output file.

  static const int nbrframes = 20;
  //: Position of number of frames data in output file.

  static const IntT frame_rate = 25;
  //: The frame rate.
};

}

#endif // RAVL_GRABFILEWRITERV1_HEADER
