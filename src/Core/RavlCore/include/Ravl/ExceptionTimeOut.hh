// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2011, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_EXCEPTIONTIMEOUT_HEADER
#define RAVL_EXCEPTIONTIMEOUT_HEADER 1
///////////////////////////////////////////////////////////////
//! docentry="Ravl.API.Core.Error Handling"
//! file="Ravl/Core/Base/Exception.hh"
//! lib=RavlCore
//! userlevel=Normal
//! author="Charles Galambos"

#include "Ravl/Exception.hh"

namespace RavlN {

  //! userlevel=Normal
  //: Exception thrown when end of stream found.

  class ExceptionTimeOutC
    : public ExceptionC
  {
  public:
    ExceptionTimeOutC(const char *ntext);
    //: Constructor

    ExceptionTimeOutC(const char *ntext,bool copy);
    //: Constructor.
    // if copy is true, make a copy of ntext.
  };


}



#endif
