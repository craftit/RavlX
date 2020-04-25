// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////
//! userlevel=Normal
//! rcsid="$Id$"
//! lib=RavlVideoIO
//! file="Ravl/Image/VideoIO/BGRFrmIOAvi.cc"

#include "Ravl/Image/BGRFrmIOAvi.hh"
#include "Ravl/OS/Filename.hh"
#include "Ravl/Array2dIter.hh"
#include "Ravl/Math.hh"

//using namespace RavlN;

namespace RavlImageN {
  
  //: Constructor from stream 
  DPISBGRAviFrameBodyC::DPISBGRAviFrameBodyC(const IStreamC &nStrm)
    : aviStrm(nStrm,false),
      currentframeNo(0) 
  {
    if(!aviStrm.Stream()) std::cerr << "DPISBGRAviFrameBodyC::DPISBGRAviFrameBodyC(IStreamC), Passed bad stream. \n";

    aviStrm.GetVideoStreamFormatInfo(strf); //gets the video info 
  }
  
  ///////////////////////////
  ///////////////////////////
  //: Seek to location in stream.
  // Returns false, if seek failed. (Maybe because its
  // not implemented.)
  // if an error occurered (Seek returned False) then stream
  // position will not be changed.

  bool DPISBGRAviFrameBodyC::Seek(UIntT frameNo) 
  {
    if(aviStrm.GotoDataPos(frameNo,0)) 
      {
	currentframeNo = frameNo;//goes to the correct position and updates the current frame 
	return true;
      }
    else return false;
  }
  
  //: Delta Seek, goto location relative to the current one.
  
  bool DPISBGRAviFrameBodyC::DSeek(IntT frames) 
  {
    IntT newframeNo = currentframeNo + frames; 
    if(aviStrm.GotoDataPos(newframeNo,0)) 
      {
	currentframeNo = newframeNo;//goes to the correct position and updates the current frame 
	return true;
      }
    else return false;
  }
  
  //: Find current location in stream.
  UIntT DPISBGRAviFrameBodyC::Tell() const { 
    return currentframeNo; 
  }
  
  //: Find the total size of the stream.
  UIntT DPISBGRAviFrameBodyC::Size() const 
  {
    return aviStrm.GetAviHeaderInfo().dwTotalFrames; 
  }
  
  /////////////////////////
  //: Get next image.
  
  ImageC<ByteBGRValueC> DPISBGRAviFrameBodyC::Get() 
  {
    int h = (int)Abs(strf.biHeight);
    int w = (int)Abs(strf.biWidth);

    IndexRange2dC myframesize(h,w);

    ImageC<ByteBGRValueC> head(myframesize);
    Get(head);
    return head;
  }
  
  //////////////////////////
  //: Get next image.
  
  bool DPISBGRAviFrameBodyC::Get(ImageC<ByteBGRValueC> &head) 
  { 
    UIntT size;
    int h = (int)Abs(strf.biHeight);
    int w = (int)Abs(strf.biWidth);

    IndexRange2dC myframesize(h,w);
    if(head.Frame() != myframesize) head = ImageC<ByteBGRValueC>(myframesize);
   
    if(!aviStrm.Stream().good())
      return false;

    if(!Seek(currentframeNo))return false;

    currentframeNo++;

    //the position is suposed to be selected with GotoDataPos
    //skips the frame size
    aviStrm >> size;
    UIntT checkSize = 0; 
    for(Array2dIterC<ByteBGRValueC> it(head);it.IsElm();it.Next())
      {
	aviStrm >> it->Blue();
	aviStrm >> it->Green();
	aviStrm >> it->Red();
	checkSize += 3;
      }

    RavlAssert(checkSize == size);
    return true; 
  }

  //////////////////////////////////////////////////////////////////////////////////////
  
  //: Constructor from stream.  
  
  DPOSBGRAviFrameBodyC::DPOSBGRAviFrameBodyC(const OStreamC &nStrm)
    : currentframeNo(0),
      framesAdded(0)
  {
    if(!nStrm)
      std::cerr << "DPOSBGRAviFrameBodyC::DPOSBGRAviFrameBodyC(OStreamC), Passed bad stream. \n";

    DListC<StringC> fccTypes;
    fccTypes += "vids";

    aviStrm = AviOStreamC(nStrm, fccTypes, true, false); 

    //inserts an empty header (NOTE: NO PARAMETER can be specified since the images are known
    //only after the put() method is called) 
    /////////////////////////////////////////////////////////////////////////////////////
  }
  
  //: Seek to location in stream.
  // Returns false, if seek failed. (Maybe because its
  // not implemented.)
  // if an error occurered (Seek returned False) then stream
  // position will not be changed.
  
  bool DPOSBGRAviFrameBodyC::Seek(UIntT frameNo) 
  {
    if(aviStrm.Goto(frameNo,0))
      {
	currentframeNo = frameNo;
	return true;
      }
    else return false;
  }

  //: Delta Seek, goto location relative to the current one.
  
  bool DPOSBGRAviFrameBodyC::DSeek(IntT off) 
  {
    int frNo = currentframeNo + off;
    if(aviStrm.Goto(frNo,0))
      {
	currentframeNo = frNo;
	return true;
      }
    else return false;
  }
  
