// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_AVIISTREAM_HEADER
#define RAVL_AVIISTREAM_HEADER 1
////////////////////////////////////////////////////////////
//! author="Daniele Muntoni"
//! docentry="Ravl.API.Images.Video.Video IO"
//! date="26/03/2003"
//! rcsid="$Id$"
//! file="Ravl/Image/VideoIO/AviStream.hh"
//! example=exImgSeq.cc
//! lib=RavlVideoIO

#include "Ravl/GenBinStream.hh"
#include "Ravl/Image/DVFrame.hh"
#include "Ravl/Stream.hh"
#include "Ravl/DList.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteBGRValue.hh"
#include "Ravl/Image/ImgIO.hh"
#include "Ravl/DP/FileFormatIO.hh"

using namespace RavlImageN;

namespace RavlImageN{

  struct WaveFmtXST
  {
    UInt16T  wFormatTag;      
    UInt16T  nChannels;       
    UIntT nSamplesPerSec;  
    UIntT nAvgBytesPerSec; 
    UInt16T  nBlockAlign;     
    UInt16T  wBitsPerSample;  
    UInt16T  cbSize;          
  };
  
  //: Struct describing an Audio Stream Format(strf) for uncompressed audio
  //
  //  "wFormatTag" Defines the audio waveform type of the audio stream. 
  //  "nChannels" Specifies the number of channels in the audio stream, 1 for mono, 2 for stereo. 
  //  "nSamplesPerSec" Specifies the sample rate of the audio stream in samples/second (Hz).  
  //  "nAvgBytesPerSec" Specifies the average data rate. Playback software can estimate the buffer size by using this value. 
  //  "nBlockAlign" Specifies the block alignment of the data, in bytes. Playback software must process a multiple of nBlockAlign bytes of data at a time, so that the value of nBlockAlign can be used for buffer alignment. 
  //  "wBitsPerSample" Number of bits per sample per channel data ( same for all samples is assumed) . Set to 0 if not needed. 
  //  "cbSize"  Size, in bytes, of the extra information in the format header, not including the size of the WaveFmtXST structure. 
  
  struct DvInfoST
  {
    UIntT dwDVAAuxSrc;
    UIntT dwDVAAuxCtl;
    UIntT dwDVAAuxSrc1;
    UIntT dwDVAAuxCtl1;
    UIntT dwDVVAuxSrc;
    UIntT dwDVVAuxCtl;
    UIntT dwDVReserved[2];
  } ;

  //: Struct describing (optional) informations about the DV data stream
  //
  // "dwDVAAuxSrc" Audio Auxiliary Data Source Pack for the first audio block (first 5 DV DIF sequences for 525-60 systems or 6 DV DIF sequences for 625-50 systems) of a frame. A DIF sequence is a data block that contains 150 DIF blocks. A DIF block consists of 80 bytes. 
  // "dwDVAAuxCtl" Specifies the Audio Auxiliary Data Source Control Pack for the first audio block of a frame..
  // "dwDVAAuxSrc1" Specifies the Audio Auxiliary Data Source Pack for the second audio block (second 5 DV DIF sequences for 525-60 systems or 6 DV DIF sequences for 625-50 systems) of a frame.
  // "dwDVAAuxCtl1" Specifies the Audio Auxiliary Data Source Control Pack for the second audio block of a frame.
  // "dwDVVAuxSrc" Specifies the Video Auxiliary Data Source Pack 
  // "dwDVVAuxCtl" Specifies the Video Auxiliary Data Source Control Pack. 
  // "DwDVReserved[2]" Reserved. Set this array to zero.
 

  struct BitmapInfoHST
  {
    UIntT biSize;
    UIntT biWidth;
    int biHeight;
    UInt16T biPlanes;
    UInt16T biBitCount;
    
    char biCompression[4];
    UIntT biSizeImage;
    UIntT biXPelsPerMeter;
    UIntT biYPelsPerMeter;
    UIntT biClrUsed;
    UIntT biClrImportant;
    
    DvInfoST dvinfo;
  } ;
  
