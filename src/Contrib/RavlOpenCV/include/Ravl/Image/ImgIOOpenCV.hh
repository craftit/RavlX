// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2019, ReactAI Ltd
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_IMGIOOPENCV_HEADER
#define RAVLIMAGE_IMGIOOPENCV_HEADER 1
///////////////////////////////////////////////////
//! userlevel=Develop
//! file="Ravl/Contrib/OpenCV/ImgIOOpenCV.hh"
//! lib=RavlOpenCV
//! docentry="Ravl.API.Images.IO.Formats"
//! author="Charles Galambos"
//! date="12/02/2019"

#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/DP/FileFormat.hh"
#include "Ravl/DP/Port.hh"
#include "Ravl/DP/AttributeValueTypes.hh"
#include "opencv2/opencv.hpp"
#include "Ravl/Image/OpenCVConvert.hh"

namespace RavlImageN {

  //: Open a video stream via OpenCV

  class DPIImageOpenCVMatBodyC :
    public DPIPortBodyC<cv::Mat>
  {
  public:
    DPIImageOpenCVMatBodyC(const StringC & fn);
    //: Constructor from filename.

    ~DPIImageOpenCVMatBodyC();
    //: Destructor

    virtual bool IsGetEOS() const;
    //: Is valid data ?

    virtual bool IsGetReady() const;
    //: Is some data ready ?
    // true = yes.

    virtual bool Get(cv::Mat &buff);
    //: Get next frame of video

    virtual cv::Mat Get();
    //: Get next piece of data.

  protected:
    cv::VideoCapture m_videoCapture;
    bool m_done;
  };

  template<typename ImageTypeT>
  class DPIImageOpenCVBodyC :
    public DPIPortBodyC<ImageTypeT>
  {
  public:
    DPIImageOpenCVBodyC(const StringC & fn)
      : m_filename(fn),
        m_videoCapture(fn.c_str())
    {
      InitAttr(*this);
      m_done = !m_videoCapture.isOpened();
    }
    //: Constructor from filename.

    virtual ~DPIImageOpenCVBodyC()
    {
      m_videoCapture.release();
    }
    //: Destructor

    virtual bool IsGetEOS() const
    { return m_done; }
    //: Is valid data ?

    virtual bool IsGetReady() const
    { return !m_done; }
    //: Is some data ready ?
    // true = yes.

    virtual bool Get(ImageTypeT &buff)
    {
      if(m_done)
        return false;
      cv::Mat img;
      m_videoCapture >> img;
      if(img.empty()) {
        m_done = true;
        return false;
      }
      IplImage iplImgData = img;
      IplImage2RavlImage(&iplImgData,buff);
      return true;
    }
    //: Get next frame of video

    virtual ImageTypeT Get()
    {
      ImageTypeT img;
      if(!Get(img))
        throw DataNotReadyC("End of file");
      return img;
    }
    //: Get next piece of data.

    //: Get a attribute.
    virtual bool GetAttr(const StringC &attrName,RealT &attrValue)
    {
      if(attrName == "framerate") {
        attrValue = m_videoCapture.get(CV_CAP_PROP_FPS);
        return true;
      }
      return DPPortBodyC::GetAttr(attrName,attrValue);
    }

    //: Get a attribute.
    virtual bool GetAttr(const StringC &attrName,StringC &attrValue)
    {
      if(attrName == "filename") {
        attrValue = m_filename;
        return true;
      }
      return DPPortBodyC::GetAttr(attrName,attrValue);
    }



  protected:
    void InitAttr(AttributeCtrlBodyC &attrCtrl) {
      attrCtrl.RegisterAttribute(AttributeTypeNumC<RealT>("framerate","Frame rate of video",true,false,0.0,1000.0,0.01,25));
      attrCtrl.RegisterAttribute(AttributeTypeStringC("aspectratio","Aspect ratio",true,false,"4:3"));
      //attrCtrl.RegisterAttribute(AttributeTypeStringC("filename","Original filename of stream",true,false,""));
    }

    StringC m_filename;
    cv::VideoCapture m_videoCapture;
    bool m_done;
  };


  //: Save a ByteRGB image.

  class DPOImageOpenCVMatBodyC :
    public DPOPortBodyC<cv::Mat>
  {
  public:
    DPOImageOpenCVMatBodyC(const StringC & fn);
    //: Constructor from filename.

    virtual bool Put(const cv::Mat &dat);
    //: Put data.

    virtual bool IsPutReady() const ;
    //: Is port ready for data ?

  protected:
    std::string m_filename;
    cv::VideoWriter m_videoWriter;
    bool m_done;
  };

}

#endif
