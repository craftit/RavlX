// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2005, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlImageProc

#include "Ravl/Option.hh"
#include "Ravl/IO.hh"
#include "Ravl/Image/ImagePointFeatureSet.hh"

using namespace RavlN;
using namespace RavlImageN;

int main(int nargs,char **argv) {
  OptionC opt(nargs,argv);
  StringC inFile = opt.String("i","in.xml","Input file. ");
  StringC outFile = opt.String("o","out.xml","Output file. ");
  opt.Check();
  
  ImagePointFeatureSetC fs;
  if(!RavlN::Load(inFile,fs)) {
    cerr << "Failed to save file " << inFile << "\n";
    return 1;
  }
  
  cerr << "Attribs=" << fs.Attributes() << "\n";
  cerr << "Attribs=" << fs.SubsetAttrib("Left Outer Eye 1") << "\n";

  if(!RavlN::Save(outFile,fs)) {
    cerr << "Failed to save file " << outFile << "\n";
    return 1;
  }

  return 0;
}
