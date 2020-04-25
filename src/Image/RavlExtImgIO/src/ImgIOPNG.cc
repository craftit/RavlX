// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlExtImgIO
//! file="Ravl/Image/ExternalImageIO/PNG/ImgIOPNG.cc"

#include <png.h>

#include "Ravl/Image/ImgIOPNGB.hh"
#include "Ravl/Image/ImgIOPNG.hh"
#include "Ravl/Image/ByteRGBAValue.hh"
#include "Ravl/Image/UInt16RGBValue.hh"
#include "Ravl/Stream.hh"

//#include <stdio.h>
#include <setjmp.h>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

using namespace RavlN;

///// Some 'C' IO interface routines. /////////////////////////////////////

extern "C" {
  static void my_png_filein(png_structp png_ptr, png_bytep data, png_size_t length);
  static void my_png_fileout(png_structp png_ptr, png_bytep data, png_size_t length);
  static void my_png_flush(png_structp png_ptr);
}

void my_png_filein(png_structp png_ptr, png_bytep data, png_size_t length) {
  RavlN::IStreamC &is = *((RavlN::IStreamC *)png_get_io_ptr(png_ptr));
  RavlAssert(is.good());
  is.read((char *) data,(streamsize) length);
  if(!is.good())
    png_error(png_ptr, "Read Error");  
}

void my_png_fileout(png_structp png_ptr, png_bytep data, png_size_t length) {
  RavlN::OStreamC &os = *((RavlN::OStreamC *)png_get_io_ptr(png_ptr));
  RavlAssert(os.good());
  os.write((const char *) data,(streamsize) length);
  if(!os.good())
    png_error(png_ptr, "Write Error");  
}

void my_png_flush(png_structp png_ptr) {
  RavlN::OStreamC &os = *((RavlN::OStreamC *)png_get_io_ptr(png_ptr));
  os.os().flush();
  if(!os.good())
    png_error(png_ptr, "Write Error (flush)"); 
}

namespace RavlImageN {
  
  
  ///// DPImageIOPNGBaseC ///////////////////////////////////
  
  //: Constructor.
  
  DPImageIOPNGBaseC::DPImageIOPNGBaseC()
    : done(false),
      initalised(false)
  {}
  
  //: Destructor.
  
  DPImageIOPNGBaseC::~DPImageIOPNGBaseC()
  {}
  
  ///// DPIImageIOPNGBaseC ///////////////////////////////////
  
  //: Constructor
  
  DPIImageIOPNGBaseC::DPIImageIOPNGBaseC(const IStreamC &nfin)
    : fin(nfin)
  {
    /* Create and initialize the png_struct with the desired error handler
     * functions.  If you want to use the default stderr and longjump method,
     * you can supply NULL for the last three parameters.  We also supply the
     * the compiler header file version, so that we know if the application
     * was compiled with a compatible version of the library.  REQUIRED
     */
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,0,0,0);
    //png_voidp user_error_ptr, user_error_fn, user_warning_fn);
    
    if (png_ptr == NULL)
      return ;
    
