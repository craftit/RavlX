// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////
//! userlevel=Normal
//! rcsid="$Id: ImgIOdvsypbpr.cc 3813 2003-12-11 12:08:36Z simone $"
//! lib=RavlRawVidIO
//! file="Ravl/Image/VideoIO/ImgIOdvsypbpr.cc"

#include "Ravl/Image/ImgIOdvsypbpr.hh"
#include "Ravl/OS/Filename.hh"
#include "Ravl/Array2dIter.hh"
#include "Ravl/OS/DMABuffer.hh"
#include  "Ravl/Image/Deinterlace.hh"
#include "Ravl/Image/NewGrabfileReader.hh"
#include "Ravl/Image/NewGrabfileWriter.hh"
#include "Ravl/Image/Utilities.hh"
#include "Ravl/StrStream.hh"
#include "Ravl/DP/FileFormatIO.hh"
#include "Ravl/Image/ImageConv.hh"
#include "Ravl/Image/FloatYPbPr422BT709Value.hh"
#include "Ravl/Image/FloatYPbPrBT709Value.hh"
#include "Ravl/Image/RealRGBValue.hh"
#include "Ravl/Image/UInt16RGBValue.hh"

using namespace RavlImageN;
using namespace RavlN;

namespace RavlImageN {
  
  //: Setup for yuv variant.
  // Where size in the size of the Y image.
  
  bool DPImageDVSYPbPr422BaseBodyC::SetupForVariant(const Index2dC &size) {
    // Work out frame size.
    frameSize = (size.Row().V() * size.Col().V()) * 2;
    // Setup image rectangle.
    rect.RowRange() = IndexRangeC(0,size.Row()-1);
    rect.ColRange() = IndexRangeC(0,size.Col()-1);
    
    RavlAssert(!(rect.Rows() &1)); // Must be even number of rows.
    RavlAssert(!(rect.Cols() &1)); // Must be even number of columns.
    return true;
  }
  
  /////////////////////////////////////////////////////////
  
  ///////////////////////////
  //: Constructor from stream 

  DPIImageDVSYPbPr422BodyC::DPIImageDVSYPbPr422BodyC( GrabfileReaderC& file_reader,const IStreamC &nStrm,const Index2dC &size)
    : DPImageDVSYPbPr422BaseBodyC(size),
      strm(nStrm),
      file_read(file_reader)
  {
    if(!file_read.Ok()) {
       cerr << "DPIImageYPbPrbBodyC::DPIImageYPbPrBodyC(GrabfileReader), Passed bad GrabfileReader. \n";
    }
    frameSize = (file_read.getVideoBufSize() + file_read.getAudioBufSize() + 8);
    seqSize = file_read.FramesLoaded();
  }
  
  ///////////////////////////
  //: Seek to location in stream.
  // Returns false, if seek failed. (Maybe because its
  // not implemented.)
  // if an error occured (Seek returned False) then stream
  // position will not be changed.
  
  bool DPIImageDVSYPbPr422BodyC::Seek(UIntT off) {
    if(off == ((UIntT) -1))
      return false; // File to big.
    file_read.Seek(CalcOffset(off));
    frameNo = off;// Wait to after seek in case of exception.
    return true;
  }
  
  //: Delta Seek, goto location relative to the current one.
  
  bool DPIImageDVSYPbPr422BodyC::DSeek(IntT off) {
    if(off < 0) {
      if((-off) > (IntT) frameNo)
	return false; // Seek off begining of data.
    }
    UIntT nfrmno = frameNo + off;
    file_read.Seek(CalcOffset(nfrmno));
    frameNo = nfrmno; // Wait till after seek in case of exception.
    return true;
  }
  
  //: Find current location in stream.
  UIntT DPIImageDVSYPbPr422BodyC::Tell() const { 
    return frameNo; 
  }
  
  //: Find the total size of the stream.
  UIntT DPIImageDVSYPbPr422BodyC::Size() const { 
    return SeqSize(); 
  }

  /////////////////////////
  //: Get next image.
  
  ImageC<FloatYPbPr422BT709ValueC> DPIImageDVSYPbPr422BodyC::Get() {
    ImageC<FloatYPbPr422BT709ValueC> head;
    Get(head);
    return head;
  }
  
  //////////////////////////
  //: Get next image.
  
  bool DPIImageDVSYPbPr422BodyC::Get(ImageC<FloatYPbPr422BT709ValueC> &head) {
    if(!file_read.Ok() || file_read.HaveMoreFrames()) {
      return false;
    }
    
    // Check input image.
    if(head.Rectangle() != rect) {
      head = ImageC<FloatYPbPr422BT709ValueC>(rect);
    }

    BufferC<char> res;
    res = file_read.GetNextFrame();

   
    //: Guess the image dimensions.
    UIntT xDim=0, yDim=0 ;
    if(!GetDimensions(IdToVideoMode(file_read.VideoMode()),xDim,yDim)) {
        //Unsuccessfully got dimensions.
  	xDim = 1080 ; 
  	yDim = 1920 ;
	RavlIssueWarning("\nDont know which image size to use for this format ! will try 1080x1920"); 
    }
    // create an image from the buffer.
    ImageC<ByteYUV422ValueC> image1( xDim, yDim, (ByteYUV422ValueC*) & res.BufferAccess()[0], false) ; // image is only valid while buffer exists !
    FloatT Yvalue,Pbrvalue;
    for(Array2dIter2C<ByteYUV422ValueC,FloatYPbPr422BT709ValueC> it(image1,head);it;it++) { 
           Yvalue = (FloatT)it.Data1().Y();   //-16;
           Pbrvalue = (FloatT)it.Data1().UV();   //-128;
           it.Data2() = FloatYPbPr422BT709ValueC(Pbrvalue,Yvalue);
    }
    head = ImageC<FloatYPbPr422BT709ValueC>(Interlace( head ));

    frameNo++;
    return true; 
  }
  
