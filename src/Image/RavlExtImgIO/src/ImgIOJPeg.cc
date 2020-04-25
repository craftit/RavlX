// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////
//! lib=RavlExtImgIO
//! file="Ravl/Image/ExternalImageIO/JPEG/ImgIOJPeg.cc"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

extern "C" {
#include <stdio.h>
#include <jpeglib.h>
#include <jerror.h>
  
  void my_error_exit (j_common_ptr cinfo);
};

#include "Ravl/Image/ImgIOJPegB.hh"
#include "Ravl/Image/ByteYUVValue.hh"
#include "Ravl/SysLog.hh"

namespace RavlImageN {
  
  const unsigned int DPImageIOJPegBaseC::defaultBufferSize = 4096;

  ///// ERROR HANDLING //////////////////////////////////////////////////
  
  static
  void my_error_exit (j_common_ptr cinfo) {
    /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
    DPImageIOJPegBaseC::my_error_ptr myerr = (DPImageIOJPegBaseC::my_error_ptr) cinfo->err;
    
    /* Always display the message. */
    /* We could postpone this until after returning, if we chose. */
    char buffer[512];
    (*cinfo->err->format_message)(cinfo, buffer);
    RavlError("JPEG Error: %s ",buffer);
    
    /* Return control to the setjmp point */
    longjmp(myerr->setjmp_buffer, 1);
  }
  
  ///// DATA OUTPUT //////////////////////////////////////////////////
  
  // Initialize destination.  This is called by jpeg_start_compress()
  // before any data is actually written.  It must initialize
  // next_output_byte and free_in_buffer.  free_in_buffer must be
  // initialized to a positive value.
  
  static
  void init_destination (j_compress_ptr cinfo) {
    ravl_destination_mgr *dest = (ravl_destination_mgr *) cinfo->dest;
    DPImageIOJPegOBaseC &obj = *dest->obj;
    
    /* Allocate the output buffer --- it will be released when done with image */
    
    dest->pub.next_output_byte = dest->buffer;
    dest->pub.free_in_buffer = obj.BufferSize();
  }
  
  // This is called whenever the buffer has filled (free_in_buffer
  // reaches zero).  In typical applications, it should write out the
  // *entire* buffer (use the saved start address and buffer length;
  // ignore the current state of next_output_byte and free_in_buffer).
  // Then reset the pointer & count to the start of the buffer, and
  // return true indicating that the buffer has been dumped.
  // free_in_buffer must be set to a positive value when true is
  // returned.  A false return should only be used when I/O suspension is
  // desired (this operating mode is discussed in the next section).
  
  static
  boolean empty_output_buffer (j_compress_ptr cinfo) {
    ravl_destination_mgr *dest = (ravl_destination_mgr *) cinfo->dest;
    DPImageIOJPegOBaseC &obj = *dest->obj;
    obj.FOut().write((char *) dest->buffer,obj.BufferSize());
    if(!obj.FOut().good()) 
      ERREXIT(cinfo, JERR_FILE_WRITE);
    dest->pub.next_output_byte = dest->buffer;
    dest->pub.free_in_buffer = obj.BufferSize();
    return TRUE;
    
  }
  
  // Terminate destination --- called by jpeg_finish_compress() after all
  // data has been written.  In most applications, this must flush any
  // data remaining in the buffer.  Use either next_output_byte or
  // free_in_buffer to determine how much data is in the buffer.
  
  static
  void term_destination (j_compress_ptr cinfo) {
    ravl_destination_mgr *dest = (ravl_destination_mgr *) cinfo->dest;
    DPImageIOJPegOBaseC &obj = *dest->obj;
    
    size_t datacount = obj.BufferSize() - dest->pub.free_in_buffer;
    
    /* Write any data remaining in the buffer */
    if (datacount > 0) {
      obj.FOut().os().write((char *) dest->buffer, datacount);
      if(!obj.FOut().good()) 
	ERREXIT(cinfo, JERR_FILE_WRITE);
  }
    obj.FOut().os().flush();
    //fflush(dest->outfile);
    /* Make sure we wrote the output file OK */
    
    if(!obj.FOut().good()) 
      ERREXIT(cinfo, JERR_FILE_WRITE);
    
  }
  
  
  ///// DATA INPUT //////////////////////////////////////////////////
  
