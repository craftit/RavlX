// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2009, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlImageProc
//! author="Charles Galambos"
//! userlevel=Normal
//! docentry="Ravl.API.Images.Morphology"

#include "Ravl/Option.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/MorphOpen.hh"
#include "Ravl/Image/MorphClose.hh"
#include "Ravl/IO.hh"

using namespace RavlN;
using namespace RavlImageN;

int main(int nargs,char **argv) {
  OptionC opts(nargs,argv);
  StringC inFile = opts.String("i","in.ppm","Input file");
  StringC outFile = opts.String("o","out.ppm","Output file");
  IntT op = opts.Int("m",0,"Operation 0=Open 1=Close. ");
  ByteT setValue = opts.Int("s",1,"Set value. ");
  opts.Check();
  
  ImageC<ByteT> inImg,outImg;
  if(!RavlN::Load(inFile,inImg)) {
    std::cerr << "Failed to load image '" << inFile << "' \n";
    return 1;
  }
  ImageC<ByteT> kernel(IndexRangeC(-1,1),IndexRangeC(-1,1));
  kernel.Fill(1);
  switch(op) {
  case 0 :
    MorphBinaryOpen2d(inImg,kernel,outImg,setValue);
    break;
  case 1:
    MorphBinaryClose2d(inImg,kernel,outImg,setValue);
    break;
  }
  
  if(!RavlN::Save(outFile,outImg)) {
    std::cerr << "Failed to save image '" << outFile << "' \n";
    return 1;    
  }
  
  return 0;
}