  //: Struct describing a Video Stream Format(strf) 
  //  
  //  "biSize" It is the number of bytes required by this structure.
  //  "biWidth" Width of the bitmap, in pixels. 
  //  "biHeight" Height of the bitmap, in pixels. If positive, the bitmap is a bottom-up DIB and its origin is the lower left corner. If negative, the bitmap is a top-down DIB and its origin is the upper left corner. 
  //  "biPlanes" Specifies the number of planes for the target device. This value must be set to 1. 
  //  "biBitCount" Number of bits per pixel. If it is set to greater than eight, bitmaps can be considered true color (No color table is used). If set to less than or equal to eight, bitmap might use a palette or color table  
  //  "biCompression" tells the type of compression used or requested (It is sometimes human readable).  
  //  "biSizeImage" Size, in bytes, of the image. This can be set to 0 for uncompressed BGR bitmaps. 
  //  "biXPelsPerMeter" Horizontal resolution, in pixels per meter, of the target device for the bitmap. 
  //  "biYPelsPerMeter" Vertical resolution, in pixels per meter, of the target device for the bitmap. 
  //  "biClrUsed" Number of color indices in the color table that are actually used by the bitmap. If 0 the bitmap uses the maximum number of colors corresponding to the value of the biBitCount member for the compression mode specified by biCompression. 
  //  "biClrImportant" Number of color indices that are considered important for displaying the bitmap. If this value is zero, all colors are important.


  struct AviHeaderST
  {
    UIntT dwMicroSecPerFrame;
    UIntT dwMaxBytesPerSec;
    UIntT dwPaddingGranularity;
    char dwFlags[4];		
    UIntT dwTotalFrames;		
    UIntT dwInitialFrames;
    UIntT dwStreams;
    UIntT dwSuggestedBufferSize;
    
    UIntT dwWidth;
    UIntT dwHeight;
    
    UIntT dwReserved[4];
  } ;

  //: Struct describing the main avi header (avih) 
  //    
  //  "dwMicroSecPerFrame"  It's the number of microseconds between consecutive frames. This value is constant.
  //  "dwMaxBytesPerSec" Estimate of the maximum data rate of the stream. 
  //  "dwPaddingGranularity" The header should be padded to reach a size that is a multiple of this quantity (normally 2K). Non-MS files, however can consider this field as a reserved field. Set to zero if not used.
  //  "dwFlags"  It is a bit array. The following flags are commonly used (for sake of simplicity hex values are reported):
  //           hasIndex = 0x00000010 (es BYTE array[4] ----> 0 0 0 16 ) MS Media Player requires an index.
  //	       mustUseIndex = 0x00000020 The index must be used because frame sequence is possibly different from the phisical one.
  //	       IsInterleaved = 0x00000100 (es BYTE array[4] ----> 0 0 1 0 ) the AVI file is interleaved.
  //	       capturefile = 0x00010000 (es BYTE array[4] ----> 0 1 0 0 ) The AVI file is used for capturing real-time video. 
  //	       copyrighted = 0x00020000 AVI file contains copyrighted data and software. 
  //	       trustCkType = 0x00000800 (?)  
  //  "dwTotalFrames" Total number of frames.
  //  "dwInitialFrames" Initial frame for interleaved files. Should set to 0 for Noninterleaved files. When creating interleaved files, players wait for "dwInitialFrames" prior to the initial frame of the AVI sequence in this member. 
  //  "dwStreams" Number of streams in the file. The (only one) video stream should be the first
  //  "dwSuggestedBufferSize" Indicates the suggested buffer size for reading the file wich should be large enough to contain the largest data chunk. If too small, the playback software has to reallocate memory dinamically, which will reduce performance (if set to 0 this happens for every frame). For an interleaved file, the buffer size should be large enough to read an entire record, and not just a chunk.
  //  "dwWidth" Width of a frame in pixels (same value for each frame). 
  //  "dwHeight" Height of a frame in pixels (same value for each frame).
  //  "dwReserved[4]"  Reserved and normally set to zero.


  struct StreamHeaderST
  {
    char fccType[4];
    char fccHandler[4];
    char dwFlags[4];	
    UInt16T wPriority;
    UInt16T wLanguage;
    UIntT dwInitialFrames;
    UIntT dwScale;	
    UIntT dwRate;
    UIntT dwStart;
    UIntT dwLength;  
    UIntT dwSuggestedBufferSize;
    UIntT dwQuality;
    UIntT dwSampleSize;
    UInt16T rcFrame[4];
  } ;
  