  // Initialize source.  This is called by jpeg_read_header() before any
  // data is actually read.  Unlike init_destination(), it may leave
  // bytes_in_buffer set to 0 (in which case a fill_input_buffer() call
  // will occur immediately).
  
  static
  void  init_source (j_decompress_ptr cinfo) {
    ravl_source_mgr *src = (ravl_source_mgr *) cinfo->src;
    
    /* We reset the empty-input-file flag for each image,
     * but we don't clear the input buffer.
     * This is correct behaviour for reading a series of images from one source.
     */
    src->start_of_file = true;
  }
  
  // This is called whenever bytes_in_buffer has reached zero and more
  // data is wanted.  In typical applications, it should read fresh data
  // into the buffer (ignoring the current state of next_input_byte and
  // bytes_in_buffer), reset the pointer & count to the start of the
  // buffer, and return true indicating that the buffer has been reloaded.
  // It is not necessary to fill the buffer entirely, only to obtain at
  // least one more byte.  bytes_in_buffer MUST be set to a positive value
  // if true is returned.  A false return should only be used when I/O
  // suspension is desired (this mode is discussed in the next section).
  
  static
  boolean  fill_input_buffer (j_decompress_ptr cinfo) {
    ravl_source_mgr *src = (ravl_source_mgr *) cinfo->src;
    DPImageIOJPegIBaseC &obj = *src->obj;  
    
    size_t nbytes;
    
    obj.FIn().read((char *) (src->buffer),obj.BufferSize());
    nbytes = obj.FIn().gcount();
    
    if (nbytes <= 0) {
      if (src->start_of_file)	/* Treat empty input file as fatal error */
	ERREXIT(cinfo, JERR_INPUT_EMPTY);
      WARNMS(cinfo, JWRN_JPEG_EOF);
      
      /* Insert a fake EOI marker */
      src->buffer[0] = (JOCTET) 0xFF;
      src->buffer[1] = (JOCTET) JPEG_EOI;
      nbytes = 2;
    }
    
    src->pub.next_input_byte = src->buffer;
    src->pub.bytes_in_buffer = nbytes;
    src->start_of_file = false;
    
    return TRUE;
  }

  //	Skip num_bytes worth of data.  The buffer pointer and count should
  //	be advanced over num_bytes input bytes, refilling the buffer as
  //	needed.  This is used to skip over a potentially large amount of
  //	uninteresting data (such as an APPn marker).  In some applications
  //	it may be possible to optimize away the reading of the skipped data,
  //	but it's not clear that being smart is worth much trouble; large
  //	skips are uncommon.  bytes_in_buffer may be zero on return.
  //	A zero or negative skip count should be treated as a no-op.
  
  static
  void  skip_input_data (j_decompress_ptr cinfo, long num_bytes) {
    ravl_source_mgr *src = (ravl_source_mgr *) cinfo->src;
    //DPImageIOJPegIBaseC &obj = *src->obj;  
    
    /* Just a dumb implementation for now.  Could use fseek() except
     * it doesn't work on pipes.  Not clear that being smart is worth
     * any trouble anyway --- large skips are infrequent.
     */
    if (num_bytes > 0) {
      while (num_bytes > (long) src->pub.bytes_in_buffer) {
	num_bytes -= (long) src->pub.bytes_in_buffer;
	fill_input_buffer(cinfo);
	/* note we assume that fill_input_buffer will never return false,
	 * so suspension need not be handled.
	 */
      }
      src->pub.next_input_byte += (size_t) num_bytes;
      src->pub.bytes_in_buffer -= (size_t) num_bytes;
    }
  }
  
  // Terminate source --- called by jpeg_finish_decompress() after all
  // data has been read.  Often a no-op.
  
  static
  void term_source (j_decompress_ptr cinfo) 
  {}
  
  
  /////// DPImageIOJPegBaseC //////////////////////////////////////////////
  
