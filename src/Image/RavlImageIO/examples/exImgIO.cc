// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlImageIO
//! file="Ravl/Image/ImageIO/exImgIO.cc"
//! userlevel=Normal
//! author="Charles Galambos"
//! date="16/03/1999"
//! docentry="Ravl.API.Images"

//: Loading and saving images.

#include "Ravl/Option.hh"
#include "Ravl/DP/FileFormatIO.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteRGBValue.hh"

int main(int argc,char **argv) {  
  using namespace RavlImageN;
  
  OptionC option(argc,argv);
  StringC infile = option.String("i","testi.pgm","Input filename");
  StringC output = option.String("o","testo.pgm","Output filename");
  StringC ifmt = option.String("if","","Input format");
  StringC ofmt = option.String("of","","Output format");
  bool viaByte = option.Boolean("bw",false,"Convert via grey scale image. ");
  bool verbose = option.Boolean("v",false,"Verbose mode ");
  option.Check();
  
  if(viaByte) {
    // Convert via a byte image.
    
    ImageC<ByteT> bi;
    
    if(!RavlN::Load(infile,bi,ifmt,verbose)) {
      std::cerr << "Failed to load data. \n";
      return 1;
    }
    if(verbose)
      std::cout << "Image size:" << bi.Rectangle() << std::endl;
    if(!RavlN::Save(output,bi,ofmt,verbose)) {
      std::cerr << "Failed to save data. \n";
      return 1;
    }
  } else {
    // Convert via an rgb image.
    
    ImageC<ByteRGBValueC> rgb;
    
    if(!RavlN::Load(infile,rgb,ifmt,verbose)) {
      std::cerr << "Failed to load data. \n";
      return 1;
    }
    if(verbose)
      std::cout << "Image size:" << rgb.Rectangle() << std::endl;
    if(!RavlN::Save(output,rgb,ofmt,verbose)) {
      std::cerr << "Failed to save data. \n";
      return 1;
    }
  }
  
  return 0;
}
