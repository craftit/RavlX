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
//! file="Ravl/Image/VideoIO/ImgIOCif.cc"

#include "Ravl/Image/ImgIOCif.hh"
#include "Ravl/OS/Filename.hh"
#include "Ravl/Array2dIter.hh"

namespace RavlImageN {
  
  //: Setup for cif variant.
  
  bool DPImageCifBaseBodyC::SetupForVariant(const StringC &suffix) {
    Index2dC size;
    if (suffix == "sqcif")     { size = Index2dC(72,88);     }
    else if (suffix == "qcif") { size = Index2dC(144,176);   }
    else if (suffix == "cif")  { size = Index2dC(288,352);   }
    else if (suffix == "4cif") { size = Index2dC(576,704);   }
    else if (suffix == "16cif"){ size = Index2dC(1152,1408); }
    else {
      std::cerr << "Unknown cif suffix :" << suffix << "\n";
      return false;
    }
    return SetupForVariant(size);
  }
  
  //: Setup for cif variant.
  // Where size in the size of the Y image.
  
  bool DPImageCifBaseBodyC::SetupForVariant(const Index2dC &size) {
    // Work out frame size.
    frameSize = (size.Row().V() * size.Col().V()) * 3/2;
    // Setup image rectangle.
    rect.RowRange() = IndexRangeC(0,size.Row()-1);
    rect.ColRange() = IndexRangeC(0,size.Col()-1);
    
    RavlAssert(!(rect.Rows() &1)); // Must be even number of rows.
    RavlAssert(!(rect.Cols() &1)); // Must be even number of columns.
    return true;
  }
  
  /////////////////////////////////////////////////////////


  //: Constructor from stream 
  
