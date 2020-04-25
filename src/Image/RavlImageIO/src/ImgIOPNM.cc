// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlImageIO
//! file="Ravl/Image/ImageIO/ImgIOPNM.cc"

#include "Ravl/BinStream.hh"
#include "Ravl/Image/ImgIOPNM.hh"
#include "Ravl/Image/ImgIOPNMB.hh"
#include "Ravl/Array2dIter.hh"
#include <ctype.h>
#include <stdlib.h>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlImageN {
  
  // PNMByteGrey
  
  //: Load a PNM header.
  
  bool DPImageIOPNMBaseC::LoadHeader(IStreamC &inf,ByteT &header,IntT &x,IntT &y) {
    if(!inf) {
      ONDEBUG(cerr << "DPImageIOPNMBaseC::LoadHeader(). ERROR: Bad input stream in " << inf.Name() << " \n");
      //inf.DiagnoseStream(cerr);
      return false;
    }
    char inb[4];
    inf.read(inb,3);
    if(!inf) {
      ONDEBUG(cerr << "DPImageIOPNMBaseC::LoadHeader(). ERROR: Bad input stream in " << inf.Name() << " reading file header. \n");
      //inf.DiagnoseStream(cerr);
      return false;
    }
    if(inb[0] != 'P') {
      ONDEBUG(cerr << "DPImageIOPNMBaseC::LoadHeader(): Unrecognised image format ID (" << (int) inb[0] << "). in " << inf.Name() << "\n");
      inb[3] = 0;
      ONDEBUG(cerr << "DPImageIOPNMBaseC::LoadHeader(): ID String '" << inb << "'\n");
      return false;
    }
    header = inb[1];
    switch(header) 
      {
      case '1': // PBM ascii
      case '4': // PBM raw
      case '2': // PGM ascii
      case '5': // PGM raw
      case '3': // PPM ascii 
      case '6': // PPM raw
	break;
      default:
	ONDEBUG(cerr << "LoadHeader: Unrecognised image type (" << header << ").\n");
	return false;// Unrecognied header type.
      }
    if(inb[2] == '#')
      SkipComment(inf);
    else {
      if(!isspace(inb[2])) {
	ONDEBUG(cerr << "LoadHeader: Unrecognised image format, illegal value after identifier (" << ((int) inb[0]) << ").\n");
	return false; // Illegal format.
      }
    }
    if(!GetNumber(inf,x)) {
      ONDEBUG(cerr << "LoadHeader: Failed to get X size.\n");
      return false;
    }
    if(!GetNumber(inf,y)) {
      ONDEBUG(cerr << "LoadHeader: Failed to get Y size.\n");
      return false;
    }
 
    if (header != '4' && header != '1') // dont do this step for PBM files !  
      {
	// Discard maxval.
	IntT t;
	if(!GetNumber(inf,t)) {
	  ONDEBUG(cerr << "LoadHeader: Failed to get max value.\n");
	  return false;
	}
}
    return true;
  }
  
  //: Skip rest of comment.
  
  bool DPImageIOPNMBaseC::SkipComment(IStreamC &inf) {
    char let;
    while(inf.good()) {
      inf.get(let);
      if(let == '\n')
	return true;
    }
    // End of file ?
    ONDEBUG(cerr << "SkipComment: End of file before end of comment. \n");
    return false;
  }
  
  //: Skip white space.
  
  char DPImageIOPNMBaseC::SkipWhite(IStreamC &inf) {
    char let = 0;
    while(inf.good()) {
      inf.get(let);
      if(let == '#') {
	SkipComment(inf);
	continue;
      }
      if(!isspace(let))
	break;
    }
    return let;
  }
  
  //: Get a number.
  
  bool DPImageIOPNMBaseC::GetNumber(IStreamC &inf,IntT &dig) {
    char let;
    char buff[32]; // Far longer than needed.
    // Skip spaces, comments etc. find first digit.
    // Got a digit.
    let = SkipWhite(inf);
    if(!isdigit(let)) {
      ONDEBUG(cerr << "GetNumber: Illegal leading char: " << ((int) let) << " \n");
      return false; // Illegal fromat.
    }
    buff[0] = let;
    IntT place = 1;
    while(inf.good()) {
      if(place > 31) {
	ONDEBUG(cerr << "GetNumber: Number too long. \n");
	return false; // Number too long.
      }
      inf.get(let);
      if(!isdigit(let)) //Must be a space or a comment.
	break;
      buff[place++] = let;
    }
    buff[place++] = 0;
    // Get number & finish up.
    dig = atoi(buff);
    if(let == '#')
      SkipComment(inf); // Make sure we skip comment.
    return true;
  }
  
  //: Read next bit from pbm
  
  bool DPImageIOPNMBaseC::GetBit(IStreamC &inf,ByteT  &into) {
    char let;
    while(inf.good()) {
      inf.get(let);
      if(let == '0') {
	into = 0;
	return true;
      }
      if(let == '1') {
	into = (char) ((unsigned char)255);
	return true;
      }
      if(isspace(let))
	continue;
      if(let != '#') // The only thing it can be now is a comment.
	break;
      SkipComment(inf);
    }
    // Load failed.
    return false;
  }
  
  ///////////////////////////////////////
  
  //: Constructor from filename.
  
  DPOImagePNMByteRGBBodyC::DPOImagePNMByteRGBBodyC(const StringC & fn)
    : outf(fn,true),
      done(false)
  {}
  
  
  //: Constructor from stream.
  
  DPOImagePNMByteRGBBodyC::DPOImagePNMByteRGBBodyC(const OStreamC &strm)
    : outf(strm),
      done(false)
  {}
  
  
  //: Is port ready for data ?
  
  bool DPOImagePNMByteRGBBodyC::IsPutReady() const 
  { return outf && !done;  }
  
  //: Write out image.
  
  bool DPOImagePNMByteRGBBodyC::Put(const ImageC<ByteRGBValueC> &nimg) 
  {
    ImageC<ByteRGBValueC> img(nimg);
    outf << "P6\n";
    outf << img.Cols() << " " << img.Rows() << "\n";
    outf << "255\n";
    if(sizeof(ByteRGBValueC) == 3)
    {
      // Packing is not a problem.
      for(IndexC i = img.TRow();i <= img.BRow();i++) 
	outf.write((const char *)img.Row(i),img.Cols() * sizeof(ByteRGBValueC));
    } 
    else 
    {
      // This may be slow but it avoids problems with structs being byte padded.
      for(Array2dIterC<ByteRGBValueC> it(img);it.IsElm();it.Next())
	outf.write((const char *) &it.Data(),3);   
    }
    
    //outf << flush; // Make sure is available ASAP.
    //flush(outf);
    outf.os().flush();

    done = true;
    return true;
  }
  
  ///////////////////////////////////////
  
  //: Constructor from filename.
  
  DPIImagePNMByteRGBBodyC::DPIImagePNMByteRGBBodyC(const StringC & fn)
    : inf(fn,true),
      done(false)
  {}

  //: Constructor from stream.
  
  DPIImagePNMByteRGBBodyC::DPIImagePNMByteRGBBodyC(const IStreamC &in)
    : inf(in),
      done(false)
  {}

  //: Is valid data ?
  
  
  bool DPIImagePNMByteRGBBodyC::IsGetEOS() const
  { return (done || (!inf)) ; }
  
  //: Is some data ready ?
  // true = yes.
  
  bool DPIImagePNMByteRGBBodyC::IsGetReady() const 
  { return (done || (!inf)); }
  

  //: Get image from stream.
  
  bool DPIImagePNMByteRGBBodyC::Get(ImageC<ByteRGBValueC> &buff) {
    if(done || (!inf))
      return false;
    try {
      buff = DPIImagePNMByteRGBBodyC::Get();
    } catch(DataNotReadyC &) {
      return false;
    }
    return true;
  }
  
  //: Get next piece of data.
  
  ImageC<ByteRGBValueC> DPIImagePNMByteRGBBodyC::Get() {
    IntT x,y;
    ByteT type;
    if(done)
      throw DataNotReadyC("ByteRGB: Image already read. ");
    if(!LoadHeader(inf,type,x,y)) {
      // Throw an exception ??
      std::cerr << "DPIImagePNMByteRGBBodyC: Failed to load header for image '" << inf.Name() << "'\n";
      throw DataNotReadyC("DPIImagePNMByteRGBBodyC: Failed to load header for image. ");
    }
    ImageC<ByteRGBValueC> img(y,x);
    switch(type) 
      {
      case '1': // ASCII PBM
	{
	  ByteT bit;
	  for(Array2dIterC<ByteRGBValueC> it(img);it.IsElm();it.Next()) {
	    if(!GetBit(inf,bit))
	      return ImageC<ByteRGBValueC>(); // Read FAILED !
	    it.Data().Set(bit,bit,bit);
	  }
	}
	break;
      case '2': // ASCII PNMByteGrey
	{
	  for(Array2dIterC<ByteRGBValueC> it(img);it.IsElm();it.Next()) {
	    IntT t;
	    if(!GetNumber(inf,t))
	      return ImageC<ByteRGBValueC>(); // Read FAILED !
	    if(t < 0)
	      t = 0;
	    if(t > 255)
	      t = 255;
	    it.Data().Set(t,t,t);
	  }
	}
	break;            
      case '3': // ASCII
	{
	  for(Array2dIterC<ByteRGBValueC> it(img);it.IsElm();it.Next()) {
	    int t;
	    if(!GetNumber(inf,t))
	      return ImageC<ByteRGBValueC>(); // Read FAILED !
	    if(t < 0)
	      t = 0;
	    if(t > 255)
	      t = 255;
	    it.Data().Red() = t;
	    if(!GetNumber(inf,t))
	      return ImageC<ByteRGBValueC>(); // Read FAILED !
	    if(t < 0)
	      t = 0;
	    if(t > 255)
	      t = 255;
	    it.Data().Green() = t;
	    if(!GetNumber(inf,t))
	      return ImageC<ByteRGBValueC>(); // Read FAILED !
	    if(t < 0)
	      t = 0;
	    if(t > 255)
	      t = 255;
	    it.Data().Blue() = t;
	  }
	}
	break;
      case '5': // Binary PNMByteGrey
	{
	  char buff;
	  for(Array2dIterC<ByteRGBValueC> it(img);it.IsElm();it.Next()) {
	    inf.read(&buff,1);
	    if(inf.good())
	      return ImageC<ByteRGBValueC>(); // Read FAILED !
	    it.Data().Set(buff,buff,buff);
	  }
	}  //: register mono 8 bit unsigned type 

	break;
      case '6': // Binary
	{
	  if(sizeof(ByteRGBValueC) == 3) {
	    // Packing is not a problem.
	    for(IndexC i = img.TRow();i <= img.BRow();i++) 
	      inf.read((char *)img.Row(i),img.Cols() * sizeof(ByteRGBValueC));
	  } else {
	    // This may be slow but it avoids problems with structs being byte padded.
	    for(Array2dIterC<ByteRGBValueC> it(img);it.IsElm();it.Next())
	      inf.read((char *)&it.Data(),3); 
	  }
	}
	break;
      default:
	//: Can't handle this image type.
	// Throw an exception ??
	ONDEBUG(cerr << "ByteRGB: Unrecognied format " << ((IntT) type) << " (" << type << ")\n");
	return ImageC<ByteRGBValueC>();
      }
    done = true;
    return img;
  }
  
  ///////////////////////////////////////
  
  //: Constructor from filename.
  
  DPIImagePNMByteGreyBodyC::DPIImagePNMByteGreyBodyC(const StringC & fn)
    : inf(fn,true), // Open Binary stream.
      done(false)
  {
#if RAVL_CHECK
    if(!inf) {
      std::cerr << "DPIImagePNMByteGreyBodyC::DPIImagePNMByteGreyBodyC(), Failed to open file:" << fn << std::endl;
    }
#endif
  }
  
  DPIImagePNMByteGreyBodyC::DPIImagePNMByteGreyBodyC(const IStreamC &strm)
    : inf(strm),
      done(false)
  {}
  
  //: Is valid data ?
  
  bool DPIImagePNMByteGreyBodyC::IsGetEOS() const
  { return (done || (!inf)) ; }

  //: Is some data ready ?
  // true = yes.
  
  bool DPIImagePNMByteGreyBodyC::IsGetReady() const 
  { return (done || (!inf)); }


  //: Get image from stream.
  
  bool DPIImagePNMByteGreyBodyC::Get(ImageC<ByteT> &buff) {
    if(done || (!inf))
      return false;
    try {
      buff = DPIImagePNMByteGreyBodyC::Get();
    } catch(DataNotReadyC &) {
      return false;
    }
    return true;
  }
  
  //: Get next piece of data.
  
  ImageC<ByteT> DPIImagePNMByteGreyBodyC::Get() {
    IntT x,y;
    ByteT type;  
    if(done)
      throw DataNotReadyC("DPIImagePNMByteGreyBodyC: Image already read. ");
    if(!LoadHeader(inf,type,x,y)) {
      // Throw an exception ??
      std::cerr << "PNMByteGrey: Failed to load header for image '" << inf.Name() << "'\n";
      throw DataNotReadyC("DPIImagePNMByteGreyBodyC: Failed to load header. ");
      //return ImageC<ByteT>();
    }
    ImageC<ByteT> img(y,x);
    switch(type) 
      {
      case '1': // ASCII PBM
	{
	  for(Array2dIterC<ByteT> it(img);it.IsElm();it.Next()) {
	    if(!GetBit(inf,it.Data()))
	      return ImageC<ByteT>(); // Read FAILED !
	  }
	}
	break;
      case '2': // ASCII PNMByteGrey
	{
	  for(Array2dIterC<ByteT> it(img);it.IsElm();it.Next()) {
	    IntT t;
	    if(!GetNumber(inf,t))
	      return ImageC<ByteT>(); // Read FAILED !
	    if(t < 0)
	      t = 0;
	    if(t > 255)
	      t = 255;
	    it.Data() = t;
	  }
	}
	break;      
      case '3': // Conversion from ASCII RGB.
	{
	  for(Array2dIterC<ByteT> it(img);it.IsElm();it.Next()) {
	    int t,total = 0;
	    if(!GetNumber(inf,t))
	      return ImageC<ByteT>(); // Read FAILED !
	    total += t;
	    if(!GetNumber(inf,t))
	      return ImageC<ByteT>(); // Read FAILED !
	    total += t;
	    if(!GetNumber(inf,t))
	      return ImageC<ByteT>(); // Read FAILED !
	    total += t;
	    t = total / 3;
	    if(t < 0)
	      t = 0;
	    if(t > 255)
	      t = 255;
	    it.Data() = t;
	  }
	}
	break;
	case '4': // Binary PBM 
	  {
	    IntT byteNo = Ceil(((RealT) img.Size()) / 8.0) ; // 1 bit per pixel ! 
	    unsigned char * tmp = (unsigned char*)  operator new (byteNo * sizeof(char))  ; // allocate some bytes 
	    inf.read((char*) tmp, byteNo) ;  // read the whole image in one go. 
	    
	    UIntT byte = 0 ; // first byte in file 
	    // go through ravl image 
	    for ( Array2dIterC<ByteT> it(img) ; it.IsElm() ; )  {
	      unsigned char tempory = tmp[byte] ; // cache the byte we are dealing with
	      for ( UIntT shift = 0 ; shift <= 7 ; ++ shift ) // iterate over each bit 
		{
		  if ( (tempory & 128) ) // check value of msb 
		    it.Data() = (UIntT) 0 ; 
		  else  
		    it.Data() = (UIntT) 255 ; 
		  it.Next() ; // next ravl pixel 
		  tempory <<= 1 ; // shift the byte by 1 bit 
		}
	      ++ byte ; // next byte (8 pixels) 
	    }
	    operator delete (tmp) ; 
	  }
	  break ; 

      case '5': // Binary PNMByteGrey
	{
	  // Can read row at time because no packing problems.
	  for(IndexC i = img.TRow();i <= img.BRow();i++) 
	    inf.read((char *) img.Row(i),img.Cols());
	}
	break;
  case '6': // Conversion from RGB.
	{
	  for(Array2dIterC<ByteT> it(img);it.IsElm();it.Next()) {
	    ByteRGBValueC tmp;
	    inf.read((char *)&tmp,3);
	    if(inf.good())
	      return ImageC<ByteT>(); // Read FAILED !
	    it.Data() = (tmp.Red() + tmp.Green() + tmp.Blue())/3;
	  }
	}
	break;
      default:
	//: Can't handle this image type.
	// Throw an exception ??
	cerr << "PNMByteGrey: Unrecognied format " << ((IntT) type) << " (" << type << ")\n";
	return ImageC<ByteT>();
      }
    done = true;
    return img;
  }
  
  ///////////////////////////////////////
  
  //: Constructor from filename.
  
  DPOImagePNMByteGreyBodyC::DPOImagePNMByteGreyBodyC(const StringC & fn)
    : outf(fn,true),
      done(false)
  {}
  
  
  //: Constructor from stream
  
  DPOImagePNMByteGreyBodyC::DPOImagePNMByteGreyBodyC(const OStreamC &strm)
    : outf(strm),
      done(false)
  {}
  
  //: Is port ready for data ?
  
  bool DPOImagePNMByteGreyBodyC::IsPutReady() const
  { return outf && !done;  }
  
  //: Put data.
  
  bool DPOImagePNMByteGreyBodyC::Put(const ImageC<ByteT> &nimg) {
    ImageC<ByteT> img(nimg);
    outf << "P5\n";
    outf << img.Cols() << " " << img.Rows() << "\n";
    outf << "255\n";
    // Can write row at time because no packing problems.
    for(IndexC i = img.TRow();i <= img.BRow();i++) 
      outf.write((const char *) img.Row(i),img.Cols());
    outf.os().flush();
    done = true;
    return true;
  }
  
  ////////////////////////////////////////////
  //: Save a RGB image in ByteRGB format.
  
  DPIImagePNMByteRGBC::DPIImagePNMByteRGBC(const StringC & fn)
    : DPEntityC(*new DPIImagePNMByteRGBBodyC(fn))
  {}
  
  
  DPIImagePNMByteRGBC::DPIImagePNMByteRGBC(const IStreamC &strm)
    : DPEntityC(*new DPIImagePNMByteRGBBodyC(strm))
  {}
  
  
  ////////////////////////////////////////////
  //: Load a RGB image in ByteRGB format.
  
  DPOImagePNMByteRGBC::DPOImagePNMByteRGBC(const StringC & fn)
    : DPEntityC(*new DPOImagePNMByteRGBBodyC(fn))
  {}
  
  
  DPOImagePNMByteRGBC::DPOImagePNMByteRGBC(const OStreamC &strm)
    : DPEntityC(*new DPOImagePNMByteRGBBodyC(strm))
  {}
  
  
  ////////////////////////////////////////////
  //: Load a byte image in PNMByteGrey format.
  
  DPIImagePNMByteGreyC::DPIImagePNMByteGreyC(const StringC & fn)
    : DPEntityC(*new DPIImagePNMByteGreyBodyC(fn))
  {}
  
  DPIImagePNMByteGreyC::DPIImagePNMByteGreyC(const IStreamC &strm)
    : DPEntityC(*new DPIImagePNMByteGreyBodyC(strm))
  {}
  
  
  ////////////////////////////////////////////
  //: Save a byte image in PNMByteGrey format.
  
  DPOImagePNMByteGreyC::DPOImagePNMByteGreyC(const StringC & fn)
    : DPEntityC(*new DPOImagePNMByteGreyBodyC(fn))
  {}
  
  DPOImagePNMByteGreyC::DPOImagePNMByteGreyC(const OStreamC &strm)
    : DPEntityC(*new DPOImagePNMByteGreyBodyC(strm))
  {}
}