    /* Allocate/initialize the memory for image information.  REQUIRED. */
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
      png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
      return ;
    }
  
    if (setjmp(png_jmpbuf(png_ptr))) {
      /* Free all of the memory associated with the png_ptr and info_ptr */
      png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
      /* If we get here, we had a problem reading the file */
      return ;
    }
    
    /* If you are using replacement read functions, instead of calling
     * png_init_io() here you would call:
     */
    png_set_read_fn(png_ptr, (void *)(&fin), my_png_filein);
    /* where user_io_ptr is a structure you want available to the callbacks */
    
    initalised = true;
  }
  
  //: Destructor.
  
  DPIImageIOPNGBaseC::~DPIImageIOPNGBaseC()  {
    initalised = false;
    png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
  }
  
  //: Get information about a png file.
  
  bool DPIImageIOPNGBaseC::ReadHeaderInfo(int &nbit_depth,int &ncolourType,int &ninterlace) {
    if (setjmp(png_jmpbuf(png_ptr))) 
      return false;
    
    /* The call to png_read_info() gives us all of the information from the
     * PNG file before the first IDAT (image data chunk).  REQUIRED
     */
    png_read_info(png_ptr, info_ptr);
    
    png_uint_32 width, height;
    
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
		 &interlace_type, NULL, NULL);
    
    nbit_depth = bit_depth;
    ncolourType = color_type;
    ninterlace = interlace_type;
    return true;
  }

  //: Read header information.
  
  bool DPIImageIOPNGBaseC::ReadHeader(const std::type_info &obj_type) {
    if (setjmp(png_jmpbuf(png_ptr))) 
      return false;
    
    /* The call to png_read_info() gives us all of the information from the
     * PNG file before the first IDAT (image data chunk).  REQUIRED
     */
    png_read_info(png_ptr, info_ptr);
    
    png_uint_32 width, height;
    
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
		 &interlace_type, NULL, NULL);
    
    imgRect = ImageRectangleC(height,width);
    
    ONDEBUG(std::cerr << " SrcBitDepth:" << bit_depth <<  " SrcColourType:" << color_type << "\n");
    
    int req_bit_depth, req_color_type,req_chan;
    
    ONDEBUG(std::cerr << "PixelType:" << obj_type.name() << "\n");
    
    if(obj_type == typeid(ByteRGBValueC)) {
      ///////////////// ByteRGBValueC /////////////////////
      req_bit_depth = 8;
      req_color_type = PNG_COLOR_TYPE_RGB;
      req_chan = 3;
    } else if(obj_type == typeid(ByteT)) {
      ///////////////// ByteT /////////////////////
      req_bit_depth = 8;
      req_color_type = PNG_COLOR_TYPE_GRAY;
      req_chan = 1;
    } else if(obj_type == typeid(UInt16T)) {
      ///////////////// GreyValueU16T /////////////////////
      req_bit_depth = 16;
      req_color_type = PNG_COLOR_TYPE_GRAY;
      req_chan = 1;
    } else if(obj_type == typeid(ByteRGBAValueC)) {
      ///////////////// ByteRGBAValueC /////////////////////
      req_bit_depth = 8;
      req_color_type = PNG_COLOR_TYPE_RGB_ALPHA;
      req_chan = 4;
    } else if(obj_type == typeid(UInt16RGBValueC)) {
      ///////////////// RGBValueU16C /////////////////////
      req_bit_depth = 16;
      req_color_type = PNG_COLOR_TYPE_RGB;
      req_chan = 3;
    } else {
      std::cerr << "DPOImageIOPNGBaseC::ReadHeader(), ERROR: Unknown pixel type '" << obj_type.name() << "'\n";
      return false;
    }
    
    /* tell libpng to strip 16 bit/color files down to 8 bits/color */
    if(req_bit_depth != 16) {
      if(bit_depth == 16) {
	ONDEBUG(std::cerr << "DPOImageIOPNGBaseC::ReadHeader(), png_set_strip_16(png_ptr), Called. \n");
	png_set_strip_16(png_ptr);
      }
    }
    
    // Extract multiple pixels with bit depths of 1, 2, and 4 from a single
    // byte into separate bytes (useful for paletted and grayscale images).
    
    if(bit_depth < 8) {
      ONDEBUG(std::cerr << "DPOImageIOPNGBaseC::ReadHeader(), png_set_packing(png_ptr), Called. \n");
      png_set_packing(png_ptr);
    }
    
    /* Change the order of packed pixels to least significant bit first
     * (not useful if you are using png_set_packing). */
    //png_set_packswap(png_ptr);
    
    // Strip alpha bytes from the input data without combining with the
    //background (not recommended).
    if(color_type == PNG_COLOR_TYPE_RGB_ALPHA || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
      if(req_color_type == PNG_COLOR_TYPE_RGB || req_color_type == PNG_COLOR_TYPE_GRAY) {
	ONDEBUG(std::cerr << "DPOImageIOPNGBaseC::ReadHeader(), png_set_strip_alpha(png_ptr), Called. \n");
	png_set_strip_alpha(png_ptr);
      }
    
    // Expand paletted colors into true RGB triplets 
    // Amma doesn't support pallet images, so always do this.
    if (color_type == PNG_COLOR_TYPE_PALETTE) {
      ONDEBUG(std::cerr << "DPOImageIOPNGBaseC::ReadHeader(), png_set_expand(png_ptr), Called. \n");
      png_set_expand(png_ptr);
      if(req_color_type == PNG_COLOR_TYPE_RGB || req_color_type == PNG_COLOR_TYPE_GRAY) {
	      ONDEBUG(std::cerr << "DPOImageIOPNGBaseC::ReadHeader(), png_set_strip_alpha(png_ptr), Called. \n");
	      png_set_strip_alpha(png_ptr);
      }
    }
    
    // Expand grayscale images to the full 8 bits from 1, 2, or 4 bits/pixel 
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) {
      ONDEBUG(std::cerr << "DPOImageIOPNGBaseC::ReadHeader(), png_set_expand(png_ptr), Called. \n");
      png_set_expand(png_ptr);
      if(req_color_type == PNG_COLOR_TYPE_RGB || req_color_type == PNG_COLOR_TYPE_GRAY) {
	      ONDEBUG(std::cerr << "DPOImageIOPNGBaseC::ReadHeader(), png_set_strip_alpha(png_ptr), Called. \n");
	      png_set_strip_alpha(png_ptr);
      }
    }
    
    if(req_color_type == PNG_COLOR_TYPE_RGB_ALPHA) {
      // Expand paletted or RGB images with transparency to full alpha channels
      // so the data will be available as RGBA quartets.
      if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) {
	ONDEBUG(std::cerr << "DPOImageIOPNGBaseC::ReadHeader(), png_set_expand(png_ptr), Called. \n");
	png_set_expand(png_ptr);
      }
    }
    