  //: Struct describing a stream header (strh) 
  //    
  //  "fccType" It's the FOURCC for the type of the data stream. The following values are commonly used:
  //	      'vids' the stream contains video data. 
  //	      'auds' indicates an audio stream. 
  //	      'txts' it's for text data.
  //  "fccHandler" Optionally, it is the FOURCC for a specific data handler(codec) which is to be considered the preferred one for the stream. 
  //  "dwFlags" Another flag(bit) array. The bits in the high-order word of these flags are specific to the type of data contained in the stream. the following standard flags are commonly used here:
  //	      avisf_disabled = 0x00000001 the stream should not be enabled by default.
  //	      palchanges = 0x00010000 for video stream containing palette changes. 
  //  "dwPriority" more important streams have the priority when too few resources are available
  //  "dwInitialFrames" anticipation for audio data to sinchronize with video frames in interleaved AVI. Typically, 0.75 seconds. 
  //  "dwScale" time scale.
  //  "dwRate" dwRate / dwScale == samples/second.
  //  "dwStart" origin of time using the units defined by the dwRate and dwScale. Usually this is zero.
  //  "dwLength" length of this stream. The units are defined by the dwRate and dwScale members of the stream's header(Usually, frames)
  //  "dwSuggestedBufferSize" size of the buffer used to read this stream (should be equal to avih.dwMaxBytesPerSec).
  //  "dwQuality" indicates the quality of the data in the stream (it's a number between 0 and 10,000. Set to 1 to use the default value.
  //  "dwSampleSize" Specifies the size of a single sample of data. This is set to zero if the samples can vary in size. If this number is nonzero, then multiple samples of data can be grouped into a single chunk within the file. If it is zero, each sample of data (such as a video frame) must be in a separate chunk. For video streams, this number is typically zero, although it can be nonzero if all video frames are the same size. For audio streams, this number should be the same as the nBlockAlign.
  //  "rcFrame" Specifies the destination rectangle for a text or video stream within the movie rectangle specified by the dwWidth and dwHeight members of the AVI main header structure. 

  struct PalEntryST
  {
    ByteT peRed; 
    ByteT peGreen; 
    ByteT peBlue; 
    ByteT peFlags; 
  } ;
  
  //: Struct describing a palette entry for a palette change
  //
  //  "peRed" The red intensity value for the palette entry.
  //  "peGreen" The green intensity value for the palette entry.
  //  "peBlue" The blue intensity value for the palette entry.
  //  "peFlags" The alpha intensity value for the palette entry. 

  struct PalChangeST
  {
    ByteT bFirstEntry;
    ByteT bNumEntries;
    UInt16T wFlags;
    PalEntryST *peNew;
  };
  
  //: Struct describing palette change informations
  //  
  //  "bFirstEntry" It's the index of the first palette entry to change.
  //  "bNumEntries" Number of palette entries to change, or zero to change all 256 palette entries.
  //  "wFlags" Reserved.
  //  "peNew" Specifies an array of PalEntryST structures, of size bNumEntries.

  struct aviIndexItemST
  {
    char dwChunkId[4];
    UIntT dwFlags;
    UIntT dwOffset;
    UIntT dwSize;
  } ;
  
  //: Struct describing an index entry
  //
  //  "dwChunkId"  It's the FOURCC that identifies a stream in the AVI file. The FOURCC must have the form 'xxyy' where xx is the stream number and yy is a TWOCC for the contents of the stream: 'db' means uncompressed video frame, 'dc' compressed video frame, 'pc' is a palette change and 'wb' means audio data.
  //  "dwFlags" anothe flag(bit) array, common flags are: 
  //	      keyframe =  0x00000010  data chunk is a key frame.
  //	      list = 0x00000001 the index item refers to a 'rec ' list.
  //	      no_time = 0x00000002 data chunk does not affect the timing of the stream, i.e. for palette changes.
  //  "dwOffset" location of the data as an offset, in bytes, from the start of the 'movi' list; NOTE: sometimes it is given as an offset from the start of the file.
  //  "dwSize" size of the data chunk, in bytes.
  



  //! userlevel=Advanced
  //: AVI stream parser
  
