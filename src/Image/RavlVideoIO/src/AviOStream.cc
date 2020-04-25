// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlVideoIO
//! file="Ravl/Image/VideoIO/AviOStream.cc"

#include "Ravl/Image/AviStream.hh"
#include "Ravl/SArray1dIter.hh"

#define DODEBUG 0

#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif
    
namespace RavlImageN{
  
  AviOStreamC::AviOStreamC(const OStreamC &nOut, DListC<StringC> fccTypes , const bool swapEndian, bool verb) 
    : GenBinOStreamC(nOut,swapEndian)
  {
    verbose = verb;
    toSwap = swapEndian;
    WriteEmptyAVI(fccTypes);
  }//END OF AviOStreamC::AviOStreamC(const OStreamC &nOut)
  
  
#if RAVL_HAVE_INTFILEDESCRIPTORS
  AviOStreamC::AviOStreamC(int fd, DListC<StringC> fccTypes, const bool swapEndian, bool verb) 
    : GenBinOStreamC(fd,swapEndian)
  {
    verbose = verb;
    toSwap = swapEndian;
    WriteEmptyAVI(fccTypes);
  }//END OF AviOStreamC::AviOStreamC(int fd)  
#endif
  
  AviOStreamC::AviOStreamC(const StringC &nOut, DListC<StringC> fccTypes, const bool swapEndian, bool verb, bool buffered) 
    : GenBinOStreamC(nOut,swapEndian,buffered)  
  {
    verbose = verb;
    toSwap = swapEndian;
    WriteEmptyAVI(fccTypes);
  } //END OF AviOStreamC::AviOStreamC(const StringC &nOut,bool buffered) 

  AviOStreamC::AviOStreamC()
  {}
  //: Default constructor.

  AviOStreamC::~AviOStreamC()
  {
    Seek(end);
    WriteIndex();
  } 
  
  bool AviOStreamC::WriteEmptyAVI(DListC<StringC> fccTypes)
  {
    DListC<int> strfSizes;//strf size list
    hdrlSize = 68 ;//minimum: (hdrl)(avih)(size)(56bytes)
    
    int c=0;
    for(DLIterC<StringC> it(fccTypes);it;it++) 
      {
	hdrlSize += 84;
	if(*it == "iavs") 
	  {
	    if(c!=0) WriteCerr("Video stream, when present, should be the first and only one\n");
	    strfSizes += 32;
	    hdrlSize += 32;
	    frameNumberPos += (streampos)140;
	  }
	else if(*it == "vids")
	  {
	    if(c!=0) WriteCerr("Video stream, when present, should be the first and only one\n");
	    strfSizes += 40;
	    hdrlSize += 40;
	    frameNumberPos += (streampos)140;
	  }
	else if(*it == "auds")
	  {
	    strfSizes += 18;
	    hdrlSize += 18;
	  }
	else 
	  {
	    WriteCerr("ERROR: cannot find format for stream " + *it + "\n");
	    return false;
	  }
	c++;
      }
    
    int count=0;
    int temp=-1;
    while(temp<0)
      {
	count++ ;
	temp = count*2048-(20+hdrlSize);
      }
    fileSize=count*2048 - 8; //minimum 2060: (RIFF)(size)(AVI )(LIST)(size)(hdrl)(avih)(size)(56bytes)(JUNK)(0s to 2048 - 16)(LIST)(size)(movi)
    
    if(toSwap) *this << 'R' << 'I' << 'F' << 'F';
    else *this << 'R' << 'I' << 'F' << 'X';
    *this << (UIntT)fileSize << 'A' << 'V' << 'I' << ' ';//RIFF FORM
    *this << 'L' << 'I' << 'S' << 'T' << (UIntT)hdrlSize << 'h' << 'd' << 'r' << 'l';//hrdl list

    avihPos = Tell();
    frameNumberPos += (avihPos + (streampos)24);

    avih.dwStreams=strfSizes.Size();
    *this << 'a' << 'v' << 'i' << 'h' << (UIntT)56 ;//avih chunk
    int i;
    for(i=0;i<6;i++) *this << (UIntT)0;
    *this << (UIntT)avih.dwStreams;
    for(i=7;i<14;i++) *this << (UIntT)0;

    for(DLIterC<int> its(strfSizes);its;its++) 
      {
	StrlPosList += Tell();
	*this << 'L' << 'I' << 'S' << 'T' << (UIntT)(*its+76) << 's' << 't' << 'r' << 'l';//strl list
	//strh
	*this << 's' << 't' << 'r' << 'h' << (UIntT)56 ;//strh chunk
	for(i=0;i<14;i++) *this << (UIntT)0;
	//strf
	*this << 's' << 't' << 'r' << 'f' << (UIntT)*its ;//strf chunk
	for(i=0;i<*its;i++) *this << (char)0;
      }
    
    Junk(2048);//inserts a JUNK chunk

    moviPos=Tell();
    moviSize = 4;

    *this << 'L' << 'I' << 'S' << 'T' << (UIntT)moviSize << 'm' << 'o' << 'v' << 'i';//movi list

    end = moviPos + (streampos)12;
    dataChunks=0;
    current = 0; //0 is the fisrt chunk: (frame 0, stream 0)

    Goto(current); // READY to write data chunk

    return true;
  }//END OF bool AviOStreamC::WriteEmptyAVI(DListC<StringC> fccTypes)
    
