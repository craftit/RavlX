// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////
//! userlevel=Normal
//! rcsid="$Id$"
//! lib=RavlVideoIO
//! author="Charles Galambos"
//! date="24/03/2002"
//! file="Ravl/Image/VideoIO/ImgIOjs.cc"

#include "Ravl/Image/ImgIOjs.hh"
#include "Ravl/OS/Filename.hh"
#include "Ravl/Array2dIter.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/DP/ByteFileIO.hh"
#include "Ravl/DP/SPortAttach.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlImageN
{

  //: Constructor.
  // This constructs with the basic yuv format.
  
  DPImageJSBaseBodyC::DPImageJSBaseBodyC()
    : frameSize(0),
      framePadding(0),
      frameNo(0),
      seqSize(streamPosUnknown),
      blockSize(16384),
      offset(0)
  { }

  //: Constructor.
  
  DPImageJSBaseBodyC::DPImageJSBaseBodyC(const StringC &filename,bool read)
    : frameSize(0),
      framePadding(0),
      frameNo(0),
      seqSize(streamPosUnknown),
      blockSize(16384),
      offset(0)
  {
    if (read)
    {
      DPIByteFileC byteFile(filename);
      m_inputStream = DPISPortAttachC<ByteT>(byteFile, byteFile);
    }
    else
    {
      m_outputStream = DPOByteFileC(filename);
    }
  }



  DPImageJSBaseBodyC::DPImageJSBaseBodyC(DPISPortC<ByteT> inputPort)
    : frameSize(0),
      framePadding(0),
      frameNo(0),
      seqSize(streamPosUnknown),
      blockSize(16384),
      offset(0)
  {
    m_inputStream = inputPort;
  }



  DPImageJSBaseBodyC::DPImageJSBaseBodyC(DPOPortC<ByteT> outputPort)
    : frameSize(0),
      framePadding(0),
      frameNo(0),
      seqSize(streamPosUnknown),
      blockSize(16384),
      offset(0)
  {
    m_outputStream = outputPort;
  }

  //: Read header from stream.
  
  bool DPImageJSBaseBodyC::ReadHeader() {
    if(!m_inputStream.IsValid() || !m_inputStream.IsGetReady()) {
      std::cerr << "DPImageJSBaseBodyC::ReadHeader(), Bad stream. \n";
      return false;
    }
    ByteT magic[4];
    SArray1dC<ByteT> magicArray(magic, 4, false);
    if(m_inputStream.GetArray(magicArray) < 0) {
      std::cerr << "DPImageJSBaseBodyC::ReadHeader(), Failed to read magic bytes. \n";
      return false;
    }
#if DODEBUG
    for(int i = 0;i < 4;i++)
      std::cerr << hex << (int) magic[i] << ' ';
    std::cerr << "\n";
#endif
    if((((int) magic[0]) != 0) || (((int) magic[1]) != 0x6) || 
       (((int) magic[2]) != 0x9) || (((int) magic[3]) != ((int) 0xce))) {
      std::cerr << "DPImageJSBaseBodyC::ReadHeader(), Bad magic number. \n";
      return false;
    }
    
    UIntT header[11];
    
    int i;
    for(i = 0;i < 9;i++) {
      UIntT x = 0;
      SArray1dC<ByteT> xArray(reinterpret_cast<ByteT*>(&x), 4, false);
      if(!m_inputStream.GetArray(xArray))
	return false;
#if RAVL_ENDIAN_LITTLE
      x = bswap_32(x);
#endif
      header[i] = x;
    }
    
#if DODEBUG
    std::cerr << "Header :\n";
    for(i = 0;i < 9;i++) {
      std::cerr << " " << i << "=" << header[i];
    }
    std::cerr << "\n" << hex;
    for(i = 0;i < 9;i++) {
      std::cerr << " " << i << "=" << header[i];
    }
    std::cerr << dec << "\n";
#endif
    
    // 0 Typical 3
    // 1 Typical 3007 2989 ...
    // 2 16384, block size.
    // 3 Typical 1
    // 4 width
    // 5 height
    // 6 Typical 1008
    // 7 Typical 14656
    // 8 Typical 0xe0000000
    
    blockSize = header[2];
    int width = header[4];
    int height = header[5];
    
    
    rect = ImageRectangleC(height,width);
    SetupIO();
    ONDEBUG(cerr << " BlockSize=" << blockSize << " Width=" << width << " Height=" << height << " \n");
    
    StreamPosT imageData = m_inputStream.Size64() - offset;
    seqSize = (imageData / frameSize);
    
    // There maybe unwritten padding for the last block.
    StreamPosT imgSize = (rect.Area() * 2);
    if((imageData % frameSize) >= imgSize)
      seqSize++; 
    
    ONDEBUG(cerr << "DPIImageJSBodyC::ReadHeader(), Sequence size=" << seqSize << " Filesize=" << m_inputStream.Size64() << "\n");
    
    return true;
  }

  //: Setup paramiters needed for io.
  
  void DPImageJSBaseBodyC::SetupIO() {
    StreamPosT imgSize = (rect.Area() * 2);
    
    // Fixme: The image size may be slightly larger, as it might contain out of frame info.
    
    framePadding = blockSize - (imgSize % blockSize);
    frameSize = imgSize + framePadding;
    
    offset = frameSize;
    ONDEBUG(cerr << "blockSize=" << blockSize << " imageSize=" << imgSize << " frameSize=" << frameSize << " framePadding=" << framePadding << " offset=" << offset << endl);
  }
  
  
  /////////////////////////////////////////////////////////
  

  ///////////////////////////
  //: Constructor from stream 

  DPIImageJSBodyC::DPIImageJSBodyC(const IStreamC &nStrm)
  { 
    RavlAssertMsg(0,"Not supported. ");
    //ReadHeader();
  }
  
  //: Constructor from a file.
  
  DPIImageJSBodyC::DPIImageJSBodyC(const StringC &fileName) 
  : DPImageJSBaseBodyC(fileName,true)
  { ReadHeader(); }



  DPIImageJSBodyC::DPIImageJSBodyC(DPISPortC<ByteT> inputPort)
  : DPImageJSBaseBodyC(inputPort)
  { ReadHeader(); }



  bool DPIImageJSBodyC::Seek(UIntT off)
  {
    if (off == static_cast<UIntT>(-1))
      return false;
    return Seek64(off);
  }



  bool DPIImageJSBodyC::DSeek(IntT off)
  {
    return DSeek64(off);
  }


  
  UIntT DPIImageJSBodyC::Tell() const
  {
    if (frameNo > static_cast<UIntT>(-1) || frameNo == streamPosUnknown)
      return static_cast<UIntT>(-1);
    return static_cast<UIntT>(frameNo);
  }



  UIntT DPIImageJSBodyC::Size() const
  {
    StreamPosT size = SeqSize();
    if (size == streamPosUnknown || size > static_cast<UIntT>(-1))
      return static_cast<UIntT>(-1);
    return static_cast<UIntT>(size);
  }


  
  bool DPIImageJSBodyC::Seek64(StreamPosT off)
  {
    if (off < 0 || off == streamPosUnknown)
      return false;
    frameNo = off;
    return true;
  }



  bool DPIImageJSBodyC::DSeek64(StreamPosT off)
  {
    if (off < 0)
    {
      if (-off > frameNo)
      	return false;
    }
    frameNo += off;
    return true;
  }



  StreamPosT DPIImageJSBodyC::Tell64() const
  {
    return frameNo;
  }



  StreamPosT DPIImageJSBodyC::Size64() const
  {
    return SeqSize();
  }

  /////////////////////////
  //: Get next image.
  
  ImageC<ByteYUV422ValueC> DPIImageJSBodyC::Get() {
    ImageC<ByteYUV422ValueC> head;
    if(!Get(head))
      throw DataNotReadyC("Failed to get image. ");
    return head;
  }
  
  //////////////////////////
  //: Get next image.
  
  bool DPIImageJSBodyC::Get(ImageC<ByteYUV422ValueC> &head) {
    if (!m_inputStream.IsValid())
      return false;

    m_inputStream.Seek64(CalcOffset(frameNo));
    
    // Check input image.
    
    if(head.Rectangle() != rect) {
      head = ImageC<ByteYUV422ValueC>(rect);

      StreamPosT frameSize = rect.Area() * sizeof(ByteYUV422ValueC);
      SArray1dC<ByteT> imageArray(reinterpret_cast<ByteT*>(&(head[rect.Origin()])), frameSize, false);
      if(m_inputStream.GetArray(imageArray) != frameSize) // Zero indicates end of file.
        return false;
    } else {
      StreamPosT width = head.Cols() * sizeof(ByteYUV422ValueC);
      IndexC atrow = head.TRow();
      IndexC offset = head.LCol();
      IndexC brow = head.BRow();
      for(;atrow <= brow;atrow++) {
        SArray1dC<ByteT> rowArray(reinterpret_cast<ByteT*>(&(head[atrow][offset])), width, false);
	      if(m_inputStream.GetArray(rowArray) <= 0) // Zero indicates end of file.
	        return false;
      }
    }
    //ONDEBUG(cerr << "Reading image... \n");
    frameNo++;
    return true; 
  }
  
  //////////////////////////////////////////////////////////////////////////////////////
  
  //: Constructor from stream.  
  
  DPOImageJSBodyC::DPOImageJSBodyC(const OStreamC &nStrm)
  : m_doneHeader(false),
    m_doFramePadding(false)
  {
    RavlAssertMsg(0,"Not supported. ");
    //if(!strm.Good())
    //cerr << "DPOImageJSBodyC::DPOImageJSBodyC(OStreamC), Passed bad stream. \n";
  }
  
  //: Constructor from stream 
  
  DPOImageJSBodyC::DPOImageJSBodyC(const StringC &fileName) 
  : DPImageJSBaseBodyC(fileName,false),
    m_doneHeader(false),
    m_doFramePadding(false)
  {}



  DPOImageJSBodyC::DPOImageJSBodyC(DPOPortC<ByteT> outputPort)
  : DPImageJSBaseBodyC(outputPort),
    m_doneHeader(false),
    m_doFramePadding(false)
  {}

  //: Write js header.
  
  bool DPOImageJSBodyC::WriteHeader(const ImageRectangleC &wrect) {
    RavlAssert(m_outputStream.IsValid());
    if(m_doneHeader)
      return true;
    rect = wrect;
    
    ByteT magic[4];
    magic[0] = 0;
    magic[1] = 0x6;
    magic[2] = 0x9;
    magic[3] = 0xce;
    SArray1dC<ByteT> magicArray(magic, 4, false);
    if(m_outputStream.PutArray(magicArray) != static_cast<IntT>(magicArray.Size()))
      return false;
    
    int datatype = 3,width = wrect.Cols(),height = wrect.Rows();
    
    UIntT header[11];
    
    header[0]= datatype;      // Typical 3.
    header[1]= ((int) 3007);  // Typical 3007 2989 ...
    header[2]= blockSize;     // 16384, offset of first frame.
    header[3]= ((int) 1);     // Typical 1
    header[4]= width;         // width.
    header[5]= height;        // height.
    header[6]= ((int) 1008);  // Typical 1008
    header[7]= ((int) 14656); // Typical 14656
    header[8]= ((UIntT) 0xe0000000); // Typical.
    
#if RAVL_ENDIAN_LITTLE
    for(int i = 0;i < 9;i++)
      header[i] = bswap_32(header[i]);
#endif
    SArray1dC<ByteT> headerArray(reinterpret_cast<ByteT*>(header), 9 * sizeof(UIntT), false);
    if(m_outputStream.PutArray(headerArray) != static_cast<IntT>(headerArray.Size()))
      return false;

    SetupIO();

    SArray1dC<ByteT> paddingArray(offset - 40);
    if(m_outputStream.PutArray(paddingArray) <= 0)
      return false;
    
    m_doneHeader = true;
    return true;
  }
  

  
  bool DPOImageJSBodyC::Seek(UIntT off)
  {
    if (off == static_cast<UIntT>(-1))
      return false;
    return Seek64(off);
  }



  bool DPOImageJSBodyC::DSeek(IntT off)
  {
    return DSeek64(off);
  }



  UIntT DPOImageJSBodyC::Tell() const
  {
    if (frameNo > static_cast<UIntT>(-1) || frameNo == streamPosUnknown)
      return static_cast<UIntT>(-1);
    return static_cast<UIntT>(frameNo);
  }



  UIntT DPOImageJSBodyC::Size() const
  {
    StreamPosT size = SeqSize();
    if (size > static_cast<UIntT>(-1) || size == streamPosUnknown)
      return static_cast<UIntT>(-1);
    return static_cast<UIntT>(size);
  }


  
  bool DPOImageJSBodyC::Seek64(StreamPosT off)
  {
    if (off < 0 || off == static_cast<UIntT>(-1))
      return false;
    frameNo = off;
    return true;
  }



  bool DPOImageJSBodyC::DSeek64(StreamPosT off)
  {
    if (off < 0)
    {
      if (-off > frameNo)
      	return false;
    }
    frameNo += off;
    return true;
  }



  StreamPosT DPOImageJSBodyC::Tell64() const
  {
    return frameNo;
  }



  StreamPosT DPOImageJSBodyC::Size64() const
  {
    return SeqSize();
  }

  /////////////////////////////////
  // Put next frame into stream.
  // Returns false if can't.
  
  bool DPOImageJSBodyC::Put(const ImageC<ByteYUV422ValueC> &img) {
    if (!m_outputStream.IsValid())
      return false;
    
    if(!m_doneHeader) {
      if(!WriteHeader(img.Rectangle())) {
        std::cerr << "DPOImageJSBodyC::Put(), ERROR: Failed to write file header. \n";
        return false;
      }
    }

    //TODO(WM) Do seeky stuff here

    if (m_doFramePadding && framePadding != 0)
    {
      SArray1dC<ByteT> paddingArray(framePadding);
      if(m_outputStream.PutArray(paddingArray) <= 0)
        return false;
    }

    RavlAssert(img.Rectangle() == rect); // Expected image size ?
    if(&(img[rect.TRow()][rect.RCol()]) == (&(img[rect.TRow()+1][rect.LCol()]))+1) {
      const ByteT* imagePtr = reinterpret_cast<const ByteT*>(&(img[rect.Origin()]));
      StreamPosT frameSize = rect.Area() * sizeof(ByteYUV422ValueC);
      SArray1dC<ByteT> imageArray(const_cast<ByteT*>(imagePtr), frameSize, false);
      if(m_outputStream.PutArray(imageArray) != frameSize)
        return false;
    } else {
      StreamPosT width = img.Cols() * sizeof(ByteYUV422ValueC);
      IndexC atrow = img.TRow();
      IndexC offset = img.LCol();
      IndexC brow = img.BRow();
      for(;atrow <= brow;atrow++)
      {
        const ByteT* rowPtr = reinterpret_cast<const ByteT*>(&(img[atrow][offset]));
        SArray1dC<ByteT> rowArray(const_cast<ByteT*>(rowPtr), width, false);
      	if(m_outputStream.PutArray(rowArray) <= 0)
          return false;
      }
    }
    
    frameNo++;
    if(frameNo > seqSize)
      seqSize = frameNo;

    m_doFramePadding = true;
    
    return true;
  }
  
  
  
  //////////////////////////////////////////////////////
  
  //: Constructor from filename.  
  
  DPIImageJSC::DPIImageJSC(const StringC &fn)
    : DPEntityC(*new DPIImageJSBodyC(fn))
  {}

  //: Constructor from filename.  
  
  DPOImageJSC::DPOImageJSC(const StringC &fn)
    : DPEntityC(*new DPOImageJSBodyC(fn))
  {}
}
