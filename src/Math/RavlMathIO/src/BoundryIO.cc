// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlMathIO
//! file="Ravl/Math/IO/BoundryIO.cc"

#include "Ravl/Boundary.hh"
#include "Ravl/DP/FileFormatStream.hh"
#include "Ravl/DP/FileFormatBinStream.hh"
#include "Ravl/DList.hh"

namespace RavlN {
  void InitBoundryIO()
  {}
  
  static TypeNameC type1(typeid(BoundaryC),"BoundaryC");
  static TypeNameC type2(typeid(DListC<BoundaryC>),"DListC<BoundaryC>");
  
  static FileFormatStreamC<BoundaryC> FileFormatStream_Boundary;
  static FileFormatBinStreamC<BoundaryC> FileFormatBinStream_Boundary;
  
  static FileFormatStreamC<DListC<BoundaryC> > FileFormatStream_DListC_Boundary;
  static FileFormatBinStreamC<DListC<BoundaryC> > FileFormatBinStream_DListC_Boundary;
  
}