#if 1
    /* If you want to shift the pixel values from the range [0,255] or
     * [0,65535] to the original [0,7] or [0,31], or whatever range the
     * colors were originally in:
     */
    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_sBIT)) {
      png_color_8p sig_bit;
      png_get_sBIT(png_ptr, info_ptr, &sig_bit);
      png_set_shift(png_ptr, sig_bit);
    }
#endif
    
    // Grey to RGB conversion needed ?
    
    if(req_color_type == PNG_COLOR_TYPE_RGB || req_color_type == PNG_COLOR_TYPE_RGB_ALPHA) {
      if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
	ONDEBUG(std::cerr << "DPOImageIOPNGBaseC::ReadHeader(), png_set_gray_to_rgb(png_ptr), Called. \n");
	png_set_gray_to_rgb(png_ptr);
      }
    }
    
    // RGB to Grey conversion needed ?
    
#if 1
    if(req_color_type == PNG_COLOR_TYPE_GRAY || req_color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
      if((color_type&2) == PNG_COLOR_TYPE_RGB) {
        ONDEBUG(std::cerr << "DPOImageIOPNGBaseC::ReadHeader(), png_set_rgb_to_gray_fixed(png_ptr), Called. \n");
        png_set_rgb_to_gray_fixed(png_ptr, 1,-1,-1);
      }
    }
