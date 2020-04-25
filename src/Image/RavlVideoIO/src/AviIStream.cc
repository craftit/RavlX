// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlVideoIO
//! file="Ravl/Image/VideoIO/AviIStream.cc"

#include "Ravl/Image/AviStream.hh"

#define DODEBUG 0

#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlImageN{
  
  AviIStreamC::AviIStreamC(const IStreamC &nIn, bool verb) 
    : GenBinIStreamC(nIn)
  {
    verbose = verb;
    correct = LoadStreamInfo();
  }//END OF AviIStreamC::AviIStreamC(const IStreamC &nIn, bool verbose = true)
  
  
#if RAVL_HAVE_INTFILEDESCRIPTORS
  AviIStreamC::AviIStreamC(int fd, bool verb) 
    : GenBinIStreamC(fd)
  {
    verbose = verb;
    correct = LoadStreamInfo();
  }//END OF AviIStreamC::AviIStreamC(int fd, bool verbose = true)  
#endif
  
  AviIStreamC::AviIStreamC(const StringC &nIn, bool verb, bool buffered)
    : GenBinIStreamC(nIn,false,buffered)
  {
    verbose = verb;
    correct = LoadStreamInfo();
  } //END OF AviIStreamC::AviIStreamC(const StringC &nIn, bool verb, bool buffered) 
  
  bool AviIStreamC::LoadStreamInfo()
  {
    char token[4]; //a FOURCC
    UIntT size; //unsigned 32bits Integer 
  
    Stream().read(token,4);
    if( (token[0] != 'R') || (token[1] != 'I') || (token[2] != 'F') ) WriteCerr( "error: not a RIFF file!\n");
    else if(token[3] == 'F') 
      {
	WriteCerr("little endian RIFF file\n(RIFF)(");
	toSwap = true;
      } else if(token[3] == 'X') 
	{
	  WriteCerr("big endian RIFF file\n(RIFX)(");      
	  toSwap = false;
	} else
	  {
	    WriteCerr("error: not a RIFF file!\n");
	    return false;
	  }

    *this >> size; //size of the file excluding FORM and hdrl (!UNRELIABLE)
    Stream().read(token,4);

    if( (token[0] != 'A') || (token[1] != 'V') || (token[2] != 'I') || (token[3] != ' ') ) 
      {
	WriteCerr("error: not an AVI file!\n");
	return false;
      }
    WriteCerr(StringC((int)size) + ")(AVI )\n");
    WriteCerr("file size: " + StringC((int)size) + "\n");

    //searchs the hdrl list
    Stream().read(token,4); //LIST
    *this >> size; //size of the hdrl list 
    moviPos = Tell() + (streampos)size; // position of the movi list

    Stream().read(token,4); //maybe hdrl

    if( (token[0] != 'h') || (token[1] != 'd') || (token[2] != 'r') || (token[3] != 'l') ) 
      {
	WriteCerr("error: cannot find compulsory 'hdrl' list!\n");
	return false;
      }
    WriteCerr("(LIST)" + StringC((int)size) + "(hdrl)\n");

    //checks the avih chunk
    avihPos = Tell();   
    if(LoadAviHeader()) hasIndex = (avih.dwFlags[0]==16 || avih.dwFlags[0]==32);
    else return false;

    for(int i=0;i<(int)avih.dwStreams;i++)
      {
	streampos  j= Tell();
	//searchs the  strl list
	Stream().read(token,4);//LIST 
	if( (token[0] != 'L') || (token[1] != 'I') || (token[2] != 'S') || (token[3] != 'T') ) 
	  {
	    WriteCerr("error: cannot find 'strl' list! for stream " + StringC(i) + "\n");
	    return false;
	  }
	
	*this >> size;
	Stream().read(token,4);
	if( (token[0] != 's') || (token[1] != 't') || (token[2] != 'r') || (token[3] != 'l') ) 
	  {
	    WriteCerr("error: cannot find 'strl' list! for stream " + StringC(i) + "\n");
	    return false;
	  } else if(i==0) firstStrlPos = j;
	
	Seek(Tell()+(streampos)(size-4));//skips the strl
      }
    
    //check the movi list position
    Stream().read(token,4);
    if( (token[0] == 'J') && (token[1] == 'U') && (token[2] == 'N') && (token[3] == 'K') ) 
      {
	*this >> size;
	Seek(Tell() + (streampos)size);
	Stream().read(token,4);
      } 
    
    if( (token[0] == 'L') && (token[1] == 'I') && (token[2] == 'S') && (token[3] == 'T') ) 
      {
	*this >> size;
	Stream().read(token,4);
	if( (token[0] != 'm') || (token[1] != 'o') || (token[2] != 'v') || (token[3] != 'i') ) 
	  {
	    WriteCerr("error: cannot find compulsory 'movi' list!\n");
	    return false;
	  }
	moviPos = Tell() - (streampos)12; 
      } else	
	{
	  WriteCerr("error: cannot find compulsory 'movi' list!\n");
	  return false;
	}

    //searchs the index chunk IF PRESENT
    if(hasIndex)
      {
	Seek(moviPos);
	Stream().read(token,4);
	*this >> size;
	Seek(Tell() + (streampos)size);
	
	Stream().read(token,4);
	if( (token[0] == 'i') && (token[1] == 'd') && (token[2] == 'x') && (token[3] == '1') ) indxPos = Tell() - (streampos)4;
	else WriteCerr("ERROR: avi file has a corrupted header.  NO INDEX found\n");    
	WriteCerr("(idx1)(" + StringC((int)size) + ")\n");
      } else  WriteCerr("avi file has no index\n");
    
    return true;
  }//END OF bool AviIStreamC::LoadStreamInfo()
  
  bool AviIStreamC::LoadAviHeader()
  {
    char token[4]; //a FOURCC
    UIntT size; //unsigned 32bits Integer 
    int i; //a simple counter
   
    Seek(avihPos);

    Stream().read(token,4);
    if( (token[0] != 'a') || (token[1] != 'v') || (token[2] != 'i') || (token[3] != 'h') ) 
      {
	WriteCerr("error: cannot find compulsory 'avih' chunk!\n");
	return false;
      }

    //loads the avi header (chunk)
    *this >> size;
    WriteCerr("(avih)("+StringC((int)size)+")\n");
    *this >> avih.dwMicroSecPerFrame;
    WriteCerr("dwMicroSecPerFrame: " + StringC((int)avih.dwMicroSecPerFrame) + "\n");
    *this >> avih.dwMaxBytesPerSec;
    WriteCerr("dwMaxBytesPerSec:" + StringC((int)avih.dwMaxBytesPerSec) + "\n");
    *this >> avih.dwPaddingGranularity;
    WriteCerr("dwPaddingGranularity:"+ StringC((int)avih.dwPaddingGranularity) + "\n");
    
    for(i=0;i<4;i++)  
      {
	*this >> avih.dwFlags[i];
	WriteCerr("dwFlags["+ StringC((int) i) + "]:"+ StringC((int)avih.dwFlags[i]) + "\n");
      }
    *this >> avih.dwTotalFrames;
    WriteCerr("dwTotalFrames:" + StringC((int)avih.dwTotalFrames) + "\n");
    *this >> avih.dwInitialFrames;
    WriteCerr("dwInitialFrames:" + StringC((int)avih.dwInitialFrames) + "\n");
    *this >> avih.dwStreams;
    WriteCerr("dwStreams:" + StringC((int)avih.dwStreams) + "\n");
    *this >> avih.dwSuggestedBufferSize;
    WriteCerr("dwSuggestedBufferSize:" + StringC((int)avih.dwSuggestedBufferSize) + "\n");
    *this >> avih.dwWidth;
    WriteCerr("dwWidth:" + StringC((int)avih.dwWidth) + "\n");
    *this >> avih.dwHeight;
    WriteCerr("dwHeight:" + StringC((int)avih.dwHeight) + "\n");
    for(i=0;i<4;i++)  
      {
	*this >> avih.dwReserved[i];
	WriteCerr("dwReserved[" + StringC((int) i) + "]:" + StringC((int)avih.dwReserved[i]) + "\n");
      }
    
    return true;
  }//END OF bool AviIStreamC::LoadHeader()

  bool AviIStreamC::GetDataStreamInfo(int streamNumber, StreamHeaderST &outStrh)
  {
    if(streamNumber >= (int)avih.dwStreams) 
      {
	WriteCerr("data stream " + StringC((int)streamNumber) + " is not present in this AVI stream\n");
	return false;
      }
    
    int i; //simple counter
    char token[4]; 
    UIntT size; //file/token size excluding the Header

    //searchs the exact position in the stream
    Seek(firstStrlPos);
    for(i=0;(i<streamNumber);i++)
      {
	Stream().read(token,4);
	*this >> size;
	Seek(Tell()+(streampos)size);
      }
    Stream().read(token,4);//LIST
    *this >> size;//list size
    WriteCerr("(LIST)("+StringC((int)size)+")");
    Stream().read(token,4);//strl
    Stream().read(token,4);//strh
    *this >> size;//chunk size
    WriteCerr("(strl)\n(strh)("+StringC((int)size)+")\n");

    //loads data
    Stream().read(strh.fccType,4);
    for(i=0;i<4;i++) WriteCerr("fccType["+ StringC((int) i) + "]:"+ strh.fccType[i] + "\n");
    Stream().read(strh.fccHandler,4);
    for(i=0;i<4;i++) WriteCerr("fccHandler["+ StringC((int) i) + "]:"+ strh.fccHandler[i] + "\n");
    Stream().read(strh.dwFlags,4);
    for(i=0;i<4;i++) WriteCerr("dwFlags["+ StringC((int) i) + "]:"+ StringC((int)strh.dwFlags[i]) + "\n");
    *this >> strh.wPriority;
    WriteCerr("wPriority:" + StringC((int)strh.wPriority) + "\n");
    *this >> strh.wLanguage;
    WriteCerr("wLanguage:" + StringC((int)strh.wLanguage) + "\n");
    *this >> strh.dwInitialFrames;
    WriteCerr("dwInitialFrames:" + StringC((int)strh.dwInitialFrames) + "\n");
    *this >> strh.dwScale;
    WriteCerr("dwScale:" + StringC((int)strh.dwScale) + "\n");
    *this >> strh.dwRate;
    WriteCerr("dwRate:" + StringC((int)strh.dwRate) + "\n");
    *this >> strh.dwStart;
    WriteCerr("dwStart:" + StringC((int)strh.dwStart) + "\n");
    *this >> strh.dwLength;
    WriteCerr("dwLength:" + StringC((int)strh.dwLength) + "\n");
    *this >> strh.dwSuggestedBufferSize;
    WriteCerr("dwSuggestedBufferSize:" + StringC((int)strh.dwSuggestedBufferSize) + "\n");
    *this >> strh.dwQuality;
    WriteCerr("dwQuality:" + StringC((int)strh.dwQuality) + "\n");
    *this >> strh.dwSampleSize;
    WriteCerr("dwSampleSize:" + StringC((int)strh.dwSampleSize) + "\n");
    for(i=0;i<4;i++) *this >> strh.rcFrame[i];
    for(i=0;i<4;i++) WriteCerr("rcFrame["+ StringC((int) i) + "]:"+ StringC((int)strh.rcFrame[i]) + "\n");

    if(!isStrhSet) isStrhSet = true;
    outStrh = strh;
    return true;
  }//END OF bool AviIStreamC::GetDataStreamInfo(int streamNumber, StreamHeaderST &out)
  
  bool AviIStreamC::GetVideoStreamFormatInfo(BitmapInfoHST &strf)
  {
    char token[4]; 
    UIntT size; //file/token size excluding the Header
    
    //checks weather the video stream exists or not
    bool isVids, isIavs;
    
    if(!isStrhSet) 
      {
	StreamHeaderST aStrh;
	GetDataStreamInfo(0, aStrh);
      }

    isVids = (strh.fccType[0] == 'v') && (strh.fccType[1] == 'i') && (strh.fccType[2] == 'd') && (strh.fccType[3] == 's'); 
    isIavs = (strh.fccType[0] == 'i') && (strh.fccType[1] == 'a') && (strh.fccType[2] == 'v') && (strh.fccType[3] == 's'); 
    if( !isVids && !isIavs ) 
      {
	return false;
	WriteCerr("No video stream in this AVI stream\n");
      }
    
    //searchs the exact position in the stream
    Stream().read(token,4);//strf
    *this >> size;//chunk size
    WriteCerr("(strf)("+StringC((int)size)+")\n");

    //loads data

    if(isIavs)
      {
	*this >> strf.dvinfo.dwDVAAuxSrc;
	WriteCerr("dwDVAAuxSrc:" + StringC((int)strf.dvinfo.dwDVAAuxSrc) + "\n");
	*this >> strf.dvinfo.dwDVAAuxCtl;
	WriteCerr("dwDVAAuxCtl:" + StringC((int)strf.dvinfo.dwDVAAuxCtl) + "\n");
	*this >> strf.dvinfo.dwDVAAuxSrc1;
	WriteCerr("dwDVAAuxSrc1:" + StringC((int)strf.dvinfo.dwDVAAuxSrc1) + "\n");
	*this >> strf.dvinfo.dwDVAAuxCtl1;
	WriteCerr("dwDVAAuxCtl1:" + StringC((int)strf.dvinfo.dwDVAAuxCtl1) + "\n");
	*this >> strf.dvinfo.dwDVVAuxSrc;
	WriteCerr("dwDVVAuxSrc:" + StringC((int)strf.dvinfo.dwDVVAuxSrc) + "\n");
	*this >> strf.dvinfo.dwDVVAuxCtl;
	WriteCerr("dwDVVAuxCtl:" + StringC((int)strf.dvinfo.dwDVVAuxCtl) + "\n");
	*this >> strf.dvinfo.dwDVReserved[0];
	WriteCerr("dwDVReserved[0]:" + StringC((int)strf.dvinfo.dwDVReserved[0]) + "\n");
	*this >> strf.dvinfo.dwDVReserved[1];
	WriteCerr("dwDVReserved[1]:" + StringC((int)strf.dvinfo.dwDVReserved[1]) + "\n");
      }
    else
      {
	*this >> strf.biSize;
	WriteCerr("biSize:" + StringC((int)strf.biSize) + "\n");
	*this >> strf.biWidth;
	WriteCerr("biWidth:" + StringC((int)strf.biWidth) + "\n");
	*this >> strf.biHeight;
	WriteCerr("biHeight:" + StringC((int)strf.biHeight) + "\n");
	*this >> strf.biPlanes;
	WriteCerr("biPlanes:" + StringC((int)strf.biPlanes) + "\n");
	*this >> strf.biBitCount;
	WriteCerr("biBitCount:" + StringC((int)strf.biBitCount) + "\n");
	Stream().read(strf.biCompression,4);
	for(int i=0;i<4;i++) WriteCerr("biCompression["+ StringC((int) i) + "]:"+ strf.biCompression[i] + "\n");
	*this >> strf.biSizeImage;
	WriteCerr("biSizeImage:" + StringC((int)strf.biSizeImage) + "\n");
	*this >> strf.biXPelsPerMeter;
	WriteCerr("biXPelsPerMeter:" + StringC((int)strf.biXPelsPerMeter) + "\n");
	*this >> strf.biYPelsPerMeter;
	WriteCerr("biYPelsPerMeter:" + StringC((int)strf.biYPelsPerMeter) + "\n");
	*this >> strf.biClrUsed;
	WriteCerr("biClrUsed:" + StringC((int)strf.biClrUsed) + "\n");
	*this >> strf.biClrImportant;
	WriteCerr("biClrImportant:" + StringC((int)strf.biClrImportant) + "\n");
	if(strf.biSize == 72)
	  {
	    *this >> strf.dvinfo.dwDVAAuxSrc;
	    WriteCerr("dwDVAAuxSrc:" + StringC((int)strf.dvinfo.dwDVAAuxSrc) + "\n");
	    *this >> strf.dvinfo.dwDVAAuxCtl;
	    WriteCerr("dwDVAAuxCtl:" + StringC((int)strf.dvinfo.dwDVAAuxCtl) + "\n");
	    *this >> strf.dvinfo.dwDVAAuxSrc1;
	    WriteCerr("dwDVAAuxSrc1:" + StringC((int)strf.dvinfo.dwDVAAuxSrc1) + "\n");
	    *this >> strf.dvinfo.dwDVAAuxCtl1;
	    WriteCerr("dwDVAAuxCtl1:" + StringC((int)strf.dvinfo.dwDVAAuxCtl1) + "\n");
	    *this >> strf.dvinfo.dwDVVAuxSrc;
	    WriteCerr("dwDVVAuxSrc:" + StringC((int)strf.dvinfo.dwDVVAuxSrc) + "\n");
	    *this >> strf.dvinfo.dwDVVAuxCtl;
	    WriteCerr("dwDVVAuxCtl:" + StringC((int)strf.dvinfo.dwDVVAuxCtl) + "\n");
	    *this >> strf.dvinfo.dwDVReserved[0];
	    WriteCerr("dwDVReserved[0]:" + StringC((int)strf.dvinfo.dwDVReserved[0]) + "\n");
	    *this >> strf.dvinfo.dwDVReserved[1];
	    WriteCerr("dwDVReserved[1]:" + StringC((int)strf.dvinfo.dwDVReserved[1]) + "\n");
	  }
      }
    return true;
  }//END OF bool AviIStreamC::GetVideoStreamFormatInfo(BitmapInfoHST &strf) 

  bool AviIStreamC::GetAudioStreamFormatInfo(WaveFmtXST &strf, int streamNumber)
  {
    if(streamNumber >= (int)avih.dwStreams) 
      {
	WriteCerr("data stream " + StringC((int)streamNumber) +" is not present in this AVI stream\n");
	return false;
      }
    
    char token[4]; 
    UIntT size; //file/token size excluding the Header
    
    //checks weather the video stream exists or not
    bool isAuds;

    if(!isStrhSet) 
      {
	StreamHeaderST aStrh;
	GetDataStreamInfo(0, aStrh);
      }

    isAuds = (strh.fccType[0] == 'a') && (strh.fccType[1] == 'u') && (strh.fccType[2] == 'd') && (strh.fccType[3] == 's'); 
    if(!isAuds) 
      {
	WriteCerr("stream " + StringC((int)streamNumber) +" is not an audio stream in this AVI stream\n");
	return false;
      }
    
    //searchs the exact position in the stream
    Stream().read(token,4);//strf
    *this >> size;//chunk size
    WriteCerr("(strf)("+StringC((int)size)+")\n");


    *this >> strf.wFormatTag;      
    WriteCerr("wFormatTag:" + StringC((int)strf.wFormatTag) + "\n");
    *this >> strf.nChannels;       
    WriteCerr("nChannels:" + StringC((int)strf.nChannels) + "\n");
    *this >> strf.nSamplesPerSec;  
    WriteCerr("nSamplesPerSec:" + StringC((int)strf.nSamplesPerSec) + "\n");
    *this >> strf.nAvgBytesPerSec; 
    WriteCerr("nAvgBytesPerSec:" + StringC((int)strf.nAvgBytesPerSec) + "\n");
    *this >> strf.nBlockAlign;     
    WriteCerr("nBlockAlign:" + StringC((int)strf.nBlockAlign) + "\n");
    *this >> strf.wBitsPerSample;  
    WriteCerr("wBitsPerSample:" + StringC((int)strf.wBitsPerSample) + "\n");
    *this >> strf.cbSize;          
    WriteCerr("cbSize:" + StringC((int)strf.cbSize) + "\n");
  
    return true;
  }

  bool AviIStreamC::GotoDataPos(int frameNumber, int streamNumber)
  {
    if( (frameNumber>= (int)avih.dwTotalFrames) || (streamNumber>=(int)avih.dwStreams)) return false;
      
    int i,j; //simple counters
    char token[4]; 
    UIntT size; //file/token size excluding the Header

    //skips the movi header 
    Seek(moviPos);

    Stream().read(token,4);//LIST
    *this >> size;//movi list size 
    Stream().read(token,4);//FOURCC 'movi'
    if( (token[0] != 'm') || (token[1] != 'o') || (token[2] != 'v') || (token[3] != 'i') ) 
      {
	WriteCerr("error: cannot find compulsory movi list!\n");
	return false;
      }

    //skips the frames before "frameNumber"
    Stream().read(token,4);
    *this >> size;//size 
    for(i=0;i<frameNumber;i++)  
      {
	//skips the 'rec ' header if present
	if( (token[0] == 'L') && (token[1] == 'I') && (token[2] == 'S') && (token[3] == 'T') ) 
	  {
	    Seek(Tell()+(streampos)size);
	    Stream().read(token,4);
	    *this >> size;//size 
	    continue;
	  }

	for(j=0;j<(int)avih.dwStreams;j++)  
	  {
	    Seek(Tell()+(streampos)size);
	    Stream().read(token,4);
	    *this >> size;
	  }
      }

    //skips the 'rec ' header if present
    if( (token[0] == 'L') && (token[1] == 'I') && (token[2] == 'S') && (token[3] == 'T') ) Stream().read(token,4);//skips the FOURCC 'rec ' 

    for(j=0;j<=(int)avih.dwStreams;j++)  
      {
	int sn = 0;
	sn += ((int)(token[1]-'0'));
	sn += (((int)(token[0]-'0'))*10);
	if(sn == streamNumber)
	  {
	    WriteCerr("stream: " + StringC((int)sn) + " stream type: " + StringC(token[2]) + StringC(token[3]) + "\n");
	    Seek(Tell()-(streampos)4);
	    return true;
	  }
	else
	  {
	    Seek(Tell()+(streampos)size);
	    Stream().read(token,4);
	    *this >> size;
	  }
	
	
      }
    return false;
  }//END OF  bool AviIStreamC::GotoPos(int frameNumber, int streamNumber)

  bool AviIStreamC::GotoNextDataPos()
  {
    char token[4]; 
    UIntT size; //file/token size excluding the Header
    int i;//simple counter
 
    for(i=1;i<(int)avih.dwStreams;i++)  
      {
	Stream().read(token,4);

	if( (token[0] == 'L') && (token[1] == 'I') && (token[2] == 'S') && (token[3] == 'T') ) 
	  {
	    *this >> size;//rec list size
	    Stream().read(token,4);//skips the FOURCC 'rec '
	    Stream().read(token,4);//reads next token
	  }

	if( !(((token[2] == 'd') && (token[3] == 'b')) ||
	      ((token[2] == 'd') && (token[3] == 'c')) ||
	      ((token[2] == 'w') && (token[3] == 'b'))) )	  
	  {
	    WriteCerr("error! Current position not suitable for function GotoNextDataPos()\n");
	    return false;
	  }
	
	*this >> size;//chunk size
	Seek(Tell()+(streampos)size);
	Stream().read(token,4);
      }

    return true;
  }//END OF  bool AviIStreamC::GotoNextFramePos()

  bool AviIStreamC::WriteCerr(const StringC info)
  {
    if(verbose) 
      {
	cerr << info;
	return true;
      }
    return true;
  }//END OF AviIStreamC::writeCerr(StringC info)

}

 
