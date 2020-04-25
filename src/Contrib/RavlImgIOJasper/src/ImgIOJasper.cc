// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2005, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlImgIOJasper

#include "Ravl/Image/ImgIOJasper.hh"
#include "Ravl/Array2dIter.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlImageN {
  
  static bool jasInitDone = false;
  //: Jasper IO base class.
  
  DPImageIOJasperBaseC::DPImageIOJasperBaseC(const RealT i_compressionRate)
    : iostream(0),
      defaultFmt(-1),
      useStrmI(false),
      useStrmO(false),
      compressionRate(i_compressionRate),
      useCompressionRate(true)
  {
    if(!jasInitDone) {
      if(jas_init()) {
        // Init failed ?
      }
      jasInitDone = true;
    }
  }
  
  //: Destructor.
  
  DPImageIOJasperBaseC::~DPImageIOJasperBaseC()
  { Close(); }
  
  //: Close the stream.
  
  void DPImageIOJasperBaseC::Close() {
    ONDEBUG(cerr << "DPImageIOJasperBaseC::Close, Called. \n");
    if(iostream != 0 && jas_stream_close(iostream)) {
      cerr << "DPImageIOJasperBaseC::~DPImageIOJasperBaseC, Warning: Failed to close stream. ";
    }
    iostream = 0;
  }
  
  //: Open stream for reading
  
  bool DPImageIOJasperBaseC::OpenRead(const StringC &filename) {
    ONDEBUG(cerr << "DPImageIOJasperBaseC::OpenRead, Opening " << filename << " \n");
    iostream = jas_stream_fopen(filename,"rb");
    return iostream != 0;
  }
  
  //: Open a memory buffer for reading.
  
  bool DPImageIOJasperBaseC::OpenReadMem(const SArray1dC<char> &data) {
    ONDEBUG(cerr << "DPImageIOJasperBaseC::OpenRead, Opening memory buffer " << data.Size() << " \n");
    if(data.Size() < 1)
      return false;
    iostream = jas_stream_memopen(const_cast<char *>(&(data[0])),data.Size());
    return iostream != 0;    
  }
  
  //: Open stream for write
  
  bool DPImageIOJasperBaseC::OpenWrite(const StringC &filename) {
    iostream = jas_stream_fopen(filename,"wb");    
    return iostream != 0;    
  }
  
  //: Open a memory buffer for reading.
  
  bool DPImageIOJasperBaseC::OpenWriteMem(void) {
    ONDEBUG(cerr << "DPImageIOJasperBaseC::OpenWriteMem, Opening memory buffer \n");
    iostream = jas_stream_memopen(0,0);
    return iostream != 0;
  }
  
  //: Access current write buffer.
  // Returns false if one is not found.
  
  bool DPImageIOJasperBaseC::WriteBuffer(SArray1dC<char> &data) {
    if(iostream == 0)
      return false;
    // How much data is there ?
    long len = jas_stream_length(iostream);
    if(len < 1) return false; // Nothing to read!
    data = SArray1dC<char>(len);
    // Go to start of stream
    if(jas_stream_seek(iostream,0,SEEK_SET) != 0)
      return false;
    // Read out data into user array.
    if(jas_stream_read(iostream,&(data[0]),len) != len)
      return false;
    return true;
  }
  
  //: Test if the current stream can be read.
  
  bool DPImageIOJasperBaseC::CanReadImage() { 
    if(iostream == 0)
      return false;
    int fmt = jas_image_getfmt(iostream);
    ONDEBUG(cerr << "DPImageIOJasperBaseC::CanReadImage, Fmt=" << fmt << "\n");
    if(fmt < 0) return false;
    jas_image_fmtinfo_t *fmtinfo = jas_image_lookupfmtbyid(fmt);
    if(fmtinfo == 0) return false;
    bool canRead = (fmtinfo->ops.decode != 0);
    ONDEBUG(cerr << "DPImageIOJasperBaseC::CanReadImage, FmtInfo='" << fmtinfo->desc << "' Read=" << canRead << " Write=" << (fmtinfo->ops.encode != 0) << "\n");
    return canRead;
  }
  
  //: Load image from current stream.
  
  jas_image_t *DPImageIOJasperBaseC::LoadImage() {
    ONDEBUG(cerr << "DPImageIOJasperBaseC::LoadImage(), Called. \n");
    RavlAssert(iostream != 0);
    jas_image_t *img = jas_image_decode(iostream, -1, 0);
    if(img == 0) {
      cerr << "DPImageIOJasperBaseC::LoadImage(), Failed. \n";
    }
    ONDEBUG(std::cerr << "Loaded Image: x=" << img->tlx_ << " y=" << img->tly_ << " brx=" << img->brx_ << " bry=" << img->bry_ << "\n");
    return img;
  }

  //: Convert image to RGB colour space.
  
  jas_image_t *DPImageIOJasperBaseC::ConvertToRGB(jas_image_t *image) {   
    ONDEBUG(cerr << "DPImageIOJasperBaseC::ConvertToRGB, Called. \n");
    jas_image_t *newimage;
    jas_cmprof_t *outprof;
    if (!(outprof = jas_cmprof_createfromclrspc(JAS_CLRSPC_SRGB))) {
      jas_image_destroy(image);
      cerr << "DPImageIOJasperBaseC::ConvertToRGB, Failed to create colour space conversion." << endl;
      return 0;
    }
    if (!(newimage = jas_image_chclrspc(image, outprof, JAS_CMXFORM_INTENT_PER))) {
      jas_image_destroy(image);
      jas_cmprof_destroy(outprof);
      cerr << "DPImageIOJasperBaseC::ConvertToRGB, Colour space conversion failed." << endl;
      return 0;
    }
    jas_image_destroy(image);
    jas_cmprof_destroy(outprof);
    return newimage;
  }
  
  //: Free an old image.
  
  bool DPImageIOJasperBaseC::FreeImage(jas_image_t *image) {
    jas_image_destroy(image);
    return true;
  }
  
  //: Convert an image into RAVL form.
  // img is free'd by operation.
  // Returns false if failes.
  
  bool DPImageIOJasperBaseC::Jas2Ravl(jas_image_t *img,ImageC<ByteRGBValueC> &rimg) {
    ONDEBUG(cerr << "DPImageIOJasperBaseC::Jas2Ravl, Called." << endl);
    // Check the colour space is correct.
    if(jas_image_clrspc(img) != JAS_CLRSPC_SRGB)
      img = ConvertToRGB(img);
    
    //: Sort out the bounding box.
    Index2dC topLeft(jas_image_tly(img),jas_image_tlx(img));
    Index2dC bottomRight(jas_image_bry(img)-1,jas_image_brx(img)-1);
    
    IndexRange2dC imageRec(topLeft,bottomRight);
    if(imageRec.Area() <= 0) { // Empty image ?
      rimg = ImageC<ByteRGBValueC>();
      return true;
    }
    Index2dC size = imageRec.Size();
    
    ONDEBUG(cerr << "ImageSize=" << size << " Origin=" << topLeft  << " End=" << bottomRight << "\n");
    
    //: Create an image.
    ImageC<ByteRGBValueC> anImg(imageRec);
    
    int ncmpts = jas_image_numcmpts(img);
    
    jas_matrix_t *matrix[3];
    matrix[0] = 0;
    matrix[1] = 0;
    matrix[2] = 0;
    
    for(int i = 0;i < ncmpts;i++) {
      
      int ctype = jas_image_cmpttype(img,i);
      int lwidth = jas_image_cmptwidth(img, i);
      int lheight = jas_image_cmptheight(img,i);
      int ldepth = jas_image_cmptprec(img, i);
      
      ONDEBUG(cerr << "Layer=" << i << " Type=" << ctype << " Depth=" << ldepth << " Width=" << lwidth << " Height=" << lheight << " \n");
      int comp = -1;
      switch(ctype) {
      case JAS_IMAGE_CT_RGB_R: comp = 0; break;
      case JAS_IMAGE_CT_RGB_G: comp = 1; break;
      case JAS_IMAGE_CT_RGB_B: comp = 2; break;
      default:
        continue;
      }
      // Check its one of the colour components
      
      if(lwidth != size.Col() && lheight != size.Row()) {
        cerr << "DPImageIOJasperBaseC::Jas2Ravl, Component size mis-match. \n";
        return false;
      }
      if(ldepth > 8) {
        cerr << "DPImageIOJasperBaseC::Jas2Ravl, Component depth mis-match. \n";
        return false;
      }
      ONDEBUG(cerr << "DPImageIOJasperBaseC::Jas2Ravl, Component=" << comp << "\n");
      
      matrix[comp] = jas_matrix_create(size.Row().V(),size.Col().V());
      jas_image_readcmpt(img,i,0,0,size.Col().V(),size.Row().V(),matrix[comp]);
    }
    
    if(matrix[0] == 0 || matrix[1] == 0 || matrix[2] == 0) {
      cerr << "DPImageIOJasperBaseC::Jas2Ravl, Component missing from image. \n";
      // Clean up.
      
      for(int i = 0;i < 3;i++) {
        if(matrix[i] != 0)
          jas_matrix_destroy(matrix[i]);
      }
      return false;
    }
    
    // Copy data from matrix's into a RAVL image.
    
    Array2dIterC<ByteRGBValueC> it(anImg);
    int r = 0;
    for(;it;) {
      int c = 0;
      do {
        *it = ByteRGBValueC(jas_matrix_get(matrix[0],r,c),
                            jas_matrix_get(matrix[1],r,c),
                            jas_matrix_get(matrix[2],r,c)
                            );
        c++;
      } while(it.Next()) ;
      r++;
    }
    
    // Clean up.
    
    for(int i = 0;i < 3;i++)
      jas_matrix_destroy(matrix[i]);
    
    rimg = anImg;
    FreeImage(img);
    
    ONDEBUG(cerr << "DPImageIOJasperBaseC::Jas2Ravl, Done. \n");
    return true;
  }
  
  
  //: save an image to the current stream.
  
  bool DPImageIOJasperBaseC::SaveImage(jas_image_t *img) {
    ONDEBUG(cerr << "DPImageIOJasperBaseC::SaveImage, Writing image. \n");
    StringC opts("");
    if (useCompressionRate && compressionRate < 1.0) {
      opts = StringC("rate=") + StringC(compressionRate);
    }
    if (jas_image_encode(img,iostream, defaultFmt, const_cast<char *>(opts.chars()))) {
      cerr << "DPImageIOJasperBaseC::SaveImage, Failed to encode image. \n";
      return false;
    }
    jas_stream_flush(iostream);
    
    return true;
  }
  
  //: Convert a Ravl image into a Jasper one.
  
  jas_image_t *DPImageIOJasperBaseC::Ravl2Jas(const ImageC<ByteRGBValueC> &img) {
    jas_image_cmptparm_t cmptparms[3];
    
    jas_matrix_t *matrix[3];

    jas_image_coord_t x = img.Frame().LCol().V();
    jas_image_coord_t y = img.Frame().TRow().V();
    jas_image_coord_t width = img.Frame().Cols();
    jas_image_coord_t height = img.Frame().Rows();
    
    ONDEBUG(cerr << " x=" << x << " y=" << y << " width=" << width << " height=" << height << "\n");
    
    for(int i = 0;i < 3;i++) {
      cmptparms[i].tlx = x;
      cmptparms[i].tly = y;
      cmptparms[i].vstep = 1;
      cmptparms[i].hstep = 1;
      cmptparms[i].width = width;
      cmptparms[i].height = height;
      cmptparms[i].prec = 8;           // 8 bits per channel
      cmptparms[i].sgnd = false;        // Not signed
      
      matrix[i] = jas_matrix_create(height,width);
    }
    jas_clrspc_t clrspc = JAS_CLRSPC_SRGB;
    jas_image_t *jimg = jas_image_create(3,cmptparms,clrspc);
    


    Array2dIterC<ByteRGBValueC> it(img);
    int r = 0;
    for(;it;) {
      int c = 0;
      do {
        jas_matrix_set(matrix[0],r,c,it->Red());
        jas_matrix_set(matrix[1],r,c,it->Green());
        jas_matrix_set(matrix[2],r,c,it->Blue());
        c++;
      } while(it.Next()) ;
      r++;
    }
    
    
    // Write planes into image.
    
    for(int i = 0;i < 3;i++) {
      jas_image_writecmpt(jimg, i,
                          0,0,width,height,
                          matrix[i]);
    }
    
    jas_image_setcmpttype(jimg,0,JAS_IMAGE_CT_RGB_R);
    jas_image_setcmpttype(jimg,1,JAS_IMAGE_CT_RGB_G);
    jas_image_setcmpttype(jimg,2,JAS_IMAGE_CT_RGB_B);
    
    for(int i = 0;i < 3;i++)
      jas_matrix_destroy(matrix[i]);
    
#if 0
    int cspc = jas_image_clrspc(jimg);
    int fam = jas_clrspc_fam(cspc);
    cerr << "Cspc=" << cspc << " Fam=" << fam << "\n";
#endif
    ONDEBUG(std::cerr << "Created Image: x=" << jimg->tlx_ << " y=" << jimg->tly_ << " brx=" << jimg->brx_ << " bry=" << jimg->bry_ << "\n");
    return jimg;
  }
  
  //: Look for format by extention name.
  // returns -1 if not found.
  
  IntT DPImageIOJasperBaseC::FindFormatByFilename(const StringC &filename){
    int fmt = jas_image_fmtfromname(const_cast<char *>(filename.chars()));
    ONDEBUG(cerr << "DPImageIOJasperBaseC::FindFormatByFilename, File='" <<filename << "' Fmt=" << fmt << "\n");
    return fmt;
  }
  
  //: Set format by name
  
  void DPImageIOJasperBaseC::SetDefaultFmt(const StringC &fmtName) {
    jas_image_fmtinfo_t *fmtInfo = jas_image_lookupfmtbyname(const_cast<char *>(fmtName.chars()));
    if(fmtInfo == 0) {
      std::cerr << "DPImageIOJasperBaseC::SetDefaultFmt, Fmt " << fmtName << " unknown \n";
      return ;
    }
    defaultFmt = fmtInfo->id;
    if (fmtName != "jp2" && fmtName != "jpc") {
      // "Rate" only supported by jp2 and jpc
      useCompressionRate = false;
    } else {
      useCompressionRate = true;
    }
  }

  static jas_stream_ops_t jas_stream_strmops = {
    DPImageIOJasperBaseC::str_read,
    DPImageIOJasperBaseC::str_write,
    DPImageIOJasperBaseC::str_seek,
    DPImageIOJasperBaseC::str_close
  };
  
  static void jas_stream_initbuf(jas_stream_t *stream, int bufmode, char *buf,
				 int bufsize)
  {
    /* If this function is being called, the buffer should not have been
       initialized yet. */
    assert(!stream->bufbase_);
    
    if (bufmode != JAS_STREAM_UNBUF) {
      /* The full- or line-buffered mode is being employed. */
      if (!buf) {
	/* The caller has not specified a buffer to employ, so allocate
	   one. */
	if ((stream->bufbase_ = (unsigned char *) jas_malloc(JAS_STREAM_BUFSIZE +
							     JAS_STREAM_MAXPUTBACK))) {
	  stream->bufmode_ |= JAS_STREAM_FREEBUF;
	  stream->bufsize_ = JAS_STREAM_BUFSIZE;
	} else {
	  /* The buffer allocation has failed.  Resort to unbuffered
	     operation. */
	  stream->bufbase_ = stream->tinybuf_;
	  stream->bufsize_ = 1;
	}
      } else {
	/* The caller has specified a buffer to employ. */
	/* The buffer must be large enough to accommodate maximum
	   putback. */
	assert(bufsize > JAS_STREAM_MAXPUTBACK);
	stream->bufbase_ = JAS_CAST(jas_uchar *, buf);
	stream->bufsize_ = bufsize - JAS_STREAM_MAXPUTBACK;
      }
    } else {
      /* The unbuffered mode is being employed. */
      /* A buffer should not have been supplied by the caller. */
      assert(!buf);
      /* Use a trivial one-character buffer. */
      stream->bufbase_ = stream->tinybuf_;
      stream->bufsize_ = 1;
    }
    stream->bufstart_ = &stream->bufbase_[JAS_STREAM_MAXPUTBACK];
    stream->ptr_ = stream->bufstart_;
    stream->cnt_ = 0;
    stream->bufmode_ |= bufmode & JAS_STREAM_BUFMODEMASK;
  }

  
  //: Open a memory buffer for reading.
  
  bool DPImageIOJasperBaseC::OpenOStream(OStreamC &os) {
    ONDEBUG(cerr << "DPImageIOJasperBaseC::OpenOStream, Called\n");        
    if (!(iostream = (jas_stream_t *) jas_malloc(sizeof(jas_stream_t)))) {
      return false;
    }
    iostream->openmode_ = JAS_STREAM_READ | JAS_STREAM_WRITE | JAS_STREAM_BINARY;
    iostream->bufmode_ = 0;
    iostream->flags_ = 0;
    iostream->bufbase_ = 0;
    iostream->bufstart_ = 0;
    iostream->bufsize_ = 0;
    iostream->ptr_ = 0;
    iostream->cnt_ = 0;
    iostream->ops_ = &jas_stream_strmops;
    iostream->obj_ = reinterpret_cast<jas_stream_obj_t *>(this);
    iostream->rwcnt_ = 0;
    iostream->rwlimit_ = -1;
    
    jas_stream_initbuf(iostream,JAS_STREAM_FULLBUF, 0, 0);
    
    ostrm = os;
    useStrmO = true;
    return true;
  }
  
  //: Open a memory buffer for reading.
  
  bool DPImageIOJasperBaseC::OpenIStream(IStreamC &is) {
    ONDEBUG(cerr << "DPImageIOJasperBaseC::OpenIStream, Called\n");    
    if (!(iostream = (jas_stream_t *) jas_malloc(sizeof(jas_stream_t)))) {
      return false;
    }
    iostream->openmode_ = JAS_STREAM_READ | JAS_STREAM_WRITE | JAS_STREAM_BINARY;
    iostream->bufmode_ = 0;
    iostream->flags_ = 0;
    iostream->bufbase_ = 0;
    iostream->bufstart_ = 0;
    iostream->bufsize_ = 0;
    iostream->ptr_ = 0;
    iostream->cnt_ = 0;
    iostream->ops_ = &jas_stream_strmops;
    iostream->obj_ = reinterpret_cast<jas_stream_obj_t *>(this);
    iostream->rwcnt_ = 0;
    iostream->rwlimit_ = -1;
    
    jas_stream_initbuf(iostream,JAS_STREAM_FULLBUF, 0, 0);
    
    istrm = is;
    useStrmI = true;
    return true;
  }
  
  int  DPImageIOJasperBaseC::str_read(jas_stream_obj_t *obj, char *buf, int cnt) {
    ONDEBUG(cerr << "DPImageIOJasperBaseC::str_read, Size=" << cnt << "\n");
    DPImageIOJasperBaseC *jb = reinterpret_cast<DPImageIOJasperBaseC *>(obj);
    jb->istrm.read(buf,cnt);
    return jb->istrm.gcount();
  }
  
  int DPImageIOJasperBaseC::str_write(jas_stream_obj_t *obj, char *buf, int cnt) {
    ONDEBUG(cerr << "DPImageIOJasperBaseC::str_write, Size=" << cnt << "\n");
    DPImageIOJasperBaseC *jb = reinterpret_cast<DPImageIOJasperBaseC *>(obj);
    jb->ostrm.write(buf,cnt);
    if(!jb->ostrm)
      return 0;
    return cnt;
  }
  
  long DPImageIOJasperBaseC::str_seek(jas_stream_obj_t *obj, long offset, int origin) {
    ONDEBUG(cerr << "DPImageIOJasperBaseC::str_seek, Called. Offset=" << offset << " Origin=" << origin << "\n");
    DPImageIOJasperBaseC *jb = reinterpret_cast<DPImageIOJasperBaseC *>(obj);
    if(!jb->useStrmO && !jb->useStrmI)
      return 0;
    long off = 0;
    RavlAssert(origin != SEEK_END);
    if(jb->useStrmO) {
      if(origin == SEEK_CUR)
	jb->ostrm.Seek(offset + jb->ostrm.Tell());
      else if(origin == SEEK_SET)
	jb->ostrm.Seek(offset);      
      off = jb->ostrm.Tell();
    }
    if(jb->useStrmI) {
      if(origin == SEEK_CUR)
	jb->istrm.Seek(offset + jb->istrm.Tell());
      else if(origin == SEEK_SET)
	jb->istrm.Seek(offset);      
      off = jb->istrm.Tell();
    }
    return off;
  }
  
  int DPImageIOJasperBaseC::str_close(jas_stream_obj_t *obj) {
    ONDEBUG(cerr << "DPImageIOJasperBaseC::str_close, Called\n");
    DPImageIOJasperBaseC *jb = reinterpret_cast<DPImageIOJasperBaseC *>(obj);
    if(jb->useStrmO) {
      jb->ostrm.Invalidate();
      jb->useStrmO = false;
    }
    if(jb->useStrmI) {
      jb->istrm.Invalidate();
      jb->useStrmI = false;
    }
    return 0;
  }
  
  
}
