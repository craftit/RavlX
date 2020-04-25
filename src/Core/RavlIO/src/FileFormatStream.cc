// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////////
//! lib=RavlIO
//! file="Ravl/Core/IO/FileFormatStream.cc"

#include "Ravl/DP/FileFormatStream.hh"
#include "Ravl/TypeName.hh"

namespace RavlN {
  void IncludeFileFormatStream() 
  {  }
  
  // Some basic file types.

  static FileFormatStreamC<RealT> FileFormatStream_RealT("RealT");
  static FileFormatStreamC<IntT> FileFormatStream_IntT("IntT");
  static FileFormatStreamC<UIntT> FileFormatStream_UIntT("UIntT");
  static FileFormatStreamC<StringC> FileFormatStream_StringC("RavlN::StringC");
  static FileFormatStreamC<ByteT> FileFormatStream_ByteT("ByteT");
}
