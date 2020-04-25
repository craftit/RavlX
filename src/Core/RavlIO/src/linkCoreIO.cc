// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2012, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////////
//! lib=RavlIO
//! file="Ravl/Core/IO/linkCoreIO.cc"

#include "Ravl/DP/CoreIO.hh"

namespace RavlN {
  
  void InitFileFormatBinStream();
  void InitFileFormatBinStreamMeta();
  void IncludeFileFormatStreamMeta();
  void LinkProcIStream();


  //: Ensure libRavlIO is linked in
  
  void LinkRavlIO () {
    InitCoreIO();
    InitFileFormatBinStream();
    InitFileFormatBinStreamMeta();
    IncludeFileFormatStreamMeta();
    LinkProcIStream();
  }
  
}

