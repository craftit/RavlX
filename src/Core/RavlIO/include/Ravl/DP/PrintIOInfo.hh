// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2005, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_PRINTIOINFO_HEADER
#define RAVL_PRINTIOINFO_HEADER 1
//! userlevel="Normal"
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.IO.Formats" 
//! lib=RavlIO

#include "Ravl/Stream.hh"

namespace RavlN {
  
  void PrintIOFormats(std::ostream &os = std::cout);
  //: Print in human readable format, information about available formats to 'os'.
  
  void PrintIOConversions(std::ostream &os = std::cout);
  //: Print in human readable format, information about available type conversions to 'os'.
  
  void PrintIOClassTypes(std::ostream &os = std::cout);
  //: Print in human readable format, information about available class types to 'os'.
  
}


#endif