  bool AviOStreamC::Junk(int bytes)
  {
    int count=1;
    int pos=(int)Tell()+8;
    int temp=-1;
    while(temp<0)
      {
	temp = count*bytes-pos;
	count++ ;
      }
    temp -= 12;
    *this << 'J' << 'U' << 'N' << 'K' << (UIntT)temp;
    for(count=0;count<temp;count++) *this << (char)0;
    return true;
  }//END OF bool AviOStreamC::Junk(int bytes)
  
  bool AviOStreamC::WriteMainHeader()
  {
    Seek(avihPos+(streampos)8);
    
    *this << avih.dwMicroSecPerFrame;
    *this << avih.dwMaxBytesPerSec;
    *this << avih.dwPaddingGranularity;
    for(int i=0;i<4;i++) *this << (char)avih.dwFlags[i];		
    *this << avih.dwTotalFrames;		
    *this << avih.dwInitialFrames;
    *this << avih.dwStreams;
    *this << avih.dwSuggestedBufferSize;
    
    *this << avih.dwWidth;
    *this << avih.dwHeight;
    
    *this << avih.dwReserved[0] << avih.dwReserved[1] << avih.dwReserved[2] << avih.dwReserved[3];

    Goto(current); // READY to write data chunk
    
    return true; 
  }//END OF bool AviOStreamC::WriteMainHeader()

  bool AviOStreamC::WriteStreamHeader(int streamNumber, StreamHeaderST &strh)
  {
    if(streamNumber >= (int)avih.dwStreams) 
      {
	WriteCerr("data stream " +StringC((int)streamNumber) + " is not present in this AVI stream\n");
	return false;
      }
    
    //searchs the exact position in the stream
    Seek(StrlPosList.Nth(streamNumber)+(streampos)20);

    //loads data
    *this << strh.fccType[0] << strh.fccType[1] << strh.fccType[2] << strh.fccType[3];
    *this << strh.fccHandler[0] << strh.fccHandler[1] << strh.fccHandler[2] << strh.fccHandler[3];
    *this << strh.dwFlags[0] << strh.dwFlags[1] << strh.dwFlags[2] << strh.dwFlags[3];
    *this << strh.wPriority;
    *this << strh.wLanguage;
    *this << strh.dwInitialFrames;
    *this << strh.dwScale;
    *this << strh.dwRate;
    *this << strh.dwStart;
    *this << strh.dwLength;
    *this << strh.dwSuggestedBufferSize;
    *this << strh.dwQuality;
    *this << strh.dwSampleSize;
    for(int i=0;i<4;i++) *this << (UInt16T)strh.rcFrame[i];

    Goto(current); // READY to write data chunk
    
    return true;
 }//END OF bool AviOStreamC::WriteStreamHeader(int streamNumber, StreamHeaderST &strh)