  class AviIStreamC : public GenBinIStreamC
  {
  public:
    AviIStreamC(const IStreamC &nIn, bool verbose = true);
    //: Constructor.
    // From a IStreamC. Set verbose to print on std err more info about the stream
    
#if RAVL_HAVE_INTFILEDESCRIPTORS
    AviIStreamC(int fd, bool verbose = true);
    //: Constructor.
    // From a file descriptor. Set verbose to print on std err more info about the stream
#endif
    
    AviIStreamC(const StringC &nIn, bool verbose = true, bool buffered = true);
    //: Constructor.
    // From a file name. Set verbose to print on std err more info about the stream 
    
    AviIStreamC()
    {}
    //: Default construtor.
    
    bool HasIndex(){return hasIndex;};
    //: returns true if the AVI stream contains a built-in index
    
    bool IsCorrectFormat(){return correct;};
    //: It is false if the stream is not a correct AVI stream 
    
    const AviHeaderST GetAviHeaderInfo() const {return avih;};
    //: returns a read only copy of the AVI main header structure 
    
    bool GetDataStreamInfo(int streamNumber, StreamHeaderST &strh);
    //: loads the Stream Header informations into structure out 
    // Returns FALSE if the stream number doesn't exist

    bool GetVideoStreamFormatInfo(BitmapInfoHST &strf);
    //: loads the Stream Format Header for the video Stream into structure strf 
    // Returns FALSE if the video stream doesn't exist.

    bool GetAudioStreamFormatInfo(WaveFmtXST &strf, int streamNumber);
    //: loads the Stream Format Header for an audio Stream into structure strf 
    // Returns FALSE if the no audio stream is stored into "streamNumber" stream .

    bool GotoDataPos(int frameNumber, int streamNumber);
    //: Goes to the position of the frame "frameNumber" of the stream "streamNumber".
    // Next bytes are (4 bytes size of actual data data)(actual data). 
    // False is returned if frameNumber or streamNumber are not present in the file.

    bool GotoNextDataPos();
    //: If a data chunk of a stream has just been read goes to data of same stream in the next frame.
    // Next bytes are (4 bytes size of actual data data)(actual data). 
    // False is returned if the next frame is not present in the file or.

  protected:
    
    //structural infos
    AviHeaderST avih;//contains the avi main header
    
    //protected variables
    streampos avihPos;//next tokens are header: ('avih')(size)
    streampos moviPos;//next tokens are header: (LIST)(size)('movi')
    streampos firstStrlPos;//next tokens are header: (LIST)(size)('strl')
    streampos indxPos;//next tokens are header: ('idx1')(size)

    //protected methods
    bool LoadStreamInfo();
    //: Initialize the stream and check whether the file format is a correct RIFF AVI

    bool LoadAviHeader();
    //: Loads the main AVI Header
    

  private:
    //private methods
    bool WriteCerr(const StringC info);

    //private variables
    bool hasIndex; //true if the AVI has the HasIndex flag set into the header 
    bool correct;//true if the format is correct (it's the result of methods GetStreamInfo())
    bool verbose; //if true writes into std::cerr info strings
  
    StreamHeaderST strh;//temporary variable to store a stream header

    bool isStrhSet;//true if variable strh has been used at least once
  };
  
  
  //! userlevel=Advanced
  //: AVI stream generator
  
  class AviOStreamC : public GenBinOStreamC
  {
  public:
    
    AviOStreamC(const OStreamC &nOut, DListC<StringC> fccTypes, const bool swapEndian, bool verbose = true); 
    //: Constructor.
    // From a IStreamC. Set swapEndian to swap between littleendian AVI  to bigendian. 
    // Set verbose to print on std err more info about the stream. 
    
#if RAVL_HAVE_INTFILEDESCRIPTORS
    AviOStreamC(int fd, DListC<StringC> fccTypes, const bool swapEndian, bool verbose = true);
    //: Constructor.
    // From a file descriptor.Set swapEndian to swap between littleendian AVI  to bigendian. 
    // Set verbose to print on std err more info about the stream. 
#endif
    
    AviOStreamC(const StringC &nOut, DListC<StringC> fccTypes,const bool swapEndian, bool verbose = true, bool buffered = true);
    //: Constructor.
    // From a file name.Set swapEndian to swap between littleendian AVI  to bigendian. 
    // Set verbose to print on std err more info about the stream. 
    
