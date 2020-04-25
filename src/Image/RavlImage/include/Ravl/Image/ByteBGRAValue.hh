// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_BYTEBGRAVALUE_HEADER
#define RAVL_BYTEBGRAVALUE_HEADER 1
/////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Image/Base/ByteBGRAValue.hh"
//! lib=RavlImage
//! userlevel=Normal
//! author="Charles Galambos"
//! date="24/01/2001"
//! docentry="Ravl.API.Images.Pixel Types.RGB"

#include "Ravl/Image/BGRAValue.hh"

namespace RavlImageN {
  
  //: Byte BGRA value class.
  
  class ByteBGRAValueC
    : public BGRAValueC<ByteT>
  {
  public:
    ByteBGRAValueC()
      {}
    //: Default constructor.
    // creates an undefined BGRA pixel.
    
    ByteBGRAValueC(ByteT r,ByteT g,ByteT b,ByteT a)
      : BGRAValueC<ByteT>(r,g,b,a)
      {}
    //: Construct from components.
    
    ByteBGRAValueC(const BGRAValueC<ByteT> &oth)
      : BGRAValueC<ByteT>(oth)
      {}
    //: Default constructor.
    // creates an undefined BGRA pixel.
    
    ByteT Y() const
      { return (ByteT)( ((int) data[0] + (int)data[1] + (int)data[2])/3); }
    //: Calculate intensity of the pixel.
    // This returns the average of the red, green
    // and blue components.
  };
  
}

#endif
