// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLSKTERROR_HEADER
#define RAVLSKTERROR_HEADER 1
///////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/OS/Network/SktError.hh"
//! lib=RavlNet
//! userlevel=Advanced
//! docentry="Ravl.API.OS.Network"
//! author="Charles Galambos"
//! date="15/02/2000"

#include "Ravl/Exception.hh"

namespace RavlN {
  //! userlevel=Advanced
  //: Networking exception.
  
  class ExceptionNetC
    : public ExceptionC
  {
  public:
    ExceptionNetC(const char *desc,bool copy = false)
      : ExceptionC(desc,copy)
      {}
    //: Constructor.
    
  };

}

#endif