  DPImageIOJPegBaseC::DPImageIOJPegBaseC()
    : initalised(false),
      buffer(0)
  {
    buffer = new unsigned char [defaultBufferSize];
  }
  
  DPImageIOJPegBaseC::DPImageIOJPegBaseC(const DPImageIOJPegBaseC &)
  : initalised(false),
    buffer(0)
  { RavlIssueError("Disabled copy constructor called."); }

  //: Destructor.
  
  DPImageIOJPegBaseC::~DPImageIOJPegBaseC() {
    initalised = false;
    if(buffer != 0)
      delete [] buffer;
    buffer = 0;
  }

  /////// DPImageIOJPegIBaseC //////////////////////////////////////////////
  
  //: Constructor.
  
  DPImageIOJPegIBaseC::DPImageIOJPegIBaseC(const IStreamC &nfin)
    : fin(nfin)
  {
    ONDEBUG(std::cerr << "DPImageIOJPegIBaseC::DPImageIOJPegIBaseC\n");
    /* We have to set up the error handler first, in case the initialization
     * step fails.  (Unlikely, but it could happen if you are out of memory.)
     * This routine fills in the contents of struct jerr, and returns jerr's
     * address which we place into the link field in cinfo.
     */
    cinfo.err = jpeg_std_error(&(jerr.pub));
    
    /* In this example we want to open the input file before doing anything else,
     * so that the setjmp() error recovery below can assume the file is open.
     * VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
     * requires it in order to read binary files.
     */
    
    /* Step 1: allocate and initialize JPEG decompression object */
    
    /* We set up the normal JPEG error routines, then override error_exit. */
    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = my_error_exit;
    
    /* Establish the setjmp return context for my_error_exit to use. */
    if (setjmp(jerr.setjmp_buffer)) {
      /* If we get here, the JPEG code has signalled an error.
       * We need to clean up the JPEG object, close the input file, and return.
       */
      RavlError("Failed to initialise codec. ");
      jpeg_destroy_decompress(&cinfo);
      initalised = false;
      return ;
    }
    
    /* Now we can initialize the JPEG decompression object. */
    jpeg_create_decompress(&cinfo);
    
    /* Now we can initialize the JPEG decompression object. */
    
    InitI(fin);  
    //RavlDebug("Initialised %p ",(void *) this);
    initalised = true;
  }
  
  //: Destructor
  
  DPImageIOJPegIBaseC::~DPImageIOJPegIBaseC() {
    if(initalised) {
      // Establish the setjmp return context for my_error_exit to use.
      if (setjmp(jerr.setjmp_buffer)) {
	// If we get here, the JPEG code has signaled an error.
	RavlError("Failed to destroy decompressor. ");
	return ;
      }
      /* This is an important step since it will release a good deal of memory. */
      jpeg_destroy_decompress(&cinfo);
      initalised = false;
    }
  }
  
  
  //: Initalise input.
  
  void DPImageIOJPegIBaseC::InitI(IStreamC &os) {
    /* Step 2: specify data source (eg, a file) */
    
    cinfo.src = (struct jpeg_source_mgr *)(&source_mgr.pub);
    typedef boolean (*rsync_ptr)(jpeg_decompress_struct *,int);
    source_mgr.obj = this;
    source_mgr.buffer = buffer;
    source_mgr.pub.init_source = init_source;
    source_mgr.pub.fill_input_buffer = fill_input_buffer;
    source_mgr.pub.skip_input_data = skip_input_data;
    source_mgr.pub.resync_to_restart = (rsync_ptr) jpeg_resync_to_restart; /* use default method */
    source_mgr.pub.term_source = term_source;
    source_mgr.pub.bytes_in_buffer = 0; /* forces fill_input_buffer on first read */
    source_mgr.pub.next_input_byte = NULL; /* until buffer loaded */
  }
  
  //: Setup input format.
  // Must be called after header has been read.
  
