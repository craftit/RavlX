// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Base/GenBinStream.cc"

#include "Ravl/GenBinStream.hh"

namespace RavlN {

  GenBinOStreamC &GenBinOStreamC::operator << (const char *text) {
    UIntT len = 0;
    if(text == 0) {// Check for null string just in case.
      (*this) << len;
      return (*this);
    }
    const char *place;
    for(place = text;*place != 0;place++,len++) ;
    (*this) << len;
    OBuff(text,len);
    return (*this);
  }
  
  void  GenBinIStreamC::Dummy(void)
  {}
  
  //: Dummy func.

  void  GenBinOStreamC::Dummy(void)
  {}

  
}
