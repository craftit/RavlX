// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlDV
//! file="Ravl/Contrib/DV/doDvDisplay.cc"
//! docentry="Ravl.API.Images.Video.Video IO.DV"
//! userlevel=Normal

#include "Ravl/Option.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/DP/SequenceIO.hh"

using namespace RavlN;
using namespace RavlImageN;

int main(int argc,char **argv) 
{
  OptionC option(argc,argv,true);
  StringC  ifmt = option.String("if","","Input format. ");
  StringC  ofmt = option.String("of","","Output format. ");
  DListC<StringC> attribs = option.List("a","List of attributes to set. e.g. deinterlace=0");
  bool verbose = option.Boolean("v",false,"Verbose mode. ");
  StringC ifilename = option.String("","test.mpg","Input stream.");
  StringC ofilename = option.String("","@X","Output stream.");
  option.Check();
  
  DPIPortC<ImageC<ByteRGBValueC> > inputStream;
  DPOPortC<ImageC<ByteRGBValueC> > outputStream;
  
  if(!OpenISequence(inputStream,ifilename,ifmt,verbose)) {
    cerr << "Failed to open input sequence '" << ifilename << "' \n";
    return 1;
  }
  
  if(!OpenOSequence(outputStream,ofilename,ofmt,verbose)) {
    cerr << "Failed to open output sequence '" << ofilename << "' \n";
    return 1;
  }
  
  for(DLIterC<StringC> it(attribs);it;it++) {
    StringC attrName = it->before('=').TopAndTail();
    StringC attrValue = it->after('=').TopAndTail();
    if(!inputStream.SetAttr(attrName,attrValue))
      cerr << "WARNING: Failed to set attribute '" << attrName << "' to '" << attrValue <<  "'\n";
  }
  
  // Copy stream of images from input to output.
  
  for(;;) {
    ImageC<ByteRGBValueC> buff;
    if(!inputStream.Get(buff)) // End of stream ?
      break; 
    if(!outputStream.Put(buff)) {
      cerr << "ERROR: Failed to write output. \n";
      break;
    }
  }
  
  return 0;
}

