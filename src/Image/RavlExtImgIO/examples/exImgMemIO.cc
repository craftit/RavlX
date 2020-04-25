// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2005-12, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! author="Charles Galambos"
//! lib=RavlExtImgIO
//! file="Ravl/Image/ExternalImageIO/Core/exImgMemIO.cc"
//! docentry="Ravl.API.Images.IO.Formats"

#include "Ravl/Option.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Array2dIter2.hh"
#include "Ravl/DP/MemIO.hh"

using namespace RavlN;
using namespace RavlImageN;

int main(int nargs,char **argv) {
  OptionC opts(nargs,argv);
  StringC formatName = opts.String("f","","Format name ");
  bool verbose = opts.Boolean("v",false,"Verbose mode.");
  opts.Check();
  
  ImageC<ByteT> img(100,100);
  img.Fill(1);
  
  SArray1dC<char> buffer;
  if(!MemSave(buffer,img,formatName,verbose)) {
    std::cerr << "Failed to save image. \n";
    return 1;
  }
  
  ImageC<ByteT> img2;
  if(!MemLoad(buffer,img2,formatName,verbose)) {
    std::cerr << "Failed to load image. \n";
    return 1;    
  }
  
  for(Array2dIter2C<ByteT,ByteT> it(img,img2);it;it++) {
    if(it.Data1() != it.Data2()) {
      std::cerr << "Images differ. \n";
      return 1;
    }
  }
  std::cerr << "Test passed ok. BufferSize=" << buffer.Size() << "\n";
  return 0;
}
