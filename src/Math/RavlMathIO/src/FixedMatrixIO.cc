// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlMathIO
//! file="Ravl/Math/IO/FixedMatrixIO.cc"

#include "Ravl/Matrix2d.hh"
#include "Ravl/Matrix3d.hh"
#include "Ravl/Matrix4d.hh"
#include "Ravl/DP/FileFormatStream.hh"
#include "Ravl/DP/FileFormatBinStream.hh"
#include "Ravl/SArray1d.hh"

namespace RavlN {
  static TypeNameC type1(typeid(Matrix2dC),"Matrix2dC");
  static TypeNameC type2(typeid(Matrix3dC),"Matrix3dC");
  static TypeNameC type3(typeid(Matrix4dC),"Matrix4dC");
  
  static TypeNameC type4(typeid(SArray1dC<Matrix2dC>),"SArray1dC<Matrix2dC>");
  static TypeNameC type5(typeid(SArray1dC<Matrix3dC>),"SArray1dC<Matrix3dC>");
  static TypeNameC type6(typeid(SArray1dC<Matrix4dC>),"SArray1dC<Matrix4dC>");
  
  void InitFixedMatrixIO()
  {}
  
  static FileFormatStreamC<Matrix2dC> FileFormatStream_Matrix2d;
  static FileFormatStreamC<Matrix3dC> FileFormatStream_Matrix3d;
  static FileFormatStreamC<Matrix4dC> FileFormatStream_Matrix4d;
  
  static FileFormatBinStreamC<Matrix2dC> FileFormatBinStream_Matrix2d;
  static FileFormatBinStreamC<Matrix3dC> FileFormatBinStream_Matrix3d;
  static FileFormatBinStreamC<Matrix4dC> FileFormatBinStream_Matrix4d;
  
  // Add some IO for arrays of Matrices.

  static FileFormatStreamC<SArray1dC<Matrix2dC> > FileFormatStream_SArray1dC_Matrix2d;
  static FileFormatStreamC<SArray1dC<Matrix3dC> > FileFormatStream_SArray1dC_Matrix3d;
  static FileFormatStreamC<SArray1dC<Matrix4dC> > FileFormatStream_SArray1dC_Matrix4d;
  
  static FileFormatBinStreamC<SArray1dC<Matrix2dC> > FileFormatBinStream_SArray1dC_Matrix2d;
  static FileFormatBinStreamC<SArray1dC<Matrix3dC> > FileFormatBinStream_SArray1dC_Matrix3d;
  static FileFormatBinStreamC<SArray1dC<Matrix4dC> > FileFormatBinStream_SArray1dC_Matrix4d;
}
