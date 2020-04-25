// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_DVFRAME_HEADER
#define RAVLIMAGE_DVFRAME_HEADER 1
////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Image/VideoIO/DVFrame.hh"
//! lib=RavlVideoIO
//! example=exImgSeq.cc
//! docentry="Ravl.API.Images.Video.Video_IO"
//! author="Daniele Muntoni"
//! date="20/03/2003"

#include "Ravl/SArray1d.hh"

namespace RavlImageN {
  using namespace RavlN;
#if RAVL_VISUALCPP_NAMESPACE_BUG
  using RavlN::SArray1dC;
#endif

  //! userlevel=Develop
  //: DV Frame class.
  
  class DVFrameC : public SArray1dC<char>
  {
  public:
    //constructors
    DVFrameC();
    //: Constructor.
    // Creates an empty DV Frame
    
    DVFrameC(const SizeT dim);
    //: Constructor.
    //: Creates a DVFrame of dim bytes
    
  };
}

#endif
