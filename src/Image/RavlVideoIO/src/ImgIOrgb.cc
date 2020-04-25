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
//! file="Ravl/Image/VideoIO/ImgIOrgb.cc"

#include "Ravl/Image/ImgIOrgb.hh"
#include "Ravl/OS/Filename.hh"
#include "Ravl/Array2dIter.hh"
#include "Ravl/TypeName.hh"
#if RAVL_HAVE_UNISTD_H
#include <unistd.h>
#endif
#if RAVL_OS_WIN32
#include <stdio.h>
#endif
#if RAVL_HAVE_IO_H
#include <io.h>
#endif
#include <fcntl.h>

namespace RavlImageN {

  //: Constructor.
  // This constructs with the basic yuv format.
  
  DPImageRGBBaseBodyC::DPImageRGBBaseBodyC(const Index2dC &size)
    : frameSize(0),
      frameNo(0),
      seqSize((UIntT) -1),
      blockSize(512),
      rawMode(false),
      doneHdr(false)
  { 
    SetupForVariant(size);
  }
  
  //: Setup for rgb variant.
  // Where size in the size of the Y image.
  
  bool DPImageRGBBaseBodyC::SetupForVariant(const Index2dC &size) {
    // Work out frame size.
    frameSize = (size.Row().V() * size.Col().V()) * 3;
    // Setup image rectangle.
    rect.RowRange() = IndexRangeC(0,size.Row()-1);
    rect.ColRange() = IndexRangeC(0,size.Col()-1);
    
    padSize = frameSize % blockSize;
    std::cerr << "Padding :"<<padSize<<"\n";
    return true;
  }
  
  //: Write header block.
  
  bool DPImageRGBBaseBodyC::WriteHdr(StringC type) {
    doneHdr = true;
    if(write(fd,&blockSize,sizeof(blockSize)) != sizeof(blockSize))
      return false;
    int sx = rect.Rows();
    int sy = rect.Cols();
    if(write(fd,&sx,sizeof(int)) != sizeof(int))
      return false;
    if(write(fd,&sy,sizeof(int)) != sizeof(int))
      return false;
    
    int len = type.length();
    if(len > 128) {
      std::cerr << "DPImageRGBBaseBodyC::WriteHdr(), Warning, Clipping class name. \n";
      len = 128;
    }
    if(write(fd,&len,sizeof(int)) != sizeof(int))
      return false;
    if(write(fd,type.chars(),len) != len)
      return false;
    if((UIntT) lseek(fd,blockSize,SEEK_SET) != (UIntT) blockSize) {
      std::cerr << "DPImageRGBBaseBodyC::WriteHdr(), Seek failed. \n";
      return false;
    }
    return true;
  }
  
  //: Read header block.
  
  bool DPImageRGBBaseBodyC::ReadHdr(StringC &typen) {
    doneHdr = true;
    if(read(fd,&blockSize,sizeof(blockSize)) != sizeof(blockSize))
      return false;
    int sx,sy;
    if(read(fd,&sx,sizeof(int)) != sizeof(int))
      return false;
    if(read(fd,&sy,sizeof(int)) != sizeof(int))
      return false;
    SetupForVariant(Index2dC(sx,sy));
    int len;
    if(read(fd,&len,sizeof(int)) != sizeof(int))
      return false;
    typen = StringC("",len+2);
    if(write(fd,typen.chars(),len) != len) 
      return false;
    ((char *)typen.chars())[len] = 0;
    
    if((UIntT) lseek(fd,blockSize,SEEK_SET) != (unsigned int) blockSize) {
      std::cerr << "DPImageRGBBaseBodyC::ReadHdr(), Seek failed. \n";
      return false;
    }
    return true;
  }
  
  /////////////////////////////////////////////////////////
  
  
  ///////////////////////////
  //: Constructor from stream 
  