    AviOStreamC();
    //: Default constructor.
    
    ~AviOStreamC();
    //: Destructor. It writes the AVI index.

    //structural infos
    AviHeaderST avih; 
    //: AVI main header

    DListC<aviIndexItemST> index; //keeps track of position of chunks within the movi list 
    //: access the AVI index 

    //video stream

    bool WriteMainHeader();
    //: writes into the stream the content of structure avih

    bool WriteStreamHeader(int streamNumber, StreamHeaderST &strh);
    //: writes the stream header "strh" for the data stream streamNumber

    bool WriteAudioStreamFormatInfo(WaveFmtXST &strf, int streamNumber);
    //: writes the stream format info "strf" for the audio stream streamNumber. NO CHECKS ARE CARRIED OUT

    bool WriteVideoStreamFormatInfo(BitmapInfoHST &strf, bool isIavs = false, bool isDV = false );
    //: writes the stream format info "strf" for the video stream. NO CHECKS ARE CARRIED OUT

    bool WriteDataChunk(const char * charBuff, const char * type, int length);
    //: Seeks the correct position, writes a data chunk (keeping track of its position into the index structure) and updates the AVI headers.
    // The chunk is (type)(UIntT length)(charBuff). 
    // IT DOES'T CHECK whether the data type is consistent with the one of the available slot or with the header
    
    bool WriteBGRImage(ImageC<ByteBGRValueC> image);
    //: Seeks the correct position, writes a BGR Image (keeping track of its position into the index structure) and updates the AVI headers.
    // The image is writen into a data chunk as follows: ("00db")(UIntT imageSize)(image). 
    // IT DOES'T CHECK whether the data type is consistent with the one of the available slot or with the header

    bool WriteDVFrame(DVFrameC frame);
    //: Seeks the correct position, writes a DV frame (keeping track of its position into the index structure) and updates the AVI headers.
    // The image is writen into a data chunk as follows: ("00db")(UIntT imageSize)(image). 
    // IT DOES'T CHECK whether the data type is consistent with the one of the available slot or with the header

    bool Goto(int frameNumber, int streamNumber);
    //: Goes to a suitable position to write the data chunk  "frameNumber" of the stream "streamNumber" . 
    // False is returned if (frameNumber, streamNumber) are not valid data chunk "coordinates".

  protected:

    bool WriteIndex();
    //: writes the index structure into the stream

    bool UpdateFrameNumber();
    //: updates the number of frames into he AVI headers 
    // ASSERTS that each frame has a data chunk for each stream

    bool WriteEmptyAVI(DListC<StringC> fccTypes);
    //: writes an empty avi file. Returns false if the attempt fails 

    bool Junk(int bytes);
    //: inserts the JUNK chunk with padding 0s until the first multiple of "bytes" bytes. Returns false if the attempt fails 
    
    //protected variables
    
  private:
    //private methods
    bool SetFileSize(int newSize);
    //: writes file size and return to the data chunk
  
    bool SetHdrlSize(int newSize);
    //: writes file size and return to the data chunk

    bool WriteCerr(const StringC info);
    //: prints info on std err on condition defined by verbose variable

    bool Goto(int datachunk);
    //: Goes to the proper position to write a data chunk: (##yy)(size)(data)

    //private variables
    int hdrlSize;//size of hdrl list  
    int moviSize;//size of movi list
    int fileSize;//size of the file
    UIntT indexSize;//size of index chunk

    int dataChunks;//number of data chunks written
    int current; //is the data chunk to write next (NOTE: not necessarily the one after the last one). 1st = 1.

    bool verbose; //if true writes into std::cerr info strings
    bool hasIndex; //true if the index has been written yet and the header updated

    //positions of data to access after constructor is called
    std::streampos end;//end of the stream

    std::streampos moviSizePos;//position of the movi list size slot
    std::streampos avihPos;//next tokens are header: ('avih')(size)
    std::streampos moviPos;//next tokens are header: (LIST)(size)('movi')
    std::streampos indxPos;//next tokens are header: ('idx1')(size)
    DListC<std::streampos> StrlPosList;//next tokens are header: (LIST)(size)('strl')
    DListC<std::streampos> frameNumberPos;//each position in the stream containing the number of frames

  };
}

#endif
