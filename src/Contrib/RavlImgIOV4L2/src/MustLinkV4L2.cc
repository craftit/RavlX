// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! file = "Ravl/Contrib/V4L2/MustLinkV4L2.cc"
//! lib=RavlImgIOV4L2
//! author = "Warren Moore"

namespace RavlImageN
{
  extern void InitV4L2Format();
  
  void MustLinkV4L2()
  { 
    InitV4L2Format();
  }
}