  bool AviOStreamC::WriteVideoStreamFormatInfo(BitmapInfoHST &strf, bool isIavs, bool isDV)
  {
    //searchs the exact position in the stream
    Seek(StrlPosList.Nth(0)+(streampos)84);
    
    if(isIavs && isDV)
      {
	*this <<  strf.dvinfo.dwDVAAuxSrc;
	*this <<  strf.dvinfo.dwDVAAuxCtl;
	*this <<  strf.dvinfo.dwDVAAuxSrc1;
	*this <<  strf.dvinfo.dwDVAAuxCtl1;
	*this <<  strf.dvinfo.dwDVVAuxSrc;
	*this <<  strf.dvinfo.dwDVVAuxCtl;
	*this <<  strf.dvinfo.dwDVReserved[0];
	*this <<  strf.dvinfo.dwDVReserved[1];
      }
    else if(!isIavs)
      {
	*this << strf.biSize;
	*this << strf.biWidth;
	*this << strf.biHeight;
	*this << strf.biPlanes;
	*this << strf.biBitCount;
	*this << strf.biCompression[0] << strf.biCompression[1] << strf.biCompression[2] << strf.biCompression[3];
	*this << strf.biSizeImage;
	*this << strf.biXPelsPerMeter;
	*this << strf.biYPelsPerMeter;
	*this << strf.biClrUsed;
	*this << strf.biClrImportant;
	if(isDV)
	  {
	    *this << strf.dvinfo.dwDVAAuxSrc;
	    *this << strf.dvinfo.dwDVAAuxCtl;
	    *this << strf.dvinfo.dwDVAAuxSrc1;
	    *this << strf.dvinfo.dwDVAAuxCtl1;
	    *this << strf.dvinfo.dwDVVAuxSrc;
	    *this << strf.dvinfo.dwDVVAuxCtl;
	    *this << strf.dvinfo.dwDVReserved[0];
	    *this << strf.dvinfo.dwDVReserved[1];
	  }
      }
    else WriteCerr("ERROR: unable to write (iavs & non DV) video format\n");

    Goto(current); // READY to write data chunk

    return true;
  }//END OF bool AviOStreamC::WriteVideoStreamFormatInfo(BitmapInfoHST &strf, bool isIavs, bool isDV)

  bool AviOStreamC::WriteAudioStreamFormatInfo(WaveFmtXST &strf, int streamNumber)
  {
    //searchs the exact position in the stream
    Seek(StrlPosList.Nth(streamNumber)+(streampos)84);

    *this << strf.wFormatTag;      
    *this << strf.nChannels;       
    *this << strf.nSamplesPerSec;  
    *this << strf.nAvgBytesPerSec; 
    *this << strf.nBlockAlign;     
    *this << strf.wBitsPerSample;  
    *this << strf.cbSize;

    Goto(current); // READY to write data chunk

    return true;     
  }//END OF bool AviOStreamC::WriteAudioStreamFormatInfo(WaveFmtXST &strf, int streamNumber)

  bool AviOStreamC::SetFileSize(int newSize)
  {
    Seek((streampos)4);
    *this << (UIntT)newSize; 

    Goto(current); // READY to write data chunk

    return true;     
  }//END OF bool AviOStreamC::SetFileSize(int newSize)
  
  bool AviOStreamC::SetHdrlSize(int newSize)
  {
    Seek((streampos)16);
    *this << (UIntT)newSize; 

    Goto(current); // READY to write data chunk

    return true;     
  }//END OF bool AviOStreamC::SetHdrlSize(int newSize)
  
  bool AviOStreamC::WriteDataChunk(const char * charBuff, const char * type, int length)
  {
    int i;
    for(i=0;i<4;i++) *this << type[i];     
    *this << (UIntT)length;
    for(i=0;i<length;i++) *this << charBuff[i];     
    
    //creates a new index entry
    aviIndexItemST newIndexItem;
    for(i=0;i<4;i++) newIndexItem.dwChunkId[i]=type[i];
    newIndexItem.dwFlags = (UIntT)16;
    newIndexItem.dwOffset = (UIntT)(end - moviPos - (streampos)8);
    newIndexItem.dwSize = (UIntT)length;
    
    index += newIndexItem;
    
    //position for a new chunk
    if(current == dataChunks)
      {
	end += (streampos)(length +8);
	current++;
	dataChunks++;
	UpdateFrameNumber();
      }
    else current++;

    
    fileSize += (length + 8);
    moviSize += (length + 8);
    
    SetFileSize(fileSize);
    
    Seek(moviPos + (streampos)4);
    *this << (UIntT)moviSize;
    
    Goto(current); // READY to write data chunk

    return true;
  }//END OF bool AviOStreamC::WriteDataChunk(const char * charBuff, const char * type, int length)
  
