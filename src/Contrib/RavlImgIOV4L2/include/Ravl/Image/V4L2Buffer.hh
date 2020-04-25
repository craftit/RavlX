// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! file = "Ravl/Contrib/V4L2/V4L2Buffer.hh"
//! lib=RavlImgIOV4L2
//! author = "Warren Moore"
//! docentry = "Ravl.API.Images.Video.Video IO.Video4Linux2"

#ifndef RAVL_V4L2BUFFER_HEADER
#define RAVL_V4L2BUFFER_HEADER 1

#include "Ravl/Buffer.hh"
#include "Ravl/Image/ImgIOV4L2.hh"

namespace RavlImageN
{
  using namespace RavlN;
 
  /////////////////////////////
  //! userlevel = Develop
  //: V4L2 fast buffer
  
  template <class PixelT>
  class V4L2BufferBodyC :
    public BufferBodyC<PixelT> 
  {
  public:
    V4L2BufferBodyC(ImgIOV4L2C<PixelT> v4l2, const UIntT id, const IntT index, PixelT *start, UIntT length) :
      BufferBodyC<PixelT>(length, start),
      m_v4l2(v4l2),
      m_id(id),
      m_index(index)
    {}
    //: Constructor
    
    ~V4L2BufferBodyC()
    { m_v4l2.ReleaseBuffer(m_id, m_index); }
    //: Destructor

  protected:
    ImgIOV4L2C<PixelT> m_v4l2;         // Handle to parent V4L2 object
    UIntT m_id;                     // Unique id
    IntT m_index;                   // V4L2 buffer index
  };
  
  
  
  /////////////////////////////
  //! userlevel = Advanced
  //: V4L2 fast buffer.
  // BIG OBJECT
  
  template <class PixelT>
  class V4L2BufferC :
    public BufferC<PixelT>
  {
  public:
    V4L2BufferC()
    {}
    //: Default constructor.
    // Creates an invalid handle.

    V4L2BufferC(ImgIOV4L2C<PixelT> v4l2, const UIntT id, const IntT index, PixelT *start, UIntT length) :
      BufferC<PixelT>(*new V4L2BufferBodyC<PixelT>(v4l2, id, index, start, length))
    {}
    
    explicit V4L2BufferC(const BufferC<PixelT> &base) :
      BufferC<PixelT>(dynamic_cast<const V4L2BufferBodyC<PixelT> *>(RCHandleC<BufferC<PixelT> >::BodyPtr(base)))
    {}
    //: Construct from handle to base class.

  protected:
    V4L2BufferBodyC<PixelT> &Body()
    { return static_cast<V4L2BufferBodyC<PixelT> &>(BufferC<PixelT>::Body()); }
    //: Access body.

    const V4L2BufferBodyC<PixelT> &Body() const
    { return static_cast<const V4L2BufferBodyC<PixelT> &>(BufferC<PixelT>::Body()); }
    //: Access body.
  };
}

#endif