  //: Find current location in stream.
  
  UIntT DPOSBGRAviFrameBodyC::Tell() const { 
    return currentframeNo; 
  }
  
  //: Find the total size of the stream.////////////////////////
  
  UIntT DPOSBGRAviFrameBodyC::Size() const { 
    return framesAdded; 
  }

  /////////////////////////////////
  // Put next frame into stream.
  // Returns false if can't.
  
  bool DPOSBGRAviFrameBodyC::Put(const ImageC<ByteBGRValueC> &Img)
  {
    //The first time Put() is called it initializes the header using properties of the first image
    
    if(framesAdded == 0) 
      {
	UIntT mss = (UIntT)((1.0/25)*(1000000));

	UIntT width = 0;
	UIntT height = 0;
	UIntT size = 0;
	
	if(!Img.IsValid()) return false;
	else
	  { 
	    width = Img.Cols();
	    height = Img.Rows();
	    size = width * height * (UIntT)3;
	  } 

	//define avih
	aviStrm.avih.dwMicroSecPerFrame = mss; //1/fps*10^6
	aviStrm.avih.dwMaxBytesPerSec = size * (UIntT)25 ; // dwSuggestedBufferSize * fps
	aviStrm.avih.dwPaddingGranularity = 0; // use default (2k)
	aviStrm.avih.dwFlags[0]=16;//AVIF_HASINDEX	0x00000010 
	aviStrm.avih.dwFlags[1]=8;// AVIF_TRUSTCKTYPE	0x00000800
	aviStrm.avih.dwFlags[2]=0;//WAS CAPTUREFILE = false
	aviStrm.avih.dwFlags[3]=0;//not COPYRIGHTED
	aviStrm.avih.dwTotalFrames = 0;
	aviStrm.avih.dwInitialFrames=0;//default for non interleaved ("vids")
	aviStrm.avih.dwStreams = 1; //number of streams =  fccHandlers.Size();
	aviStrm.avih.dwSuggestedBufferSize=size; //max image size 
	aviStrm.avih.dwWidth = width;
	aviStrm.avih.dwHeight = height;
	for(int i=0;i<4;i++) aviStrm.avih.dwReserved[i]=(UIntT)0;
	
	aviStrm.WriteMainHeader();
	
	//define strh
	StreamHeaderST strh;
	
	strh.fccType[0]='v';
	strh.fccType[1]='i';
	strh.fccType[2]='d';
	strh.fccType[3]='s';
	strh.fccHandler[0]=(char)0; 
	strh.fccHandler[1]=(char)0; 
	strh.fccHandler[2]=(char)0; 
	strh.fccHandler[3]=(char)0; 
	strh.dwFlags[0]=(char)0;
	strh.dwFlags[1]=(char)0;
	strh.dwFlags[2]=(char)0;
	strh.dwFlags[3]=(char)0;
	strh.wPriority=0;
	strh.wLanguage=0;
	strh.dwInitialFrames=0;
	strh.dwScale=1;	
	strh.dwRate=25;	// dwRate / dwScale == samples/second 
	strh.dwStart=0;
	strh.dwLength=aviStrm.avih.dwTotalFrames; // In units above... 
	strh.dwSuggestedBufferSize=aviStrm.avih.dwSuggestedBufferSize;
	strh.dwQuality=0;
	strh.dwSampleSize=0;//sample size can vary
	strh.rcFrame[0]=0;
	strh.rcFrame[1]=0;
	strh.rcFrame[2]=aviStrm.avih.dwWidth;
	strh.rcFrame[3]=aviStrm.avih.dwHeight;
  
	aviStrm.WriteStreamHeader(0,strh);

	//define strf
	BitmapInfoHST strf;
	
	strf.biSize=40;
	strf.biWidth= aviStrm.avih.dwWidth;
	strf.biHeight=(-1)*aviStrm.avih.dwHeight;
	strf.biPlanes=1;
	strf.biBitCount=24;
	strf.biCompression[0]=(char)0; 
	strf.biCompression[1]=(char)0; 
	strf.biCompression[2]=(char)0; 
	strf.biCompression[3]=(char)0; 
	strf.biSizeImage=0;//set to 0 for uncompressed BGR bitmaps
	strf.biXPelsPerMeter=0;
	strf.biYPelsPerMeter=0;
	strf.biClrUsed=0;
	strf.biClrImportant=0;
  
	aviStrm.WriteVideoStreamFormatInfo(strf);
	
	aviStrm.Goto(0,0);
      }

    aviStrm.Goto(framesAdded ++,0);
    aviStrm.WriteBGRImage(Img);

    return aviStrm.Stream().good();
  }
  
  
  
  //////////////////////////////////////////////////////
  
  
  //: Constructor from filename.  
  
  DPISBGRAviFrameC::DPISBGRAviFrameC(const StringC &fn)
    : DPEntityC(*new DPISBGRAviFrameBodyC(IStreamC(fn)))
  {}
  
  //: Constructor from filename.  
  
  DPOSBGRAviFrameC::DPOSBGRAviFrameC(const StringC &fn)
    : DPEntityC(*new DPOSBGRAviFrameBodyC(OStreamC(fn)))
  {}
}
