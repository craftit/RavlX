// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! lib=RavlExtImgIO
//! file="Ravl/Image/ExternalImageIO/Tiff/ImgIOTiff.cc"

#include "Ravl/Image/ImgIOTiff.hh"
#include "Ravl/Image/ImgIOTiffB.hh"
#include "Ravl/Array2dIter.hh"
#include <ctype.h>

#define DPDEBUG 0
#if DPDEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlImageN {

  //: Constructor.
  
  DPImageIOTIFFBaseC::DPImageIOTIFFBaseC()
    : tif(0)
  {}
  
  //: Destructor.
  
  DPImageIOTIFFBaseC::~DPImageIOTIFFBaseC() {
    if(tif != 0) {
      TIFFClose(tif);
      ONDEBUG(tif = 0);
    }
  }
  
  ///////////////////////////////////////
  
  //: Constructor from filename.
  
  DPOImageTIFFByteRGBABodyC::DPOImageTIFFByteRGBABodyC(StringC fn)
    : done(false)
  {
    ONDEBUG(std::cerr << "DPOImageTIFFByteRGBABodyC(), Open file '" << fn << "' \n");
    tif = TIFFOpen(fn.chars(),"w");
  }
  
  //: Constructor from stream.
  
  DPOImageTIFFByteRGBABodyC::DPOImageTIFFByteRGBABodyC(const OStreamC &strm)
    : outf(strm),
      done(false)
  {
    ONDEBUG(std::cerr << "DPOImageTIFFByteRGBABodyC(), Open stream '" << strm.Name() << "' \n");
    tif = TIFFClientOpen(outf.Name().chars(),"w",
			 (thandle_t)this,
			 &TIFFReadProc, &TIFFWriteProc,
			 &TIFFSeekProc, &TIFFCloseProc,
			 &TIFFSizeProc,
			 &TIFFMapFileProc, &TIFFUnmapFileProc
			 );
  }
  
  //: Is port ready for data ?
  
  bool DPOImageTIFFByteRGBABodyC::IsPutReady() const { 
    return tif != 0 && !done;  
  }

  //: Write out image.
  
  bool DPOImageTIFFByteRGBABodyC::Put(const ImageC<ByteRGBAValueC> &img)
  {
    uint32 w = img.Cols();
    uint32 h = img.Rows();
  
    ONDEBUG(std::cerr << "DPIImageTIFFByteRGBABodyC::Put(), Called. " << w << " " << h << " \n");
    
    if (tif == 0) 
      return false;
    
    uint32 rowsperstrip = (uint32) -1;
    
    TIFFSetField(tif, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, w);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, h);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 4);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE,8);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP,
		 TIFFDefaultStripSize(tif,rowsperstrip));
    TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_LZW);
    
    const IndexC offset = img.LCol();
    
    ONDEBUG(std::cerr << "DPIImageTIFFByteRGBABodyC::Put(), TRow:" << img.TRow() << " BRow:" << img.BRow() << " . \n");
    for (IndexC row = img.TRow(); row <= img.BRow(); row++)
      TIFFWriteScanline(tif,(char *) &(img[row][offset]), row.V(),0);
    
    TIFFFlush(tif);
    done = true;
    return true;
  }
  
  
  // Read some data.
  
  tsize_t DPOImageTIFFByteRGBABodyC::TIFFReadProc(thandle_t dptr, tdata_t data, tsize_t noBytes) {
    std::cerr << "DPOImageTIFFByteRGBABodyC::TIFFWriteProc(), WARNING: read called on a write-only stream. \n";
    RavlAssert(0);
    return 0;
  }
  
  tsize_t DPOImageTIFFByteRGBABodyC::TIFFWriteProc(thandle_t dptr, tdata_t data, tsize_t noBytes) {
    DPOImageTIFFByteRGBABodyC &body = *(DPOImageTIFFByteRGBABodyC *) dptr;
    body.outf.write((const char *) data,noBytes);
    return noBytes;
  }
  
  toff_t  DPOImageTIFFByteRGBABodyC::TIFFSeekProc(thandle_t dptr, toff_t off, int whence) {
    DPOImageTIFFByteRGBABodyC &body = *(DPOImageTIFFByteRGBABodyC *) dptr;
    switch(whence)
      {
      case SEEK_SET: body.outf.seekp(off,ios::beg); break;
      case SEEK_CUR: if(off != 0) body.outf.seekp(off,std::ios::cur); break;
      case SEEK_END: body.outf.seekp(off,ios::end); break;
      default:
	std::cerr << "DPOImageTIFFByteRGBABodyC::TIFFSeekProc(), WARNING: Illegal value for whence : " << whence << "\n";
	break;
      }
    return body.outf.Tell();
  }
  
  int DPOImageTIFFByteRGBABodyC::TIFFCloseProc(thandle_t dptr) {
    DPOImageTIFFByteRGBABodyC &body = *((DPOImageTIFFByteRGBABodyC *) dptr);
    body.outf.os() << std::flush;
    // We don't actuall close there.
    return 1;
  }
  
  toff_t DPOImageTIFFByteRGBABodyC::TIFFSizeProc(thandle_t dptr) {
    DPOImageTIFFByteRGBABodyC &body = *((DPOImageTIFFByteRGBABodyC *) dptr);
    // Not very quick, but it should work...
    toff_t at = body.outf.Tell();
    body.outf.seekp(0,std::ios::end);
    toff_t ret = body.outf.Tell();
    body.outf.seekp(at,std::ios::beg);
    return ret;
  }

  int DPOImageTIFFByteRGBABodyC::TIFFMapFileProc(thandle_t dptr, tdata_t*, toff_t*) {
    return 0;
  }
  
  void DPOImageTIFFByteRGBABodyC::TIFFUnmapFileProc(thandle_t dptr, tdata_t, toff_t) {
  }
  
  ///////////////////////////////////////
  
  //: Constructor from filename.
  
  DPIImageTIFFByteRGBABodyC::DPIImageTIFFByteRGBABodyC(StringC fn)
  : done(false)
  {
    ONDEBUG(std::cerr << "DPIImageTIFFByteRGBABodyC(), Open file '" << fn << "' \n");
    tif = TIFFOpen(fn.chars(),"r");
  }
  
  
  //: Constructor from stream.
  
  DPIImageTIFFByteRGBABodyC::DPIImageTIFFByteRGBABodyC(const IStreamC &in)
    : inf(in),
      done(false)
  {
    ONDEBUG(std::cerr << "DPIImageTIFFByteRGBABodyC(), Open std::istream \n");
    tif = TIFFClientOpen(inf.Name().chars(),"r",
			 (thandle_t)this,
			 &TIFFReadProc, &TIFFWriteProc,
			 &TIFFSeekProc, &TIFFCloseProc,
			 &TIFFSizeProc,
			 &TIFFMapFileProc, &TIFFUnmapFileProc
			 );
    
  }
  
  //: Is valid data ?
  
  bool DPIImageTIFFByteRGBABodyC::IsGetEOS() const { 
    return (done || tif == 0) ; 
  }
  
  //: Get next piece of data.
  
  ImageC<ByteRGBAValueC> DPIImageTIFFByteRGBABodyC::Get()
  {
    ONDEBUG(std::cerr << "DPIImageTIFFByteRGBABodyC::Get(), Called. \n");

    ImageC<ByteRGBAValueC> img;
    if (tif == 0) 
      throw DataNotReadyC("DPIImageTIFFByteRGBABodyC: Not file handle. ");

    size_t npixels;
    uint32* raster;

#if 0
    uint32 w, h;
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &w);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &h);
    npixels = w * h;
    raster = (uint32*) _TIFFmalloc(npixels * sizeof (uint32));
    if (raster == NULL)
      throw DataNotReadyC("DPIImageTIFFByteRGBABodyC: Allocation failed.. ");
    
    if (!TIFFReadRGBAImage(tif, w, h, raster, 0)) {
      throw DataNotReadyC("DPIImageTIFFByteRGBABodyC: Read failed. ");
    img = ImageC<ByteRGBAValueC>(h,w,BufferRC<ByteRGBAValueC>((ByteRGBAValueC *) raster,true),true);
#else

    TIFFRGBAImage tiffimg;
    char emsg[1024];
    
    if (!TIFFRGBAImageBegin(&tiffimg, tif, 0, emsg)) 
    {
      throw DataNotReadyC("DPIImageTIFFByteRGBABodyC: ImageBegin failed.. ");
    }

    npixels = tiffimg.width * tiffimg.height;
    UIntT allocSize = npixels * sizeof (uint32);
    raster = (uint32*) _TIFFmalloc(allocSize);
    if (raster == 0)
    {
      TIFFRGBAImageEnd(&tiffimg);
      throw DataNotReadyC("DPIImageTIFFByteRGBABodyC: Allocation failed.. ");
    }
    
    if (!TIFFRGBAImageGet(&tiffimg, raster, tiffimg.width, tiffimg.height)) 
    {
      _TIFFfree(raster);
      TIFFRGBAImageEnd(&tiffimg);
      throw DataNotReadyC("DPIImageTIFFByteRGBABodyC: Allocation failed.. ");
    }

    // Allocate whole image buffer, and copy from file data buffer
    // FIXME: this is very inefficient - should read straight from file to image
    img = ImageC<ByteRGBAValueC>(tiffimg.height, tiffimg.width);

    ONDEBUG(std::cerr << "DPIImageTIFFByteRGBABodyC::Get(), Orientation : " << tiffimg.orientation << "\n");
    switch(tiffimg.orientation)
    {
      default:
      {
	std::cerr << "DPIImageTIFFByteRGBABodyC::Get(), WARNING Unexpected orientation : " << tiffimg.orientation << "\n";
      }
      /* no break */
      case ORIENTATION_BOTLEFT:
      {
#ifndef __sgi__
	_TIFFmemcpy(img.Row(0), raster, allocSize);
#else
	UIntT h = tiffimg.height;
	UIntT w = tiffimg.width;
	UIntT hw = h*w;
	uint32* buff = (uint32*)img.Row(0);

	for (UIntT y = 0; y < hw; y+=w)
	{
	  for (UIntT x = 0; x < w; x++)
	  {
	    uint32& p = raster[y+x];
	    buff[y+x] = 
	      ((p & 0xFF000000) >> 24) |
	      ((p & 0x00FF0000) >>  8) |
	      ((p & 0x0000FF00) <<  8) |
	      ((p & 0x000000FF) << 24);
	  }
	}
#endif
	break;
      }
      case ORIENTATION_TOPLEFT:
      {
	UIntT h = tiffimg.height;
	UIntT w = tiffimg.width;
	UIntT hw = h*w;
	uint32* buff = (uint32*)img.Row(0);

#ifndef __sgi__
	for (UIntT y = 0, ny = hw-w; y < hw; y+=w, ny-=w)
	  for (UIntT x = 0; x < w; x++)
	    buff[y+x] = raster[ny+x];
#else
        for (UIntT y = 0, ny = hw-w; y < hw; y+=w, ny-=w)
	{
	  for (UIntT x = 0; x < w; x++)
	  {
	    uint32& p = raster[ny+x];
	    buff[y+x] =
	      ((p & 0xFF000000) >> 24) |
	      ((p & 0x00FF0000) >>  8) |
	      ((p & 0x0000FF00) <<  8) |
	      ((p & 0x000000FF) << 24);
	  }
	}
#endif
	break;
      } 
    }

    TIFFRGBAImageEnd(&tiffimg);
#endif
    _TIFFfree(raster);
    done = true;
    return img;
  }
  
  // Read some data.
  
  tsize_t DPIImageTIFFByteRGBABodyC::TIFFReadProc(thandle_t dptr, tdata_t data, tsize_t noBytes) {
    DPIImageTIFFByteRGBABodyC &body = *(DPIImageTIFFByteRGBABodyC *) dptr;
    body.inf.read((char *) data,noBytes);
    return body.inf.gcount();
  }
  
  tsize_t DPIImageTIFFByteRGBABodyC::TIFFWriteProc(thandle_t dptr, tdata_t, tsize_t) {
    std::cerr << "DPIImageTIFFByteRGBABodyC::TIFFWriteProc(), WARNING: Write called on a read-only stream. \n";
    RavlAssert(0);
    return 0;
  }
  
  toff_t  DPIImageTIFFByteRGBABodyC::TIFFSeekProc(thandle_t dptr, toff_t off, int whence) {
    DPIImageTIFFByteRGBABodyC &body = *(DPIImageTIFFByteRGBABodyC *) dptr;
    switch(whence)
      {
      case SEEK_SET: body.inf.seekg(off,std::ios::beg); break;
      case SEEK_CUR: if(off != 0) body.inf.seekg(off,std::ios::cur); break;
      case SEEK_END: body.inf.seekg(off,std::ios::end); break;
      default:
	std::cerr << "DPIImageTIFFByteRGBABodyC::TIFFSeekProc(), WARNING: Illegal value for whence : " << whence << "\n";
	break;
      }
    return body.inf.Tell();
  }
  
  int DPIImageTIFFByteRGBABodyC::TIFFCloseProc(thandle_t dptr) {
    //DPIImageTIFFByteRGBABodyC &body = *((DPIImageTIFFByteRGBABodyC *) dptr);
    //  return body.inf.is() << flush;   
    // We don't actuall close there.
    return 1;
  }
  
  toff_t DPIImageTIFFByteRGBABodyC::TIFFSizeProc(thandle_t dptr) {
    DPIImageTIFFByteRGBABodyC &body = *((DPIImageTIFFByteRGBABodyC *) dptr);
    // Not very quick, but it should work...
    toff_t at = body.inf.Tell();
    body.inf.seekg(0,ios::end);
    toff_t ret = body.inf.Tell();
    body.inf.seekg(at,ios::beg);
    return ret;
  }
  
  int DPIImageTIFFByteRGBABodyC::TIFFMapFileProc(thandle_t dptr, tdata_t*, toff_t*) {
    return 0;
  }
  
  void DPIImageTIFFByteRGBABodyC::TIFFUnmapFileProc(thandle_t dptr, tdata_t, toff_t) {
  }
  
  
  ////////////////////////////////////////////
  //: Save a RGBA image in ByteRGBA format.
  
  DPIImageTIFFByteRGBAC::DPIImageTIFFByteRGBAC(StringC fn)
    : DPEntityC(*new DPIImageTIFFByteRGBABodyC(fn))
  {}
  
  DPIImageTIFFByteRGBAC::DPIImageTIFFByteRGBAC(const IStreamC &strm)
    : DPEntityC(*new DPIImageTIFFByteRGBABodyC(strm))
  {}
  
  ////////////////////////////////////////////
  //: Load a RGBA image in ByteRGBA format.
  
  DPOImageTIFFByteRGBAC::DPOImageTIFFByteRGBAC(StringC fn)
    : DPEntityC(*new DPOImageTIFFByteRGBABodyC(fn))
  {}
  
  
  DPOImageTIFFByteRGBAC::DPOImageTIFFByteRGBAC(const OStreamC &strm)
    : DPEntityC(*new DPOImageTIFFByteRGBABodyC(strm))
  {} 
}