  bool AviOStreamC::WriteBGRImage(ImageC<ByteBGRValueC> image)
  {
    int length;
    char type[] = "00db";

    int i;
    for(i=0;i<4;i++) *this << type[i];     
    streampos p = Tell();
    *this << (UIntT)0;
    this->Stream().write((char *)(&image[image.Frame().Origin()]),3 * image.Frame().Area());     

    length = (int)(Tell() - end) - 8;
    Seek(p);
    *this << (UIntT)length;
    
    //creates a new index entry
    aviIndexItemST newIndexItem;
    for(i=0;i<4;i++) newIndexItem.dwChunkId[i]=type[i];
    newIndexItem.dwFlags = (UIntT)16;
    newIndexItem.dwOffset = (UIntT)(end - moviPos - (streampos)8);
    newIndexItem.dwSize = (UIntT)length;

    index += newIndexItem;

    //position for a new chunk
    if(current == dataChunks )
      {
	end += (streampos)(length +8);
	dataChunks++;
	current++;
	UpdateFrameNumber();
      }
    else current++;
    
    fileSize += (length + 8);
    moviSize += (length + 8);

    SetFileSize(fileSize);
    
    Seek(moviPos + (streampos)4);
    *this << (UIntT)moviSize;

    Goto(current); // READY to write data chunk

    return true;
  }//END OF bool AviOStreamC::WriteBGRImage(ImageC<ByteBGRValueC> image)

  bool AviOStreamC::WriteDVFrame(DVFrameC frame)
  {
    UIntT length = frame.Size();
    char type[] = "00db";

    int i;
    for(i=0;i<4;i++) *this << type[i];     
    *this << length;
   
    for(SArray1dIterC<char> it(frame);it;it++) *this << *it;
    
    //creates a new index entry
    aviIndexItemST newIndexItem;
    for(i=0;i<4;i++) newIndexItem.dwChunkId[i]=type[i];
    newIndexItem.dwFlags = (UIntT)16;
    newIndexItem.dwOffset = (UIntT)(end - moviPos - (streampos)8);
    newIndexItem.dwSize = (UIntT)length;

    if(index.IsValid()) index += newIndexItem;
    ONDEBUG(else std::cerr << "index not valid" << std::endl;)

    current++;
    
    //position for a new chunk
    if((current - 1) == dataChunks )
      {
	end += (streampos)(length +8);
	dataChunks++;
	UpdateFrameNumber();
      }
    fileSize += (length + 8);
    moviSize += (length + 8);

    SetFileSize(fileSize);
    
    Seek(moviPos + (streampos)4);
    *this << (UIntT)moviSize;

    Goto(current); // READY to write data chunk

    return true;
  }//END OF bool AviOStreamC::WriteDVFrame(DVFrameC frame)

  bool AviOStreamC::UpdateFrameNumber()
  {
    //computes the number of frames
    int fr=0;
    if(avih.dwStreams==0) return true;
    fr = (int)dataChunks/(int)avih.dwStreams;

    //updates the number of frames into the avi headers
    for(DLIterC<streampos> it(frameNumberPos);it;it++) 
      {
	Seek(*it);
	*this << (UIntT)fr;
      }

    Goto(current); // READY to write data chunk

    return true;
  }//END OF bool AviOStreamC::UpdateFrameNumber()


  bool AviOStreamC::WriteIndex()
  {
    indxPos = end;
    indexSize = 0;
    *this << 'i' << 'd' << 'x' << '1' << indexSize;    
    fileSize += 8;

    for(DLIterC<aviIndexItemST> it(index);it;it++)
      {
	for(int i=0;i<4;i++) *this << it->dwChunkId[i];
	*this << it->dwFlags;
	*this << it->dwOffset;
	*this << it->dwSize;      
	
	indexSize += 16;
	fileSize += 16;
      } 

    Seek(indxPos + (streampos)4);
    *this << indexSize;

    SetFileSize(fileSize);
    return true;
  }//END OF bool AviOStreamC::WriteIndex()
  
  bool AviOStreamC::WriteCerr(const StringC info)
  {
    if(verbose) 
      {
	cerr << info;
	return true;
      }
    return true;
  }//END OF AviOStreamC::WriteCerr(const StringC info)

  bool AviOStreamC::Goto(int frameNumber, int streamNumber)
  {
    int requestedDataChunk = ((frameNumber)*(avih.dwStreams) + streamNumber );
    return Goto(requestedDataChunk);
  }//END OF  AviOStreamC::PrepareToWrite(int frameNumber, int streamNumber)


  bool AviOStreamC::Goto(int requestedDataChunk)
  {
    if(requestedDataChunk > dataChunks ) return false; 

    current = requestedDataChunk;
   
    if(requestedDataChunk == dataChunks) Seek(end);
    else  Seek(moviPos + (streampos)(index.Nth(requestedDataChunk).dwOffset - 8));

    return true;
  }//END OF  AviOStreamC::PrepareToWrite(int frameNumber, int streamNumber)

}