#endif
    
    // Need to add an alpha channel ?
    
    if(req_color_type == PNG_COLOR_TYPE_RGB_ALPHA) {
      if (color_type == PNG_COLOR_TYPE_RGB) {
	ONDEBUG(std::cerr << "DPOImageIOPNGBaseC::ReadHeader(), png_set_filler(png_ptr,x,PNG_FILLER_AFTER), Called. \n");
	if(bit_depth == 8) 
	  png_set_filler(png_ptr,255,PNG_FILLER_AFTER);
	else
	  png_set_filler(png_ptr,65535,PNG_FILLER_AFTER);
      }
    }
    
    /* Update information about the image taking into account any transformations. 
     * This may cause a warning to be issues with some versions of the library. 
     * FIXME :- Find a way of getting rid of it.
     */
    png_read_update_info(png_ptr, info_ptr);
    
    /* Do some sanity checking. */
    
    ONDEBUG(std::cerr << "Width:" << width << " Height:" << height << " ReqBitDepth:" << req_bit_depth <<  " Channels:" << req_chan << " ReqColourType=" << req_color_type << "\n");
    ONDEBUG(std::cerr << " Actual BitDepth:" << bit_depth <<  " ColourType=" << color_type << "\n");
    ONDEBUG(std::cerr << "Row bytes " << png_get_rowbytes(png_ptr, info_ptr) << " Expected:" << (width * (req_bit_depth/8) * req_chan) << "\n");
    // Check the configuration is at least roughly right!!
    RavlAssert(png_get_rowbytes(png_ptr, info_ptr)  == (width * (req_bit_depth/8) * req_chan));
    
    return true;
  }
  
  ///// DPOImageIOPNGBaseC ///////////////////////////////////
  
  //: Constructor
  
  DPOImageIOPNGBaseC::DPOImageIOPNGBaseC(const OStreamC &nfout)
    : fout(nfout)
  {
    bit_depth = 8;
    interlacing = 0;
    
    /* Create and initialize the png_struct with the desired error handler
     * functions.  If you want to use the default stderr and longjump method,
     * you can supply NULL for the last three parameters.  We also check that
     * the library version is compatible with the one used at compile time,
     * in case we are using dynamically linked libraries.  REQUIRED.
     */
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,
				      0,0,0);
    //png_voidp user_error_ptr, user_error_fn, user_warning_fn);
    
    if (png_ptr == NULL)
      return ;  
    
    /* Allocate/initialize the image information data.  REQUIRED */
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
      png_destroy_write_struct(&png_ptr,  (png_infopp)NULL);
      return ;
    }
    
    /* If you are using replacement read functions, instead of calling
     * png_init_io() here you would call */
    png_set_write_fn(png_ptr, 
		     (void *)(&fout), 
		   my_png_fileout,
		     my_png_flush);
    /* where user_io_ptr is a structure you want available to the callbacks */
    
    initalised = true;
  }
  
  //: Destructor.
  
  DPOImageIOPNGBaseC::~DPOImageIOPNGBaseC()  {
    initalised = false;
    /* If you png_malloced a palette, free it here (don't free info_ptr->palette,
       as recommended in versions 1.0.5m and earlier of this example; if
       libpng mallocs info_ptr->palette, libpng will free it).  If you
       allocated it with malloc() instead of png_malloc(), use free() instead
       of png_free(). */
    //png_free(png_ptr, palette);
    
    /* Similarly, if you png_malloced any data that you passed in with
       png_set_something(), such as a hist or trans array, free it here,
       when you can be sure that libpng is through with it. */
    //png_free(png_ptr, trans);
    
    /* clean up after the write, and free any memory allocated */
    
    png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
  }
  
  //: Write header information.
  
  bool DPOImageIOPNGBaseC::WriteHeader(const std::type_info &obj_type,const ImageRectangleC &nimgRect) {
    imgRect = nimgRect;
    
    /* Set the image information here.  Width and height are up to 2^31,
     * bit_depth is one of 1, 2, 4, 8, or 16, but valid values also depend on
     * the color_type selected. color_type is one of PNG_COLOR_TYPE_GRAY,
     * PNG_COLOR_TYPE_GRAY_ALPHA, PNG_COLOR_TYPE_PALETTE, PNG_COLOR_TYPE_RGB,
     * or PNG_COLOR_TYPE_RGB_ALPHA.  interlace is either PNG_INTERLACE_NONE or
     * PNG_INTERLACE_ADAM7, and the compression_type and filter_type MUST
     * currently be PNG_COMPRESSION_TYPE_BASE and PNG_FILTER_TYPE_BASE. REQUIRED
     */
    
    png_uint_32 width = imgRect.Cols();
    png_uint_32 height = imgRect.Rows();
    interlace_type = PNG_INTERLACE_NONE;
    
    if(obj_type == typeid(ByteRGBValueC)) {
      ///////////////// ByteRGBValueC /////////////////////
      bit_depth = 8;
      color_type = PNG_COLOR_TYPE_RGB;
    } else if(obj_type == typeid(ByteT)) {
      ///////////////// ByteT /////////////////////
      bit_depth = 8;
      color_type = PNG_COLOR_TYPE_GRAY;
    } else if(obj_type == typeid(UInt16T)) {
      ///////////////// GreyValueU16T /////////////////////
      bit_depth = 16;
      color_type = PNG_COLOR_TYPE_GRAY;
    } else if(obj_type == typeid(ByteRGBAValueC)) {
      ///////////////// ByteRGBAValueC /////////////////////
      bit_depth = 8;
      color_type = PNG_COLOR_TYPE_RGB_ALPHA;
    } else if(obj_type == typeid(UInt16RGBValueC)) {
      ///////////////// RGBValueU16C /////////////////////
      bit_depth = 16;
      color_type = PNG_COLOR_TYPE_RGB;
    } else {
      std::cerr << "DPOImageIOPNGBaseC::WriteHeader(), ERROR: Unknown pixel type '" << obj_type.name() << "'\n";
      return false;
    }
    
    
    if (setjmp(png_jmpbuf(png_ptr))) 
      return false;
    
    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type,
		 interlace_type, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
    
    /* optional significant bit chunk */
#if 0
    /* if we are dealing with a grayscale image then */
    sig_bit.gray = true_bit_depth;
    /* otherwise, if we are dealing with a color image then */
    sig_bit.red = true_red_bit_depth;
    sig_bit.green = true_green_bit_depth;
    sig_bit.blue = true_blue_bit_depth;
    /* if the image has an alpha channel then */
    sig_bit.alpha = true_alpha_bit_depth;
    png_set_sBIT(png_ptr, info_ptr, sig_bit);
#endif
    
    /* Optional gamma chunk is strongly suggested if you have any guess
     * as to the correct gamma of the image.
     */
#if 0
    png_set_gAMA(png_ptr, info_ptr, gamma);
#endif
    
    /* Optionally write comments into the image */
#if 0
    text_ptr[0].key = "Title";
    text_ptr[0].text = "Mona Lisa";
    text_ptr[0].compression = PNG_TEXT_COMPRESSION_NONE;
    text_ptr[2].key = "Description";
    text_ptr[2].text = "<desc>";
    text_ptr[2].compression = PNG_TEXT_COMPRESSION_zTXt;
#endif
    png_text_struct text_ptr[1];
    text_ptr[0].key = (char *)"Description";
    text_ptr[0].text = (char *)"Generated with RAVL.";
    text_ptr[0].compression = PNG_TEXT_COMPRESSION_NONE;
#ifdef PNG_iTXt_SUPPORTED
    text_ptr[0].lang = NULL;
#endif
    png_set_text(png_ptr, info_ptr, text_ptr, 1);
    
    /* other optional chunks like cHRM, bKGD, tRNS, tIME, oFFs, pHYs, */
    /* note that if sRGB is present the gAMA and cHRM chunks must be ignored
     * on read and must be written in accordance with the sRGB profile */
    
    /* Write the file header information.  REQUIRED */
    png_write_info(png_ptr, info_ptr);
    
    /* If you want, you can write the info in two steps, in case you need to
     * write your private chunk ahead of PLTE:
     *
     *   png_write_info_before_PLTE(write_ptr, write_info_ptr);
     *   write_my_chunk();
     *   png_write_info(png_ptr, info_ptr);
     *
     * However, given the level of known- and unknown-chunk support in 1.1.0
     * and up, this should no longer be necessary.
     */
    
    /* Once we write out the header, the compression type on the text
     * chunks gets changed to PNG_TEXT_COMPRESSION_NONE_WR or
     * PNG_TEXT_COMPRESSION_zTXt_WR, so it doesn't get written out again
     * at the end.
     */
    
    /* set up the transformations you want.  Note that these are
     * all optional.  Only call them if you want them.
     */
    
    /* invert monocrome pixels */
    //png_set_invert_mono(png_ptr);
    
    /* Shift the pixels up to a legal bit depth and fill in
     * as appropriate to correctly scale the image.
     */
    //png_set_shift(png_ptr, &sig_bit);
    
    /* pack pixels into bytes */
    //png_set_packing(png_ptr);
    
    /* swap location of alpha bytes from ARGB to RGBA */
    //png_set_swap_alpha(png_ptr);
    
    /* Get rid of filler (OR ALPHA) bytes, pack ARGB/RGBA/ARGB/RGBA into
     * RGB (4 channels -> 3 channels). The second parameter is not used.
     */
    //png_set_filler(png_ptr, 0, PNG_FILLER_BEFORE);
    
    /* flip BGR pixels to RGB */
    //png_set_bgr(png_ptr);
    
    /* swap bytes of 16-bit files to most significant byte first */
    //png_set_swap(png_ptr);
    
    /* swap bits of 1, 2, 4 bit packed pixel formats */
    //png_set_packswap(png_ptr);
    
    return true;
  }

  //// DPIImagePNGByteRGBC /////////////////////////////////////////////
  
  //: Constructor from filename.  
  
  DPIImagePNGByteRGBC::DPIImagePNGByteRGBC(StringC fn)
    : DPEntityC(*new DPIImagePNGBodyC<ByteRGBValueC>(fn))
  {}
  
  //: Constructor from stream 
  // Not supported at the moment...
  
  DPIImagePNGByteRGBC::DPIImagePNGByteRGBC(const IStreamC &strm)
    : DPEntityC(*new DPIImagePNGBodyC<ByteRGBValueC>(strm))
  {}

  //// DPOImagePNGByteRGBC /////////////////////////////////////////////
  
  //: Constructor from filename.  
  
  DPOImagePNGByteRGBC::DPOImagePNGByteRGBC(StringC fn)
    : DPEntityC(*new DPOImagePNGBodyC<ByteRGBValueC>(fn))
  {}
  
  
  //: Constructor from stream.  
  // Not supported at the moment...
  
  DPOImagePNGByteRGBC::DPOImagePNGByteRGBC(const OStreamC &strm)
    : DPEntityC(*new DPOImagePNGBodyC<ByteRGBValueC>(strm))
  {}
  
  
  //// DPIImagePNGByteGreyC /////////////////////////////////////////////
  
  //: Constructor from filename.  
  
  DPIImagePNGByteGreyC::DPIImagePNGByteGreyC(StringC fn)
    : DPEntityC(*new DPIImagePNGBodyC<ByteT>(fn))
  {}
  
  
  //: Constructor from stream 
  // Not supported at the moment...
  
  DPIImagePNGByteGreyC::DPIImagePNGByteGreyC(const IStreamC &strm)
    : DPEntityC(*new DPIImagePNGBodyC<ByteT>(strm))
  {}
  
  //// DPOImagePNGByteGreyC /////////////////////////////////////////////
  
  //: Constructor from filename.  
  
  DPOImagePNGByteGreyC::DPOImagePNGByteGreyC(StringC fn)
    : DPEntityC(*new DPOImagePNGBodyC<ByteT>(fn))
  {}
  
  
  //: Constructor from stream.  
  // Not supported at the moment...
  
  DPOImagePNGByteGreyC::DPOImagePNGByteGreyC(const OStreamC &strm)
    : DPEntityC(*new DPOImagePNGBodyC<ByteT>(strm))
  {}
}

