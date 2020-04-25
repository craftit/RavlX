// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#include "Ravl/Image/LegacyGrabfileReader.hh"
//! lib=RavlRawVidIO

#include "Ravl/Assert.hh"
#include "Ravl/Image/Utilities.hh"
#include <arpa/inet.h>

namespace RavlImageN {
//==========================================================================//

LegacyGrabfileReaderC::~LegacyGrabfileReaderC()
{
  if(m_infile.is_open()) {
    m_infile.close();
  }
}

// Open file and read file header.
bool LegacyGrabfileReaderC::Open(const char* const filename)
{
  // Is the file alread open? It shouldn't be!
  if(m_infile.is_open()) {
    RavlIssueError("LegacyGrabfileReader: Class already contains an open file.");
  }

    m_infile.open(filename, std::ios::binary | std::ios::in);
  // We can now read the header.
  
   IntT pos = m_infile.tellg();

   uint32_t dummy_int = 0;
   UIntT vbtemp = 0;
  // Video buffer size  
  m_infile.read(reinterpret_cast<char*>(&vbtemp), sizeof(vbtemp));

  m_video_buffer_size = vbtemp;
  
  // Audio buffer size
  m_infile.read(reinterpret_cast<char*>(&dummy_int), 4);
 
  m_audio_buffer_size = dummy_int; //asize;
  
  m_infile.seekg(pos);
  

  return Ok();
}

//--------------------------------------------------------------------------//

// Close file.
void LegacyGrabfileReaderC::Close()
{
  m_infile.close();
}


//-------------------------------------------------------------------------//
bool LegacyGrabfileReaderC::HaveMoreFrames()
{
  m_infile.peek();
  return m_infile.eof();
}



//--------------------------------------------------------------------------//

// Are there any problems with the IO?
bool LegacyGrabfileReaderC::Ok() const
{
  return m_infile.good();
}

//--------------------------------------------------------------------------//

// Read the next frame to a buffer.
bool LegacyGrabfileReaderC::GetNextFrame(BufferC<char> &bu, UIntT &vsize, UIntT &asize)
{
  bool ok = HaveMoreFrames();
 
  if(!ok) {
 
   uint32_t dummy_int = 0;
   UIntT vbtemp = 0;
  // Video buffer size  
  m_infile.read(reinterpret_cast<char*>(&vbtemp), sizeof(vbtemp));
  vsize = vbtemp;
  m_video_buffer_size = vbtemp;
  
  // Audio buffer size
  m_infile.read(reinterpret_cast<char*>(&dummy_int), 4);
  asize = dummy_int;
  m_audio_buffer_size = asize;
    
    m_infile.read(reinterpret_cast<char*>(&videoOffsets[0]),8);

    m_infile.read(reinterpret_cast<char*>(&audioOffsets[0]),32);
  
    m_infile.read(reinterpret_cast<char*>(&m_number_of_frames),4);
   
    m_infile.read(reinterpret_cast<char*>(&m_frame_rate),8);

    m_infile.read(reinterpret_cast<char*>(&videomode),4);

    ByteFormatT temp;
    m_infile.read(reinterpret_cast<char*>(&temp),4);
    
    byteformat = temp;

    ColourModeT cmt;
    m_infile.read(reinterpret_cast<char*>(&cmt),4);
    colourmode = cmt;
    

    AudioChannelsT audioChannels;
    AudioBitsT audioBits;
    AudioFreqT audioFreq;
  
    m_infile.read(reinterpret_cast<char*>(&audioChannels),4);
   
    m_infile.read(reinterpret_cast<char*>(&audioBits),4);
 
    m_infile.read(reinterpret_cast<char*>(&audioFreq),4);
   
    if(m_infile.good()) {
      int csize = m_video_buffer_size + m_audio_buffer_size;
      char * obuf = new char[csize];
      m_infile.read(bu.BufferAccess().DataStart(),csize);  //obuf,csize);
      delete obuf;
   }
   ++m_frames_loaded;
  }
  
  return ok;
}



} // End DVSN namespace
