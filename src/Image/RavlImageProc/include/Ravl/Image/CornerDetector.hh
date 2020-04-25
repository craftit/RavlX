// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_CORNERDETECTOR_HEADER
#define RAVLIMAGE_CORNERDETECTOR_HEADER 1
////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! author="Charles Galambos"
//! docentry="Ravl.API.Images.Corner Detection"
//! date="20/7/2002"
//! lib=RavlImageProc
//! file="Ravl/Image/Processing/Corners/CornerDetector.hh"

#include "Ravl/RefCounter.hh"
#include "Ravl/DList.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/Corner.hh"

namespace RavlImageN {
  
  //: Abstract corner detector.
  // Where a 'corner' is a defined as a well localised 
  // 2d position in an image.
  
  class CornerDetectorBodyC 
    : public RCBodyVC
  {
  public:
    CornerDetectorBodyC();
    //: Default constructor.
    
    virtual DListC<CornerC> Apply(const ImageC<ByteT> &img);
    //: Get a list of corners from 'img'
    
  };

  //: Abstract corner detector.
  // Where a 'corner' is a defined as a well localised 
  // 2d position in an image.
  
  class CornerDetectorC 
    : public RCHandleC<CornerDetectorBodyC>
  {
  public:
    CornerDetectorC()
    {}
    //: Default constructor.

  protected:
    CornerDetectorC(CornerDetectorBodyC &bod)
      : RCHandleC<CornerDetectorBodyC>(bod)
    {}
    //: Body constructor.
    
    CornerDetectorBodyC &Body()
    { return RCHandleC<CornerDetectorBodyC>::Body(); }
    //: Access body.
    
    const CornerDetectorBodyC &Body() const
    { return RCHandleC<CornerDetectorBodyC>::Body(); }
    //: Access body.
    
  public:
    DListC<CornerC> Apply(const ImageC<ByteT> &img)
    { return Body().Apply(img); }
    //: Get a list of corners from 'img'
    
  };

  
}



#endif
