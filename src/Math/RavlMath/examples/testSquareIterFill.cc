// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/Sequence/testSquareIterFill.cc"
//! author="Charles Galambos"
//! docentry="Ravl.API.Math.Sequences"
//! userlevel=Develop

#include "Ravl/EntryPnt.hh"
#include "Ravl/SquareIter.hh"
#include "Ravl/ZigZagIter.hh"
#include "Ravl/OS/Filename.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/DP/FileFormatIO.hh"
#include "Ravl/Option.hh"

using namespace RavlN;
using namespace RavlImageN;

int Main(int argc, char* argv[])
{
  OptionC opt(argc,argv);
  IntT size = opt.Int("s",512,"size of square. ");
  bool label = opt.Boolean("l",false,"Label test. ");
  bool zigZag = opt.Boolean("z",false,"Zig zag test. ");
  opt.Check();
  
  ImageC<ByteT> image(size,size);
  image.Fill(0);
  if(zigZag) {
    if(label) {
      int i = 1;
      for(ZigZagIterC it(image.Frame());it;it++) 
	image[*it] = i++;
    } else {
      for(ZigZagIterC it(image.Frame());it;it++)
	image[*it]++;
    }
  } else {
    if(label) {
      int i = 1;
      for (SquareIterC it(size/2-1, Index2dC(size/2,size/2)); it.IsElm(); it.Next()) 
	image[it.Data()] = i++;
    } else {
      for (SquareIterC it(size/2-1, Index2dC(size/2,size/2)); it.IsElm(); it.Next()) 
	image[it.Data()]++;
    }
  }
  
  if (!Save("square.pgm", image))
     std::cerr << "Image saving failed" << std::endl;

  return 0;
}

RAVL_ENTRY_POINT(Main);

