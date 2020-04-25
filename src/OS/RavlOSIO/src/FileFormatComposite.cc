// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlOSIO
//! file="Ravl/OS/IO/FileFormatComposite.cc"

#include "Ravl/DP/FileFormatComposite.hh"
#include "Ravl/DP/FileFormatStream.hh"
#include "Ravl/DP/FileFormatBinStream.hh"
#include "Ravl/TypeName.hh"

namespace RavlN {
  void IncludeFileFormatComposite() 
  {}
  
  static TypeNameC type1(typeid(PlayListC),"RavlN::PlayListC");
  
  // Some basic file types.
  
  FileFormatStreamC<PlayListC> FileFormatStream_StdIO_PlayListC;
  FileFormatBinStreamC<PlayListC> FileFormatBinStream_StdIO_PlayListC;
  FileFormatCompositeC<IntT> FileFormatComposite_IntT;
  
}
