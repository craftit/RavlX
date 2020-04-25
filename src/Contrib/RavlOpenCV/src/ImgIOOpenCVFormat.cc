// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2019, ReactAI Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////
//! lib=RavlOpenCV
//! file="Ravl/src/Contrib/OpenCV/ImgIOOpenCVFormat.cc"

#include "Ravl/Image/ImgIOOpenCVFormat.hh"
#include "Ravl/Image/ImgIOOpenCV.hh"
#include "Ravl/TypeName.hh"
#include "opencv2/opencv.hpp"

#define DODEBUG 0

#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlImageN {

  //: Constructor.

  FileFormatOpenCVBodyC::FileFormatOpenCVBodyC()
    : FileFormatBodyC("opencv","Open CV image IO.")
  {
  }

  //: Is stream in std stream format ?

  const std::type_info &
  FileFormatOpenCVBodyC::ProbeLoad(IStreamC &in,const std::type_info &obj_type) const
  {
    // IStreamC processing is not supported.
    return typeid(void);
  }

  const std::type_info &
  FileFormatOpenCVBodyC::ProbeLoad(const StringC &nfilename,IStreamC &in,const std::type_info &obj_type) const
  {
    cv::VideoCapture videoCapture(nfilename.c_str());
    if(!videoCapture.isOpened())
      return typeid(void);

    //int videoFormat = videoCapture.get(CV_CAP_PROP_FORMAT);
    //int isMono = videoCapture.get(CV_CAP_PROP_MONOCHROME);
    videoCapture.release();

    ONDEBUG(RavlDebug("Opened file '%s' in format %d Mono:%d (8C1:%d)",nfilename.c_str(),isMono,videoFormat,CV_8UC1));

    if(obj_type == typeid(cv::Mat))
      return typeid(cv::Mat);
#if 0
    // It seems videoFormat isn't set correctly when you open the stream.
    if(videoFormat == CV_8UC3)
      return typeid(ImageC<ByteBGRValueC>);
    if(videoFormat == CV_8UC1)
      return typeid(ImageC<ByteT>);
#endif
    return typeid(ImageC<ByteBGRValueC>);
  }

  const std::type_info &
  FileFormatOpenCVBodyC::ProbeSave(const StringC &filename,const std::type_info &obj_type,bool forceFormat ) const
  {
#if 0
    if(forceFormat)
      return typeid(ImageC<ByteBGRValueC>);
    StringC ext = Extension(filename);
    if(ext != StringC("mpg") && ext != StringC("avi"))
      return typeid(void);
    if(obj_type == typeid(RavlImageN::ImageC<ByteT>))
      return obj_type;
    return typeid(ImageC<ByteBGRValueC>);
#else
    return typeid(void);
#endif
  }

  //: Create a input port for loading from file 'filename'.
  DPIPortBaseC FileFormatOpenCVBodyC::CreateInput(const StringC &filename,const std::type_info &obj_type) const

  {
    if(obj_type == typeid(cv::Mat))
      return DPIPortC<cv::Mat>(*new DPIImageOpenCVMatBodyC(filename));
    if(obj_type == typeid(ImageC<ByteBGRValueC>))
      return DPIPortC<ImageC<ByteBGRValueC> >(*new DPIImageOpenCVBodyC<ImageC<ByteBGRValueC> >(filename));
    if(obj_type == typeid(ImageC<ByteT>))
      return DPIPortC<ImageC<ByteT> >(*new DPIImageOpenCVBodyC<ImageC<ByteT> >(filename));
    return DPOPortBaseC();
  }

  //: Create a output port for saving to file 'filename'..
  DPOPortBaseC FileFormatOpenCVBodyC::CreateOutput(const StringC &filename,const std::type_info &obj_type) const
  {
    return DPOPortBaseC();
  }

  //: Create a input port for loading.

  DPIPortBaseC FileFormatOpenCVBodyC::CreateInput(IStreamC &in,const std::type_info &obj_type) const
  {
    return DPIPortBaseC();
  }

  //: Create a output port for saving.

  DPOPortBaseC FileFormatOpenCVBodyC::CreateOutput(OStreamC &out,const std::type_info &obj_type) const
  {
    return DPOPortBaseC();
  }

  //: Get preferred IO type.

  const std::type_info &FileFormatOpenCVBodyC::DefaultType() const
  { return typeid(ImageC<ByteBGRValueC>); }

  //////////////////////////////////////////////////////////////////

  void InitOpenCVFormat()
  {}

  FileFormatOpenCVC RegisterFileFormatOpenCV;

}
