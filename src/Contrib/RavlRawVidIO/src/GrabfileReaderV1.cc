// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001-15, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#include "Ravl/Image/GrabfileReaderV1.hh"
#include "Ravl/Image/GrabfileCommon.hh"
//! lib=RavlRawVidIO

#include "Ravl/Assert.hh"
#include "Ravl/DArray1d.hh"
#include <arpa/inet.h>
#include <stdint.h>

namespace RavlImageN {
//==========================================================================//

GrabfileReaderV1C::~GrabfileReaderV1C()
{
  if(m_infile.is_open()) {
    m_infile.close();
  }
}

//--------------------------------------------------------------------------//

// Open file and read file header.
bool GrabfileReaderV1C::Open(const char* const filename)
{ 
  // Is the file alread open? It shouldn't be!
  if(m_infile.is_open()) {
    RavlIssueError("GrabfileReaderV1: Class already contains an open file.");
  }

  bool ok = true;
  // Is the file a grabfile?
  if(ok) {
    m_infile.open(filename, std::ios::binary | std::ios::in);
    ok = IsGrabfile(m_infile);
  }

  // Is the version correct, (this code will only deal with version 1,
  // hence the hardcode below.
  if(ok) {
    ok = (FileVersion(m_infile) == m_version_number);
  }
  // We can now read the header.
  if(ok) {
    int dummy_int = 0;

    // Video buffer size
    m_infile.read(reinterpret_cast<char*>(&dummy_int), 4);
    m_video_buffer_size = ntohl(dummy_int);

    // Audio buffer size
    m_infile.read(reinterpret_cast<char*>(&dummy_int), 4);
    m_audio_buffer_size = ntohl(dummy_int);

   // Set audio channel sizes.
    chansize = m_audio_buffer_size / 8;
    audioOffsets[0] = m_video_buffer_size;
    audioOffsets[1] = m_video_buffer_size+chansize;
    audioOffsets[2] = m_video_buffer_size+(2*chansize);
    audioOffsets[3] = m_video_buffer_size+(3*chansize);
    audioOffsets[4] = m_video_buffer_size+(4*chansize);
    audioOffsets[5] = m_video_buffer_size+(5*chansize);
    audioOffsets[6] = m_video_buffer_size+(6*chansize);
    audioOffsets[7] = m_video_buffer_size+(7*chansize);
    //Set video offsets.
    videoOffsets[0] = 0;

   // Originally billed as "code to possibly deal with time codes in the
   // future", this field now receives the frame count once frames are 
   // written to the grab file.
    m_infile.read(reinterpret_cast<char*>(&dummy_int),4);
    m_frames_loaded = ntohl(dummy_int);

    m_infile.read(reinterpret_cast<char*>(&dummy_int),4);
    m_frame_rate = ntohl(dummy_int);
    // Original code put out 8 bytes for the frame rate (from a uint32_t - go
    // figure!) so we now have to skip the extra 4 (writer has been fixed to
    // put out the rate data correctly but now puts it out twice to maintain
    // the file layout).
    m_infile.read(reinterpret_cast<char*>(&dummy_int),4);

    UIntT id = m_infile.get();
    videomode = id;

    char id2 = m_infile.get();
    byteformat = int(id2);

    char id3 = m_infile.get();
    colourmode = int(id3);

    ok = Ok();
  }

  if(!ok) {
    Close();
  }

  return ok;
}

//--------------------------------------------------------------------------//

// Close file.
void GrabfileReaderV1C::Close()
{
  m_infile.close();
}

//--------------------------------------------------------------------------//

// Are there any problems with the IO?
bool GrabfileReaderV1C::Ok() const
{
  return m_infile.good();
}

//--------------------------------------------------------------------------//

bool GrabfileReaderV1C::HaveMoreFrames()
{
  m_infile.peek();
  return m_infile.eof();
}

//--------------------------------------------------------------------------//

//--------------------------------------------------------------------------//

bool GrabfileReaderV1C::Seek(UInt64T off)
{
  m_infile.clear();
  m_infile.seekg(off+header_size,ios::beg);
  m_frame_number = (off/(m_video_buffer_size + m_audio_buffer_size)); 
  return m_infile.eof();
}

//--------------------------------------------------------------------------//

//--------------------------------------------------------------------------//

bool GrabfileReaderV1C::DSeek(UInt64T off)
{
  m_infile.seekg(off,ios::cur);
  IntT numframes = (off/(m_video_buffer_size + m_audio_buffer_size));
  m_frame_number = m_frame_number + numframes;
  return m_infile.eof();
}

//--------------------------------------------------------------------------//


// Read the next frame to a buffer.
bool GrabfileReaderV1C::GetNextFrame(BufferC<char> &bu, UIntT &vsize, UIntT &asize)   //(DVSBufferC &buffer)
{

  // Set the card mode.
  bool ok = HaveMoreFrames();
  if(!ok) {
 
 
   uint32_t dummy_int = 0;
  // Video buffer size
  m_infile.read(reinterpret_cast<char*>(&dummy_int), 4);
  vsize = ntohl(dummy_int);

  // Audio buffer size
  m_infile.read(reinterpret_cast<char*>(&dummy_int), 4);
  asize = ntohl(dummy_int);
  
    if(m_infile.good()) {
      int csize = m_video_buffer_size + m_audio_buffer_size; 
      //char * obuf = new char[csize];
      //char * start = obuf;
      //BufferC<char> buffy(csize);
      m_infile.read(bu.BufferAccess().DataStart(),csize);
      //bu = buffy;  //BufferC<char> (buffy.Size(),buffy.BufferAccess().DataStart(),true,false);
      /*char * buffypointer = buffy.BufferAccess().DataStart();
      char * bupointer = bu.BufferAccess().DataStart();
      for(int y=0;y<csize;y++) {
         *bupointer = *buffypointer;
         bupointer++;
         buffypointer++;
      }*/
      //delete obuf;
   }
   ++m_frames_loaded;
   ++m_frame_number;
  }
  return ok;
}


BufferC<char> GrabfileReaderV1C::GetNextFrame()
{
  uint32_t dummy_int = 0;
  // Video buffer size
  m_infile.read(reinterpret_cast<char*>(&dummy_int), 4);

  // Audio buffer size
  m_infile.read(reinterpret_cast<char*>(&dummy_int), 4);
 
  bool ok = m_infile.good();
  RavlAlwaysAssertMsg(ok, "ERROR in GrabfileReaderV1C::GetNextFrame when reading i/p file");
  if(ok) {
      if(IdToByteFormat(byteformat) == BITS_8) {
         BufferC<char> video(m_video_buffer_size);
         m_infile.read(video.BufferAccess().DataStart(),m_video_buffer_size);     
         ++m_frames_loaded;
	 ++m_frame_number;
         BufferC<char> audio(m_audio_buffer_size);
         m_infile.read(audio.BufferAccess().DataStart(),m_audio_buffer_size);
#if (RAVL_COMPILER_GCC3)     /* gcc 3.x work around for gcc as mem dosent seem to get deleted.*/
         return BufferC<char> (video.Size(),video.BufferAccess().DataStart(),true,false);
#else
         return BufferC<char> (video.Size(),video.BufferAccess().DataStart(),true,true);
         //return BufferC<char> (video.Size(),video.BufferAccess().DataStart(),true,false);
#endif	 
         
      }
      if(IdToByteFormat(byteformat) == BITS_10_DVS) {
         //Convert to 8 Bits
         unsigned int osize = m_video_buffer_size * 3 / 4 ;
         BufferC<char> nextframe(osize);
         char * obuf = nextframe.BufferAccess().DataStart();
	 
         BufferC<char> video(m_video_buffer_size);
         m_infile.read(video.BufferAccess().DataStart(),m_video_buffer_size);
	 
         BufferC<char> audio(m_video_buffer_size);
         m_infile.read(audio.BufferAccess().DataStart(),m_audio_buffer_size);     

	 //const 
         char * vbuf = video.BufferAccess().DataStart();
         for ( IntT vcount = 0 ; vcount < (m_video_buffer_size/4)  ; ++ vcount ) {
            *obuf++ = *vbuf++ ;
            *obuf++ = *vbuf++ ;
            *obuf++ = *vbuf++ ;
            vbuf ++ ;
	}
	++m_frames_loaded;
	++m_frame_number;
        vbuf = 0;
        obuf = 0;
        delete vbuf;
        delete obuf;
        audio = BufferC<char>();
        video = BufferC<char>();
#if (RAVL_COMPILER_GCC3)   /* gcc 3.x work around for gcc as mem dosent seem to get deleted.*/
        return BufferC<char> (osize, nextframe.BufferAccess().DataStart(), true, false);
#else
        return BufferC<char> (osize, nextframe.BufferAccess().DataStart(), true, true);
        //return BufferC<char> (osize, nextframe.BufferAccess().DataStart(), true, false);  
#endif
      }
  }
     return BufferC<char>();
  
}

BufferC<UInt16T> GrabfileReaderV1C::GetNextFrame16T()
{
  uint32_t dummy_int = 0;
  // Video buffer size
  m_infile.read(reinterpret_cast<char*>(&dummy_int), 4);

  // Audio buffer size
  m_infile.read(reinterpret_cast<char*>(&dummy_int), 4);
 
 
  bool ok = m_infile.good();
  if(ok) {
      if(IdToByteFormat(byteformat) == BITS_8) {
         BufferC<char> video(m_video_buffer_size);
         BufferC<UInt16T> uint16tvideo(m_video_buffer_size);
         m_infile.read(video.BufferAccess().DataStart(),m_video_buffer_size);     
         ++m_frames_loaded;
	 ++m_frame_number;
         BufferC<char> audio(m_audio_buffer_size);
         m_infile.read(audio.BufferAccess().DataStart(),m_audio_buffer_size);
         UInt16T * poin = uint16tvideo.BufferAccess().DataStart();
         char * charpoin = video.BufferAccess().DataStart();
         for(int index=0;index<m_video_buffer_size;index++) {
            (*poin++) = (UInt16T)(*charpoin++);
         }
         poin = 0;
         charpoin = 0;
         audio = BufferC<char>();
         video = BufferC<char>();
         delete poin;
         delete charpoin; 
         return BufferC<UInt16T> (video.Size(),uint16tvideo.BufferAccess().DataStart(),true,false);
	 
         
      }
      if(IdToByteFormat(byteformat) == BITS_10_DVS) {
         //Convert to 8 Bits
         //Writing data as UInt16T so twice the size of chars
	 
         BufferC<char> video(m_video_buffer_size);
         m_infile.read(video.BufferAccess().DataStart(),m_video_buffer_size);
	 
         BufferC<char> audio(m_video_buffer_size);
         m_infile.read(audio.BufferAccess().DataStart(),m_audio_buffer_size);
     
        char c1,c2,c3,c4;
        BufferC<UInt16T> buffertoconvert((m_video_buffer_size/4)*3);
        char * videoptr = video.BufferAccess().DataStart();

        UInt16T * nextTenPtr = buffertoconvert.BufferAccess().DataStart();

        //Mask values to extract the necessary bits for the 10 bit data.
        ByteT mask1 = 48;
        ByteT mask2 = 12;
        ByteT mask3 = 3;
        ByteT bitsone,bitstwo,bitsthree;
        UInt16T first_ten_bits,second_ten_bits,third_ten_bits;

        for ( IntT vcount = 0 ; vcount < (m_video_buffer_size/4)  ; ++ vcount ) {
            //Get the next 4 chars.
            c1 = *videoptr++;
            c2 = *videoptr++;
            c3 = *videoptr++;
            c4 = *videoptr++;
            //Get the extra 2 bits with masking.    
            bitsone = c4 & mask1;
            bitstwo = c4 & mask2;
            bitsthree = c4 & mask3;
            //Shift first 8 bits along and add last 8 bits to UInt16T.
            first_ten_bits = (c1 << 8) + (bitsone << 2);
            second_ten_bits = (c2 << 8) + (bitstwo << 4);
            third_ten_bits = (c3 << 8) + (bitsthree << 6);
            //Shift the first 6 bits off as first 2 bits of the above last 8 should be the lsfb.
           /* first_ten_bits = (first_ten_bits >> 6);
            second_ten_bits = (second_ten_bits >> 6);
            third_ten_bits = (third_ten_bits >> 6);
            //And out the first 6 bits.
            first_ten_bits = first_ten_bits & buf;
            second_ten_bits = second_ten_bits & buf;
            third_ten_bits = third_ten_bits & buf;*/
            //Assign 10 bit values to UInt16T buffer.
            (*nextTenPtr++) = first_ten_bits;
            (*nextTenPtr++) = second_ten_bits;
            (*nextTenPtr++) = third_ten_bits;
         }
	++m_frames_loaded;
	++m_frame_number;
        nextTenPtr = 0;
        videoptr = 0;
        delete nextTenPtr;
        delete videoptr;
        audio = BufferC<char>();
        video = BufferC<char>();
        return BufferC<UInt16T> (buffertoconvert.Size(), buffertoconvert.BufferAccess().DataStart(), true, false);
      }
  }
     return BufferC<UInt16T>();
  
}

} // End DVSN namespace