  bool DPImageIOJPegIBaseC::SetupFormat(const std::type_info &ti) {
    if(ti == typeid(ByteRGBValueC)) {
      //cinfo.out_components = 3;		/* # of color components per pixel */
      cinfo.out_color_space = JCS_RGB; 	/* colorspace of input image */
      return true;
    }
    if(ti == typeid(ByteYUVValueC)) {
      //cinfo.out_components = 3;		/* # of color components per pixel */
      cinfo.out_color_space = JCS_YCbCr; 	/* colorspace of input image */
      return true;
    }
    if(ti == typeid(ByteT)) {
      //cinfo.out_components = 1;		 /* # of color components per pixel */
      cinfo.out_color_space = JCS_GRAYSCALE;/* colorspace of input image */
      return true;
    }
    std::cerr << "DPImageIOJPegIBaseC::SetupFormat(), Unknown pixel type.  '" << ti.name() << "'\n";
    RavlAssert(0);
    return false;
  }
  
  /////// DPImageIOJPegOBaseC //////////////////////////////////////////////
  
  //: Constructor.
  
  DPImageIOJPegOBaseC::DPImageIOJPegOBaseC(const OStreamC &nfout,int ncompression )
    : fout(nfout),
      compression(ncompression)
  {
    
    /* Step 1: allocate and initialize JPEG compression object */
    
    /* We have to set up the error handler first, in case the initialization
     * step fails.  (Unlikely, but it could happen if you are out of memory.)
     * This routine fills in the contents of struct jerr, and returns jerr's
     * address which we place into the link field in cinfo.
     */
    cinfo.err = jpeg_std_error(&jerr.pub);
    
    /* Now we can initialize the JPEG compression object. */
    
    jerr.pub.error_exit = my_error_exit;
    
    // Establish the setjmp return context for my_error_exit to use.
    
    if (setjmp(jerr.setjmp_buffer)) {
      /* If we get here, the JPEG code has signalled an error.
       * We need to clean up the JPEG object, close the input file, and return.
       */
      RavlError("Error initialising jpeg library. ");
      jpeg_destroy_compress(&cinfo);
      return ;
    }
    
    jpeg_create_compress(&cinfo);
    
    InitO(fout);
    //RavlDebug("JPEG write codec initialised ok. ");
    initalised = true;
  }
  
  //: Destructor
  
  DPImageIOJPegOBaseC::~DPImageIOJPegOBaseC() {
    /* This is an important step since it will release a good deal of memory. */
    if(initalised) {
      // Establish the setjmp return context for my_error_exit to use.
      if (setjmp(jerr.setjmp_buffer)) {
	// If we get here, the JPEG code has signalled an error.
	return ;
      }
      jpeg_destroy_compress(&cinfo);
      initalised = false;
    }
  }
  
  //: Initalise output.
  
  void DPImageIOJPegOBaseC::InitO(OStreamC &os) {
    
    cinfo.dest = (struct jpeg_destination_mgr *) (&destination_mgr); 
    
    destination_mgr.obj = this;
    destination_mgr.buffer = buffer;
    destination_mgr.pub.init_destination = init_destination;
    destination_mgr.pub.empty_output_buffer = empty_output_buffer;
    destination_mgr.pub.term_destination = term_destination;
  }
  
  
  //: Setup output format.
  
  bool DPImageIOJPegOBaseC::SetupFormat(const std::type_info &ti) {
    ONDEBUG(std::cerr << "DPImageIOJPegOBaseC::SetupFormat(), Compression: " << compression << "\n");
    if(ti == typeid(ByteRGBValueC)) {
      cinfo.input_components = 3;		/* # of color components per pixel */
      cinfo.in_color_space = JCS_RGB; 	/* colorspace of input image */
      return true;
    }
    if(ti == typeid(ByteYUVValueC)) {
      cinfo.input_components = 3;		/* # of color components per pixel */
      cinfo.in_color_space = JCS_YCbCr; 	/* colorspace of input image */
      return true;
    }
    if(ti == typeid(ByteT)) {
      cinfo.input_components = 1;		 /* # of color components per pixel */
      cinfo.in_color_space = JCS_GRAYSCALE;      /* colorspace of input image */
      return true;
    }
    std::cerr << "DPImageIOJPegOBaseC::SetupFormat(), Unknown pixel type.  '" << ti.name() << "'\n";
    RavlAssert(0);
    return false;
  }
  
