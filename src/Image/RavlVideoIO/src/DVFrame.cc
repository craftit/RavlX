// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlVideoIO
//! file="Ravl/Image/VideoIO/DVFrame.cc"

#include "Ravl/Image/DVFrame.hh"
#include "Ravl/TypeName.hh"


namespace RavlImageN {
  static TypeNameC type1(typeid(DVFrameC),"RavlImageN::DVFrameC");  
  
#if RAVL_VISUALCPP_NAMESPACE_BUG
  using RavlN::SizeT;
#endif

  DVFrameC::DVFrameC():SArray1dC<char>()
  {}
  
  DVFrameC::DVFrameC(const SizeT dim):SArray1dC<char>(dim)
  {}

}
