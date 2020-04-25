// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2011, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////////////
//! lib=RavlCore
//! file="Ravl/Core/Base/ExceptionTimeOut.cc"

#include "Ravl/ExceptionTimeOut.hh"

namespace RavlN {

  //: Constructor
  ExceptionTimeOutC::ExceptionTimeOutC(const char *ntext)
   : ExceptionC(ntext)
  {}

  //: Constructor.
  // if copy is true, make a copy of ntext.
  ExceptionTimeOutC::ExceptionTimeOutC(const char *ntext,bool copy)
   : ExceptionC(ntext,copy)
  {}

}