  //////////////////////////////////////////////////////////////////////////////////////
  
  //: Constructor from stream.  
  
  DPOImageDVSYPbPr422BodyC::DPOImageDVSYPbPr422BodyC(GrabfileWriterC& file_write,const OStreamC &nStrm,const Index2dC &size)
    : DPImageDVSYPbPr422BaseBodyC(size),
      strm(nStrm),
      //Imgcopy(),
      file_writer(file_write)
  {
    if(!file_write.Ok()) {
      cerr << "DPOImageDVSYPbPr422BodyC::DPOImageDVSYPbPr422BodyC(OStreamC), Passed bad file writer. \n";
    }
  }
  
  //: Seek to location in stream.
  // Returns false, if seek failed. (Maybe because its
  // not implemented.)
  // if an error occurered (Seek returned False) then stream
  // position will not be changed.
  
  bool DPOImageDVSYPbPr422BodyC::Seek(UIntT off) {
    if(off == ((UIntT)-1))
      return false; 
    strm.Seek(CalcOffset(off));
    if(!strm.good())
      return false;
    frameNo = off;// Wait to after seek in case of exception.
    if(frameNo > seqSize)
      seqSize = frameNo;
    return true;
  }
  
  //: Delta Seek, goto location relative to the current one.
  
  bool DPOImageDVSYPbPr422BodyC::DSeek(IntT off) {
    if(off < 0) {
      if((-off) > ((IntT) frameNo))
	return false; // Seek off begining of data.
    }
    UIntT nfrmno = frameNo + off;
    strm.Seek(CalcOffset(nfrmno));
    if(!strm.good())
      return false;
    frameNo = nfrmno; // Wait to after seek in case of exception.
    if(frameNo > seqSize)
      seqSize = frameNo;
    return true;
  }
  
  //: Find current location in stream.
  
  UIntT DPOImageDVSYPbPr422BodyC::Tell() const { 
    return frameNo; 
  }

  //: Find the total size of the stream.
  
  UIntT DPOImageDVSYPbPr422BodyC::Size() const { 
    return SeqSize(); 
  }

  /////////////////////////////////
  // Put next frame into stream.
  // Returns false if can't.
  
  bool DPOImageDVSYPbPr422BodyC::Put(const ImageC<ByteYUV422ValueC> &Img) {


    Imgcopy = Img;
    //Work out if it is a PAL sized image or SMTPE sized image.
    if(Imgcopy.Size() == (576*720)) {
       //PAL
       if(file_writer.VideoBuffer() != 831488) {
          //reset the video mode on the new grab file.
	  vmode = PAL;
	  bformat = BITS_8;
	  vbuffer = 831488;
	  file_writer.Reset(vmode,bformat,vbuffer);
       }
    }
    if(Imgcopy.Size() == (1080*1920)) {
       //SMPTE274_25I
       if(file_writer.VideoBuffer() != 5529600) {
          //reset the video mode on the new grab file.
          vmode = SMPTE274_25I;
          bformat = BITS_10_DVS;
          vbuffer = 5529600;
	  file_writer.Reset(vmode,bformat,vbuffer);
       }

    }
    
    SArray1dC<char> chararray(Imgcopy.Size() * 2);
    int i =0;
    ImageC<ByteYUV422ValueC> dimage;
    dimage = DeInterlace(Imgcopy);
    for(Array2dIterC<ByteYUV422ValueC> it(dimage);it;it++)  {
       ByteYUV422ValueC rgc = it.Data();
       chararray[i] = (char)rgc.Y();
       chararray[i+1] = (char)rgc.UV();
       i+=2;
    }   
   
    file_writer.PutFrame(chararray);
    frameNo++;
    return file_writer.Ok();
  }
  
  
  
  //////////////////////////////////////////////////////
  
  //: Constructor from filename.  
  
  DPIImageDVSYPbPr422C::DPIImageDVSYPbPr422C(GrabfileReaderC& file_reader,const StringC &fn)
    : DPEntityC(*new DPIImageDVSYPbPr422BodyC(file_reader,IStreamC(fn)))
  {}

  //: Constructor from filename.  
  
  DPOImageDVSYPbPr422C::DPOImageDVSYPbPr422C(GrabfileWriterC& file_write,const StringC &fn)
    : DPEntityC(*new DPOImageDVSYPbPr422BodyC(file_write,OStreamC(fn)))
  {}
}