  DPIImageCifBodyC::DPIImageCifBodyC(const IStreamC &nStrm,const StringC &suffix)
    : DPImageCifBaseBodyC(suffix),
      strm(nStrm)
  {
    if(!strm)
      std::cerr << "DPIImageCifBodyC::DPIImageCifBodyC(IStreamC), Passed bad stream. \n";
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
  
  ///////////////////////////
  //: Constructor from stream 
  
  DPIImageCifBodyC::DPIImageCifBodyC(const IStreamC &nStrm,const Index2dC &size)
    : DPImageCifBaseBodyC(size),
      strm(nStrm)
  {
    if(!strm)
      std::cerr << "DPIImageCifBodyC::DPIImageCifBodyC(IStreamC), Passed bad stream. \n";
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
  
  ///////////////////////////
  //: Seek to location in stream.
  // Returns false, if seek failed. (Maybe because its
  // not implemented.)
  // if an error occurered (Seek returned False) then stream
  // position will not be changed.

  bool DPIImageCifBodyC::Seek(UIntT off) {
    if(off == ((UIntT) -1))
      return false; // File to big.
    strm.is().clear(); // Clear any errors.
    strm.Seek(CalcOffset(off));
    if(strm.is().fail())
      return false;
    frameNo = off;// Wait to after seek in case of exception.
    return true;
  }
  
  //: Delta Seek, goto location relative to the current one.
  
  bool DPIImageCifBodyC::DSeek(IntT off) {
    if(off < 0) {
      if((-off) > (IntT) frameNo)
	return false; // Seek off begining of data.
    }
    UIntT nfrmno = frameNo + off;
    strm.is().clear(); // Clear any errors.
    strm.Seek(CalcOffset(nfrmno));
    if(strm.is().fail())
      return false;
    frameNo = nfrmno; // Wait to after seek in case of exception.
    return true;
  }
  
  //: Find current location in stream.
  UIntT DPIImageCifBodyC::Tell() const { 
    return frameNo; 
  }
  
  //: Find the total size of the stream.
  UIntT DPIImageCifBodyC::Size() const { 
    return SeqSize(); 
  }

  /////////////////////////
  //: Get next image.
  
  ImageC<ByteYUVValueC> DPIImageCifBodyC::Get() {
    ImageC<ByteYUVValueC> head;
    if(!Get(head))
      throw DataNotReadyC("Failed to get image. ");
    return head;
  }
  
  //////////////////////////
  //: Get next image.
  
  bool DPIImageCifBodyC::Get(ImageC<ByteYUVValueC> &head) { 
    const IndexC rows = rect.Rows();
    const IndexC cols = rect.Cols();
    if(!strm.good() || strm.is().fail())
      return false;
    
    // Check input image.
    if(head.Rectangle() != rect)
      head = ImageC<ByteYUVValueC>(rect);
    
    // Read data.
    for(Array2dIterC<ByteYUVValueC> it(head);it.IsElm();it.Next())
      it.Data().Y() = strm.get();
    if(!strm.good())
      return false; // Error while reading..
    IndexC r;
    for (r=0; r< rows;) {
      ByteYUVValueC *r1,*r2,*eor;
      r1 = head.Row(r++);
      r2 = head.Row(r++);
      eor = &r1[cols.V()];
      while(r1 != eor) {
	SByteT byte = strm.get() - 128;
	r1->U() = byte;
	r2->U() = byte;
	r1++;
	r2++; 
	r1->U() = byte;
	r2->U() = byte;
	r1++;
	r2++;     
      }
    }
    if(!strm.good())
      return false; // Error while reading..
    for ( r=0; r< rows;) {
      ByteYUVValueC *r1,*r2,*eor;
      r1 = head.Row(r++);
      r2 = head.Row(r++);
      eor = &(r1[cols.V()]);
      while(r1 != eor) {
	SByteT byte = strm.get() - 128;
	r1->V() = byte;
	r2->V() = byte;
	r1++;
	r2++; 
	r1->V() = byte;
	r2->V() = byte;
	r1++;
	r2++;
      }
    }
    if(!strm.good())
      return false; // Error while reading..
    frameNo++;
    return true; 
  }

  //////////////////////////////////////////////////////////////////////////////////////
  
  //: Constructor from stream.  
  
  DPOImageCifBodyC::DPOImageCifBodyC(const OStreamC &nStrm,const StringC &suffix)
    : DPImageCifBaseBodyC(suffix),
      strm(nStrm)
  {
    if(!strm)
      std::cerr << "DPOImageCifBodyC::DPOImageCifBodyC(OStreamC), Passed bad stream. \n";
  }
  
  //: Constructor from stream.  
  
  DPOImageCifBodyC::DPOImageCifBodyC(const OStreamC &nStrm,const Index2dC &size)
    : DPImageCifBaseBodyC(size),
      strm(nStrm)
  {
    if(!strm)
      std::cerr << "DPOImageCifBodyC::DPOImageCifBodyC(OStreamC), Passed bad stream. \n";
  }
  
  //: Seek to location in stream.
  // Returns false, if seek failed. (Maybe because its
  // not implemented.)
  // if an error occurered (Seek returned False) then stream
  // position will not be changed.
  
  bool DPOImageCifBodyC::Seek(UIntT off) {
    if(off == ((UIntT)-1))
      return false; 
    strm.Seek(CalcOffset(off));
    frameNo = off;// Wait to after seek in case of exception.
    if(frameNo > seqSize)
      seqSize = frameNo;
    return true;
  }

  //: Delta Seek, goto location relative to the current one.
  
  bool DPOImageCifBodyC::DSeek(IntT off) {
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
  
  UIntT DPOImageCifBodyC::Tell() const { 
    return frameNo; 
  }
  
  //: Find the total size of the stream.
  
  UIntT DPOImageCifBodyC::Size() const { 
    return SeqSize(); 
  }

  /////////////////////////////////
  // Put next frame into stream.
  // Returns false if can't.
  
  bool DPOImageCifBodyC::Put(const ImageC<ByteYUVValueC> &Img) {
    const IntT rows = rect.Rows();
    const IntT cols = rect.Cols();
    if(!strm.good())
      return false;
    
    IntT v_offset = (rows - (IntT) Img.Rows()) / 2;
    IntT h_offset = (cols - (IntT) Img.Cols()) / 2;
    IndexC r;
    for (r = -v_offset; r < rows-v_offset; ++r) { // Y component
      if ((r >= 0) && (r < Img.Rows())) {
	for (IndexC c = -h_offset; c < cols-h_offset; ++c) {
	  if ((c >= 0) && (c < Img.Cols()))
	    strm.put(Img[r+Img.TRow()][c+Img.LCol()].Y());
	  else strm.put(0);
	}
      }
      else {
	for (IndexC c = -h_offset; c < cols-h_offset; ++c)  strm.put(0);
      }
    }
    for (r = -v_offset; r < rows-v_offset; r+=2) { // U component
      if ((r >= 0) && (r < Img.Rows())) {
	for (IndexC c = -h_offset; c < cols-h_offset; c+=2) {
	  if ((c >= 0) && (c < Img.Cols())) {
	    IndexC R(r+Img.TRow()), C(c+Img.LCol());
	    strm.put( (UByteT) ((Img[R][C].U()+Img[R][C+1].U()+Img[R+1][C].U()+Img[R+1][C+1].U())/4 + 128 ));
	  }
	  else strm.put((UByteT)128);
	}
      }
      else {
	for (IndexC c = -h_offset; c < cols-h_offset; c+=2)  strm.put((UByteT)128);
      }
    }
    for (r = -v_offset; r < rows-v_offset; r+=2) { // V component
      if ((r >= 0) && (r < Img.Rows())) {
	for (IndexC c = -h_offset; c < cols-h_offset; c+=2) {
	  if ((c >= 0) && (c < Img.Cols())) {
	    IndexC R(r+Img.TRow()), C(c+Img.LCol());
	    strm.put((UByteT) ((Img[R][C].V()+Img[R][C+1].V()+Img[R+1][C].V()+Img[R+1][C+1].V())/4 + 128));
	  }
	  else strm.put((UByteT)128);
	}
      }
      else {
	for (IndexC c = -h_offset; c < cols-h_offset; c+=2)  strm.put((UByteT)128);
      }
    }
    frameNo++;
    if(frameNo > seqSize)
      seqSize = frameNo;
    return strm.good();
  }
  
  
  
  //////////////////////////////////////////////////////
  
  
  //: Constructor from filename.  
  
  DPIImageCifC::DPIImageCifC(const StringC &fn)
    : DPEntityC(*new DPIImageCifBodyC(IStreamC(fn),FilenameC(fn).Extension()))
  {}
  
  //: Constructor from filename.  
  
  DPOImageCifC::DPOImageCifC(const StringC &fn)
    : DPEntityC(*new DPOImageCifBodyC(OStreamC(fn),FilenameC(fn).Extension()))
  {}
}
