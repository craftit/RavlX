// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlDMedia
//! userlevel=Normal
//! docentry="Ravl.API.Images.Video.Video IO.SGI DMedia"
//! author="Charles Galambos"
//! file="Ravl/Contrib/DMedia/exDMediaIO.cc"

#include "Ravl/DP/SequenceIO.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteRGBValue.hh"

using namespace RavlImageN;
using namespace RavlN;

int main()
{  
  DPIPortC<ImageC<ByteRGBValueC> > imgIn;
  DPOPortC<ImageC<ByteRGBValueC> > imgOut;
  
  if(!OpenISequence(imgIn,"@DIVO_1","",true)) {
    cerr << "Open @DIVO_1 failed. \n";
    return 1;
  }
  if(!OpenOSequence(imgOut,"@X","",true)) {
    cerr << "Open @X failed.  \n";
    return 1;
  }
  
  while(1)
    imgOut.Put(imgIn.Get());
  
  cerr << "Passed!\n";
  return 0;
}
