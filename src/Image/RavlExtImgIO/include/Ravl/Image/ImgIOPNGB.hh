// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef DPIMAGEIOPNG_HEADER
#define DPIMAGEIOPNG_HEADER
///////////////////////////////////////////////////
//! file="Ravl/Image/ExternalImageIO/PNG/ImgIOPNGB.hh"
//! lib=RavlExtImgIO
//! docentry="Ravl.API.Images.IO.Formats"
//! author="Charles Galambos"
//! userlevel=Develop
//! date="29/10/98"

#include <png.h>

 /* The png_jmpbuf() macro, used in error handling, became available in
  * libpng version 1.0.6.  If you want to be able to run your code with older
  * versions of libpng, you must define the macro yourself (but only if it
  * is not already defined by libpng!).
  */

#ifndef png_jmpbuf
#  define png_jmpbuf(png_ptr) ((png_ptr)->jmpbuf)
#endif

#include "Ravl/DP/FileFormat.hh"
#include "Ravl/DP/Port.hh"
#include "Ravl/Stream.hh"
#include "Ravl/Image/ImgIOPNG.hh"

namespace RavlImageN {


  static inline bool IsLittleEndian()
  {
    short test = 0x4321;
    if(((char *) &test)[0] != 0x21) 
      return false;  
    return (((char *) &test)[1] == 0x43);
  }
  
  //: PNG Utilities

  class DPImageIOPNGBaseC {
  public:    
    DPImageIOPNGBaseC();
    //: Constructor.
    
    virtual ~DPImageIOPNGBaseC();
    //: Destructor.
    
    
  protected:
    
    png_structp png_ptr;
    png_infop info_ptr;
    
    ImageRectangleC imgRect; // Current image size.
    
    int bit_depth, color_type, interlace_type;
    int interlacing ;
    
    bool done;
    bool initalised;
  };
  
  class DPIImageIOPNGBaseC 
    : public DPImageIOPNGBaseC
  {
  public:    
    DPIImageIOPNGBaseC(const IStreamC &fin);
    //: Constructor
    
    virtual ~DPIImageIOPNGBaseC();
    //: Destructor.
    
    bool ReadHeaderInfo(int &bit_depth,int &colourType,int &interlace);
    //: Get information about a png file.
    
  protected:
    bool ReadHeader(const std::type_info &ti);
    //: Read header information.
    // and setup read.
    
    IStreamC fin;
    
    // Info about the current image.
    
  };
  
  class DPOImageIOPNGBaseC
    : public DPImageIOPNGBaseC
  {
  public:
    DPOImageIOPNGBaseC(const OStreamC &fout);
    //: Constructor.
    
    virtual ~DPOImageIOPNGBaseC();
    //: Destructor.
    
  protected:
    bool WriteHeader(const std::type_info &ti,const ImageRectangleC &nimgRect);
    //: Write header information.
    
    OStreamC fout;
  };

  //: Body class for loading a PNG image.
  
  template<class PixelT>
  class DPIImagePNGBodyC : 
    public DPIPortBodyC<ImageC<PixelT> >,
    public DPIImageIOPNGBaseC
  {
  public:
    DPIImagePNGBodyC(StringC fn);
    //: Constructor from filename.
    
    DPIImagePNGBodyC(const IStreamC &in);
    //: Constructor from stream.
    // Not supported at the moment...
    
    virtual bool IsGetEOS() const;
    //: Is valid data ?
    
    virtual ImageC<PixelT> Get();
    //: Get next piece of data.
    
  protected:
    bool Read(ImageC<PixelT> &img);
    //: Read an image.
  };
  
  //: Save a  image.
  
  template<class PixelT>
  class DPOImagePNGBodyC : 
    public DPOPortBodyC<ImageC<PixelT> >,
    public DPOImageIOPNGBaseC
  {
  public:
    DPOImagePNGBodyC(StringC fn);
    //: Constructor from filename.
    
    DPOImagePNGBodyC(const OStreamC &strm);
    //: Constructor from filename.
    // Not supported at the moment...
    
    virtual bool Put(const ImageC<PixelT> &dat);
    //: Put data.
    
    virtual bool IsPutReady() const ;
    //: Is port ready for data ?
    
  protected:
    bool Write(const ImageC<PixelT> &img);
    //: Write an image.
  };
  
  
  //// DPIImagePNGBodyC /////////////////////////////////////////////
  