  //// DPIImageJPegByteRGBC /////////////////////////////////////////////
  
  //: Constructor from filename.  
  
  DPIImageJPegByteRGBC::DPIImageJPegByteRGBC(const StringC &fn)
    : DPEntityC(*new DPIImageJPegBodyC<ByteRGBValueC>(fn))
  {}
  
  //: Constructor from stream 
  // Not supported at the moment...
  
  DPIImageJPegByteRGBC::DPIImageJPegByteRGBC(const IStreamC &strm)
    : DPEntityC(*new DPIImageJPegBodyC<ByteRGBValueC>(strm))
  {}
  
  
  //// DPOImageJPegByteRGBC /////////////////////////////////////////////
  
  //: Constructor from filename.  
  
  DPOImageJPegByteRGBC::DPOImageJPegByteRGBC(const StringC &fn,int compression)
    : DPEntityC(*new DPOImageJPegBodyC<ByteRGBValueC>(fn,compression))
  {}
  
  //: Constructor from stream.  
  // Not supported at the moment...
  
  DPOImageJPegByteRGBC::DPOImageJPegByteRGBC(const OStreamC &strm,int compression)
    : DPEntityC(*new DPOImageJPegBodyC<ByteRGBValueC>(strm,compression))
  {}
  
  
  //// DPIImageJPegByteYUVC /////////////////////////////////////////////
  
  //: Constructor from filename.  
  
  DPIImageJPegByteYUVC::DPIImageJPegByteYUVC(const StringC &fn)
    : DPEntityC(*new DPIImageJPegBodyC<ByteYUVValueC>(fn))
  {}
  
  //: Constructor from stream 
  // Not supported at the moment...
  
  DPIImageJPegByteYUVC::DPIImageJPegByteYUVC(const IStreamC &strm)
    : DPEntityC(*new DPIImageJPegBodyC<ByteYUVValueC>(strm))
  {}
  
  
  //// DPOImageJPegByteYUVC /////////////////////////////////////////////
  
  //: Constructor from filename.  
  
  DPOImageJPegByteYUVC::DPOImageJPegByteYUVC(const StringC &fn,int compression)
    : DPEntityC(*new DPOImageJPegBodyC<ByteYUVValueC>(fn,compression))
  {}
  
  //: Constructor from stream.  
  // Not supported at the moment...
  
  DPOImageJPegByteYUVC::DPOImageJPegByteYUVC(const OStreamC &strm,int compression)
    : DPEntityC(*new DPOImageJPegBodyC<ByteYUVValueC>(strm,compression))
  {}
  
  //// DPIImageJPegByteGreyC /////////////////////////////////////////////
  
  //: Constructor from filename.  
  
  DPIImageJPegByteGreyC::DPIImageJPegByteGreyC(const StringC &fn)
    : DPEntityC(*new DPIImageJPegBodyC<ByteT>(fn))
  {}
  
  //: Constructor from stream 
  // Not supported at the moment...
  
  DPIImageJPegByteGreyC::DPIImageJPegByteGreyC(const IStreamC &strm)
    : DPEntityC(*new DPIImageJPegBodyC<ByteT>(strm))
  {}
  
  //// DPOImageJPegByteGreyC /////////////////////////////////////////////
  
  //: Constructor from filename.  
  
  DPOImageJPegByteGreyC::DPOImageJPegByteGreyC(const StringC &fn,int compression)
    : DPEntityC(*new DPOImageJPegBodyC<ByteT>(fn,compression))
  {}
  
  //: Constructor from stream.  
  // Not supported at the moment...
  
  DPOImageJPegByteGreyC::DPOImageJPegByteGreyC(const OStreamC &strm,int compression)
    : DPEntityC(*new DPOImageJPegBodyC<ByteT>(strm,compression))
  {}



}
