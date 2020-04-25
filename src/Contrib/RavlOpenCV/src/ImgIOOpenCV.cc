// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2019, ReactAI Ltd
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! lib=RavlOpenCV
//! file="Ravl/src/Contrib/OpenCV/ImgIOOpenCV.cc"

#include "Ravl/Image/ImgIOOpenCV.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlImageN {

  ///////////////////////////////////////

  //: Constructor from filename.

  DPIImageOpenCVMatBodyC::DPIImageOpenCVMatBodyC(const StringC & fn)
    : m_videoCapture(fn.c_str())
  {
    m_done = !m_videoCapture.isOpened();
  }

  //: Destructor
  DPIImageOpenCVMatBodyC::~DPIImageOpenCVMatBodyC()
  {
    m_videoCapture.release();
  }

  //: Is valid data ?

  bool DPIImageOpenCVMatBodyC::IsGetEOS() const
  { return m_done; }

  //: Is some data ready ?
  // true = yes.

  bool DPIImageOpenCVMatBodyC::IsGetReady() const
  { return !m_done; }


  //: Get image from stream.

  bool DPIImageOpenCVMatBodyC::Get(cv::Mat &buff)
  {
    if(m_done)
      return false;
    m_videoCapture >> buff;
    return !buff.empty();
  }

  //: Get next piece of data.

  cv::Mat DPIImageOpenCVMatBodyC::Get()
  {
    cv::Mat img;
    if(!Get(img))
      throw DataNotReadyC("End of file");
    return img;
  }

  ///////////////////////////////////////

  //: Constructor from filename.

  DPOImageOpenCVMatBodyC::DPOImageOpenCVMatBodyC(const StringC & fn)
    : m_filename(fn.c_str()),
      m_done(false)
  {}


  //: Is port ready for data ?

  bool DPOImageOpenCVMatBodyC::IsPutReady() const
  { return !m_done;  }

  //: Write out image.

  bool DPOImageOpenCVMatBodyC::Put(const cv::Mat &img)
  {
    m_videoWriter.write(img);
    return true;
  }

}

