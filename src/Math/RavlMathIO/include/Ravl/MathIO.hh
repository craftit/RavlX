// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_MATHIO_HEADER
#define RAVL_MATHIO_HEADER 1
///////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlMathIO
//! file="Ravl/Math/IO/MathIO.hh"
//! author="Charles Galambos"
//! userlevel=Develop
//! docentry="Ravl.API.Math"

namespace RavlN {
  
  extern void InitMathIO(); //: Initalise all the math IO.
  
  extern void InitFixedVectorIO();
  extern void InitFixedPointIO();
  extern void InitFixedMatrixIO();
  extern void InitBoundryIO();
  extern void InitMatrixIO();
  extern void InitGeom2dIO();
  extern void InitTMatrixIO();
}

#endif