  DPIImageRGBBodyC::DPIImageRGBBodyC(const IStreamC &nStrm,const Index2dC &size)
    : DPImageRGBBaseBodyC(size),
      strm(nStrm)
  {
    if(!strm)
      std::cerr << "DPIImageRGBBodyC::DPIImageRGBBodyC(IStreamC), Passed bad stream. \n";
    else {
      if(!nStrm.Name().IsEmpty()) {
	FilenameC tmp(nStrm.Name());
	// Stream must be good, so if file doesn't exist maybe its
	// some kind of named pipe.
	if(tmp.Exists())
	  SetSequenceSize (tmp.FileSize() / frameSize);
      }
    }
  }
  
  //: Constructor from stream 
  
  DPIImageRGBBodyC::DPIImageRGBBodyC(const StringC &fn,bool useHdr)  {
    rawMode = true;
    fd = open(fn.chars(),O_RDONLY);
  }
  
  ///////////////////////////
  //: Seek to location in stream.
  // Returns false, if seek failed. (Maybe because its
  // not implemented.)
  // if an error occurered (Seek returned False) then stream
  // position will not be changed.
  
  bool DPIImageRGBBodyC::Seek(UIntT off) {
    if(off == ((UIntT) -1))
      return false; // File to big.
    strm.is().clear(); // Clear any errors.
    strm.Seek(CalcOffset(off));
    frameNo = off;// Wait to after seek in case of exception.
    return true;
  }
  
  //: Delta Seek, goto location relative to the current one.
  
  bool DPIImageRGBBodyC::DSeek(IntT off) {
    if(off < 0) {
      if((-off) > (IntT) frameNo)
	return false; // Seek off begining of data.
    }
    UIntT nfrmno = frameNo + off;
    strm.is().clear(); // Clear any errors.
    strm.Seek(CalcOffset(nfrmno));
    frameNo = nfrmno; // Wait till after seek in case of exception.
    return true;
  }
  
  //: Find current location in stream.
  UIntT DPIImageRGBBodyC::Tell() const
  { return frameNo; }

  //: Find the total size of the stream.
  UIntT DPIImageRGBBodyC::Size() const
  { return SeqSize(); }
  
  /////////////////////////
  //: Get next image.
  
  ImageC<ByteRGBValueC> DPIImageRGBBodyC::Get() {
    ImageC<ByteRGBValueC> head;
    if(!Get(head))
      throw DataNotReadyC("Failed to get image. ");
    return head;
  }
  
  //////////////////////////
  //: Get next image.
  
  bool DPIImageRGBBodyC::Get(ImageC<ByteRGBValueC> &head) { 
    if(!strm.good())
      return false;
    
    if(!doneHdr && rawMode) {
      StringC ntype;
      if(!ReadHdr(ntype)) {
	cerr << "DPIImageRGBBodyC::Get(), Failed to read hdr. \n";
	return false;
      }
      if(ntype != TypeName(typeid(ImageC<ByteRGBValueC>))) {
	cerr << "DPIImageRGBBodyC::Get(), Header type mismatch Read:'" << ntype << "' . \n";
	return false;
      }
    }
    // Check input image.
    if(head.Stride() != ((IntT) rect.Cols()) || head.Rectangle().Origin() != rect.Origin())
      head = ImageC<ByteRGBValueC>(rect);
    
    // sizeof(ByteRGBValueC) == 3 we can do it line at a time.
    //for(ImageIterC<ByteRGBValueC> it(head);it.IsElm();it.Next()) 
    //    strm.read((char *) &it.Data(),3);
    
    char *buff = (char*)&(head[rect.Origin()]);  
    if((UIntT) read(fd,buff,frameSize) != (UIntT) frameSize) // Read image.
      return false;
    if(lseek(fd,padSize,SEEK_CUR) < 0)  // Skip pad bytes.
      return false;
    // --------------------
    
    frameNo++;
    return true; 
  }
  
  //////////////////////////////////////////////////////////////////////////////////////
  
  //: Constructor from stream.  
  