  //: Constructor from filename.
  
  template<class PixelT>
  DPIImagePNGBodyC<PixelT>::DPIImagePNGBodyC(StringC nfn) 
    : DPIImageIOPNGBaseC(IStreamC(nfn))
  {}
  
  //: Constructor from stream.
  
  
  template<class PixelT>
  DPIImagePNGBodyC<PixelT>::DPIImagePNGBodyC(const IStreamC &in)
    : DPIImageIOPNGBaseC(in)
  {}

  //: Is valid data ?
  
  template<class PixelT>
  bool DPIImagePNGBodyC<PixelT>::IsGetEOS() const 
  { return done || (!fin.good()); }
  
  
  //: Get next piece of data.
  
  
  template<class PixelT>
  ImageC<PixelT> DPIImagePNGBodyC<PixelT>::Get() {
    if(done)
      return ImageC<PixelT>();
    done = true;
    if(!fin.good()) {
      std::cerr << "DPIImagePNGByteRGBBodyC<PixelT>::Get(), ERROR: Bad stream. \n";
      return ImageC<PixelT>();
    }
    ImageC<PixelT> ret;
    if(!Read(ret)) 
      std::cerr << "DPIImagePNGByteRGBBodyC<PixelT>::Get(), Error in file. \n";
    return ret;
  }
  
  
  //: Read an image.
  
  template<class PixelT>
  bool DPIImagePNGBodyC<PixelT>::Read(ImageC<PixelT> &dat) {
    if(!ReadHeader(typeid(PixelT)))
      return false;
    
    /* Set error handling if you are using the setjmp/longjmp method (this is
     * the normal method of doing things with libpng).  REQUIRED unless you
     * set up your own error handlers in the png_create_read_struct() earlier.
     */
    
    if (setjmp(png_jmpbuf(png_ptr))) 
      return false;
    
    /* If we have already read some of the signature */
    //png_set_sig_bytes(png_ptr, sig_read);
    
    
    /* Turn on interlace handling.  REQUIRED if you are not using
     * png_read_image().  To see how to handle interlacing passes,
     * see the png_read_row() method below:
     */
    int number_passes = png_set_interlace_handling(png_ptr);
    
    /* Optional call to gamma correct and add the background to the palette
     * and update info structure.  REQUIRED if you are expecting libpng to
     * update the palette for you (ie you selected such a transform above).
     */
    //png_read_update_info(png_ptr, info_ptr);
    
    /* Allocate the memory to hold the image using the fields of info_ptr. */
    
    png_bytep row_pointers[1];
    // It seems we need a few extra rows to avoid memory corruption.
    // similar to jpeg, suspicious!
    
    dat = ImageC<PixelT>(IndexRange2dC(imgRect.Origin(),imgRect.End() + Index2dC(2,0)));
    dat = ImageC<PixelT>(dat,imgRect);
    
    //ONDEBUG(std::cerr << "Width:" << dat.Cnum() << " Height:" << dat.Rnum() << " passes:" << number_passes << " \n");
    // Check format...
    //ONDEBUG(std::cerr << "Row bytes " << png_get_rowbytes(png_ptr, info_ptr) << " Expected:" << (width * 3) << "\n");
    
    if (bit_depth == 16 && IsLittleEndian())
      png_set_swap(png_ptr);

    /* Now it's time to read the image. */
    
    const UIntT height = dat.Rows();
    const IndexC LCol = dat.LCol();
    for (int pass = 0; pass < number_passes; pass++) {
      IndexC crow = dat.TRow();
      for (unsigned int y = 0; y < height; y++) {
	row_pointers[0] = (png_bytep)(&dat[crow++][LCol]);
	png_read_rows(png_ptr, row_pointers, NULL, 1);
      }
    }
    
    /* read rest of file, and get additional chunks in info_ptr - REQUIRED */
    png_read_end(png_ptr, info_ptr);
    
    /* At this point you have read the entire image */
    
    return true;
  }
  
