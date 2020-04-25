// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////
//! lib=RavlImage
//! file="Ravl/Image/Base/testHSVValue.cc"
//! rcsid="$Id$"
//! docentry="Ravl.API.Images.Pixel Types"
//! userlevel=Develop

#include "Ravl/Random.hh"
#include "Ravl/Image/RealHSVValue.hh"

using namespace RavlImageN;
 
int TestBasic();

//template class ImageC<int>; // Make sure all functions are compiled.

int main()
{
  std::cerr << "\n\nTesting basic HSV RGB conversion operations \n";
  int lineno;
  if((lineno = TestBasic()) != 0) {
    std::cerr << "\nHSV RGB Conversion Failed : " << lineno << "\n";
    return 1;
  }
  
  std::cerr << "Test Completed OK " ; 
  return 0;
}

////////////////////////////////
// Check iterators, and image copying


int TestBasic()
{
  const UIntT numTrials = 1000000 ;
  const RealT smallThresh = 0.01 ;
 
  for ( UIntT count = 1 ; count <= numTrials ; ++count ) 
  {
    RealRGBValueC rgb ( Random1(), Random1(), Random1() ) ; 
    RealHSVValueC hsv (rgb) ; 

    if ( ( hsv.RGB() - rgb).Sum() > smallThresh )
      return __LINE__ ;
  }

  return 0 ;
}

