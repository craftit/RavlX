// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#include "Ravl/Stream.hh"
#include "Ravl/String.hh"
#include "Ravl/OS/Directory.hh"
#include "Ravl/OS/Filename.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/Array2dIter.hh"
#include "Ravl/IO.hh"
#include "Ravl/Tuple3.hh"
#include "Ravl/DList.hh"
#include "Ravl/DLIter.hh"
#include "Ravl/Math.hh"
#include "Ravl/DP/SequenceIO.hh"

using namespace RavlN;
using namespace RavlImageN;

// Function to convert between 2 given video formats
// Includes check that frame sizes match, and pixels correspond within set tolerance.
// args are: in file, out file, pixel match tolerance

int testImageConvert(const Tuple3C<StringC,StringC,UIntT>& imFiles) {
  cout << "Converting from " << imFiles.Data1() << " to " << imFiles.Data2() << endl;
  DirectoryC dir = StringC(PROJECT_OUT)+"/share/RAVL/testData/";
  FilenameC in =  dir + imFiles.Data1();
  FilenameC out = dir + imFiles.Data2();
  cout << "Converting from " << in << " to " << out << endl;
  if (!in.Exists())  return __LINE__;
  FilenameC prog = "conv";
  // This program must be called with a controlled setting of PATH to pickup the
  // correct version of conv to be tested
  StringC cmd = prog + " -s " + in + " " + out;
  if (system(cmd.chars()) != 0) return __LINE__;

  DPIPortC<ImageC<ByteRGBValueC> > inStr, outStr;
  ImageC<ByteRGBValueC> i, o;
  if(!OpenISequence(inStr, in)) return __LINE__;
  inStr.Get(i);
  if(!OpenISequence(outStr, out)) return __LINE__;
  outStr.Get(o);
  if (i.Frame().Size() != o.Frame().Size()) return __LINE__;
  for (Array2dIter2C<ByteRGBValueC,ByteRGBValueC>p(i,o); p; ++p)
    for (UIntT c=0; c<3; ++c) 
      if (Abs((IntT)p.Data1()[c] - (IntT)p.Data2()[c]) > (IntT)imFiles.Data3()) {
        cout << p.Index() << " "<< Abs((IntT)p.Data1()[c] - (IntT)p.Data2()[c]);
        return __LINE__;
      }
  return 0;
}


int main() {
  int ln;
  DListC<Tuple3C<StringC,StringC,UIntT> > imPair;
  // Just add whatever conversion you like.
  // Last arg is max allowable conversion error
  imPair.Append(Tuple3C<StringC,StringC,UIntT>("in0.ppm",  "out0.ppm",  0));
  imPair.Append(Tuple3C<StringC,StringC,UIntT>("in0.ppm",  "out0.png",  0));
  imPair.Append(Tuple3C<StringC,StringC,UIntT>("out0.png", "out0.ppm",  0));
  imPair.Append(Tuple3C<StringC,StringC,UIntT>("in0.ppm",  "out0.jpg", 15));
  for (DLIterC<Tuple3C<StringC,StringC,UIntT> > i(imPair); i; ++i) {
    if ((ln = testImageConvert(*i)) != 0) {
      cerr << "Test for \"conv\" failed for " << i->Data1() << " -> " << i->Data2() << " on line " << ln << "\n";
      return 1;
    }
  }
  cerr << "Test passed ok. \n";
  return 0;
}

