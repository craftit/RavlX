// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2006, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_TAKESUBIMAGE_HEADER
#define RAVLIMAGE_TAKESUBIMAGE_HEADER 1
//! author="Charles Galambos"
//! docentry="Ravl.API.Images.Misc"
//! lib=RavlImageProc

#include "Ravl/Image/Image.hh"

namespace RavlImageN {
  
  //! userlevel=Normal
  //: Take a sub image.
  // This class is designed to be used with the Ravl data flow classes.
  
  template<typename PixelT>
  class TakeSubImageC
  {
  public:
     TakeSubImageC(const IndexRange2dC &rng)
      : m_subRange(rng)
    {}
    //: Constructor.
    
    ImageC<PixelT> Apply(const ImageC<PixelT> &img) const
    { return ImageC<PixelT>(img,m_subRange); }
    //: Take sub image.
    
    const IndexRange2dC &SubRange() const
    { return m_subRange; }
    //: Access sub range.

    bool SetSubRange(const IndexRange2dC &rng)
    { m_subRange = rng; return true; }
    //: Access sub range.
    
  protected:
    IndexRange2dC m_subRange;
  };

}


#endif
