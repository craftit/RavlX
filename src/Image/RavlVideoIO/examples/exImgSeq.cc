// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlVideoIO
//! file="Ravl/Image/VideoIO/exImgSeq.cc"
//! example=exImgSeq.cc
//! docentry="Ravl.API.Images.Video"
//! userlevel=Normal
//! author="Bill Christmas, Charles Galambos"
//! date="08/04/1999"

//: Loading and saving image sequences

#include "Ravl/Option.hh"
#include "Ravl/DP/SequenceIO.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ImgIO.hh"

using namespace RavlN;
using namespace RavlImageN;

int main(int argc,char **argv) 
{  
  // Process options.
  
  OptionC option(argc,argv);
  StringC ifmt = option.String("if","","Input format. ");
  StringC ofmt = option.String("of","","Output format. ");
  bool verbose = option.Boolean("v",false,"Verbose mode. ");
  StringC ifilename = option.String("","","Input stream.");
  StringC ofilename = option.String("","@X","Output stream.");
  option.CompulsoryArgs(1); // input sequence name is compulsory
  option.Check();
  
  DPIPortC<ImageC<ByteT> > inputStream;
  DPOPortC<ImageC<ByteT> > outputStream;
  
  if(!OpenISequence(inputStream,ifilename,ifmt,verbose)) {
    std::cerr << "Failed to open input sequence '" << ifilename << "' \n";
    return 1;
  }
  
  if(!OpenOSequence(outputStream,ofilename,ofmt,verbose)) {
    std::cerr << "Failed to open output sequence '" << ofilename << "' \n";
    return 1;
  }
  
  // Copy stream of images from input to output.
  
  for(;;) {
    ImageC<ByteT> buff;
    if(!inputStream.Get(buff)) // End of stream ?
      break; 
    if(!outputStream.Put(buff)) {
      std::cerr << "ERROR: Failed to write output. \n";
      break;
    }
  }
  
#if 0
  ////////////////////////////////////////////
  // Alernate main loop...
  for(;!inputStream.IsGetEOS();) { // Is End Of Stream ?
    ImageC<ByteT> buff = inputStream.Get();
    if(!outputStream.Put(buff)) {
      std::cerr << "ERROR: Failed to write output. \n";
      break;
    }    
  }  
#endif  
  
  return 0;
}