  DPOImageRGBBodyC::DPOImageRGBBodyC(const OStreamC &nStrm,const Index2dC &size)
    : DPImageRGBBaseBodyC(size),
      strm(nStrm)
  {
    std::cerr << "DPOImageRGBBodyC::DPOImageRGBBodyC(), WARNING: Not fully implemented. \n";
    if(!strm)
      std::cerr << "DPOImageRGBBodyC::DPOImageRGBBodyC(OStreamC), Passed bad stream. \n";
  }
  
  //: Constructor from filename
  
  DPOImageRGBBodyC::DPOImageRGBBodyC(const StringC &fn,bool useHdr) {
    rawMode = true;
    fd = open(fn.chars(),O_RDONLY);
  }
  
  //: Seek to location in stream.
  // Returns false, if seek failed. (Maybe because its
  // not implemented.)
  // if an error occurered (Seek returned False) then stream
  // position will not be changed.
  
  bool DPOImageRGBBodyC::Seek(UIntT off) {
    if(off == ((UIntT)-1))
      return false; 
    strm.Seek(CalcOffset(off));
    frameNo = off;// Wait to after seek in case of exception.
    if(frameNo > seqSize)
      seqSize = frameNo;
    return true;
  }
  
  //: Delta Seek, goto location relative to the current one.
  
  bool DPOImageRGBBodyC::DSeek(IntT off) {
    if(off < 0) {
      if((-off) > ((IntT) frameNo))
	return false; // Seek off begining of data.
    }
    UIntT nfrmno = frameNo + off;
    strm.Seek(CalcOffset(nfrmno));
    frameNo = nfrmno; // Wait to after seek in case of exception.
    if(frameNo > seqSize)
      seqSize = frameNo;
    return true;
  }
  
  //: Find current location in stream.
  
  UIntT DPOImageRGBBodyC::Tell() const
  { return frameNo; }
  
  //: Find the total size of the stream.
  
  UIntT DPOImageRGBBodyC::Size() const
  { return SeqSize(); }
  
  /////////////////////////////////
  // Put next frame into stream.
  // Returns false if can't.
  
  bool DPOImageRGBBodyC::Put(const ImageC<ByteRGBValueC> &Img) {
    //const IntT rows = rect.Rows();
    //  const IntT cols = rect.Cols();
    if(!strm.good())
      return false;
    
    if(!doneHdr && rawMode) {
      rect = Img.Rectangle();
      SetupForVariant(Index2dC(rect.Rows(),rect.Cols()));
      WriteHdr(TypeName(typeid(ImageC<ByteRGBValueC>)));
    }
    if(Img.Stride() != ((IntT) rect.Cols())) { // Can't deal with sub-rectangles at the moment
      std::cerr << "DPOImageRGBBodyC::Put(), Image size mismatch. \n";
      return false;
    }
    
    //for(ImageIterC<ByteRGBValueC> it(Img);it.IsElm();it.Next()) 
    //strm.write((const char *) &it.Data(),3);
  
    char *buff = (char*)&(Img[rect.Origin()]);
    if((UIntT) write(fd,buff,frameSize) != (UIntT) frameSize) // Read image.
      return false;
    if(lseek(fd,padSize,SEEK_CUR) < 0)  // Skip pad bytes.
      return false;
    
    // -----------
    
    frameNo++;
    if(frameNo > seqSize)
      seqSize = frameNo;
    return strm.good();
  }
  
  //////////////////////////////////////////////////////
  
  //: Constructor from filename.  
  
  DPIImageRGBC::DPIImageRGBC(const StringC &fn)
    : DPEntityC(*new DPIImageRGBBodyC(IStreamC(fn)))
  {}
  
  //: Constructor from filename.  
  
  DPOImageRGBC::DPOImageRGBC(const StringC &fn)
    : DPEntityC(*new DPOImageRGBBodyC(OStreamC(fn)))
  {}
}