  //// DPOImagePNGByteRGBBodyC /////////////////////////////////////////////
  
  //: Constructor from filename.
  
  template<class PixelT>
  DPOImagePNGBodyC<PixelT>::DPOImagePNGBodyC(StringC nfn)
    : DPOImageIOPNGBaseC(nfn)
  {}
  
  //: Constructor from filename.
  
  template<class PixelT>
  DPOImagePNGBodyC<PixelT>::DPOImagePNGBodyC(const OStreamC &strm) 
    : DPOImageIOPNGBaseC(strm)
  {}
  
  //: Put data.
  
  template<class PixelT>
  bool DPOImagePNGBodyC<PixelT>::Put(const ImageC<PixelT> &dat) {
    if(done)
      return false;
    done = true;
    if(!fout.good()) {
      std::cerr << "DPIOImagePNGBodyC<PixelT>::Get(), ERROR: bad stream.\n";
      return false;
    }
    return Write(dat);
  }
  
  //: Is port ready for data ?
  
  template<class PixelT>
  bool DPOImagePNGBodyC<PixelT>::IsPutReady() const { 
    return !done && fout.good(); 
  }
  
  //: Write an image.
  
  template<class PixelT>
  bool DPOImagePNGBodyC<PixelT>::Write(const ImageC<PixelT> &dat) {
    
    if(!WriteHeader(typeid(PixelT),dat.Rectangle()))
      return false;
    
    /* Set error handling. */
    
    if (setjmp(png_jmpbuf(png_ptr)))
      return false;
    
    /* turn on interlace handling if you are not using png_write_image() */
    
    int number_passes;
    if (interlacing)
      number_passes = png_set_interlace_handling(png_ptr);
    else
      number_passes = 1;
    
    if (bit_depth == 16 && IsLittleEndian())
      png_set_swap(png_ptr);

    png_bytep row_pointers[1];
    
    /* The number of passes is either 1 for non-interlaced images,
     * or 7 for interlaced images.
     */
    const IndexC LCol = dat.LCol();
    const UIntT height = dat.Rows();
    IndexC crow = dat.TRow();
    for (int pass = 0; pass < number_passes; pass++) {
      for (unsigned int y = 0; y < height; y++) {
	row_pointers[0] = (png_bytep)  (& (dat[crow++][LCol]));
	png_write_rows(png_ptr, &row_pointers[0], 1);
      }
    }
    
    /* It is REQUIRED to call this to finish writing the rest of the file */
    png_write_end(png_ptr, info_ptr);
    
    /* that's it */
    return true;
  }

  //: Load an image
  // Use via function found in amma/StdType/DataProc/FileFormatIO.hh
  
  template<class PixelT>
  class DPIImagePNGC 
    :  public DPIPortC<ImageC<PixelT> >
  {
  public:
    DPIImagePNGC(StringC fn)
      : DPEntityC(*new DPIImagePNGBodyC<PixelT>(fn))
      {}
    //: Constructor from filename.  
    
    DPIImagePNGC(const IStreamC &strm)
    : DPEntityC(*new DPIImagePNGBodyC<PixelT>(strm))
      {}
    //: Constructor from stream 
  };
  
  //: Save an image
  
  template<class PixelT>
  class DPOImagePNGC
    : public DPOPortC<ImageC<PixelT> >
  {
  public:
    DPOImagePNGC(StringC fn)
      : DPEntityC(*new DPOImagePNGBodyC<PixelT>(fn))
      {}
    //: Constructor from filename.  
    
    DPOImagePNGC(const OStreamC &strm)
      : DPEntityC(*new DPOImagePNGBodyC<PixelT>(strm))
      {}
    //: Constructor from stream.  
  };
}

#endif
