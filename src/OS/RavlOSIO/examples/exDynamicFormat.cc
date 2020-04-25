// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlOSIO
//! docentry="Ravl.API.OS.Dynamic Linking"
//! author="Charles Galambos"
//! file="Ravl/OS/IO/exDynamicFormat.cc"

#include "Ravl/IO.hh"
#include "Ravl/Array2d.hh"
#include "Ravl/Option.hh"
#include "Ravl/DP/DynamicIO.hh"

using namespace RavlN;

int main(int nargs,char **argv) {
  OptionC opt(nargs,argv);
  StringC inf = opt.String("","in.ppm","File to load. \n");
  StringC outf = opt.String("","@X","File to save. \n");
  opt.Check();
  
  InitDynamicIO();
  
  Array2dC<RealT> img;
  if(!RavlN::Load(inf,img))
    cerr << "Failed to load "<< inf << "\n";
  if(!RavlN::Save(outf,img))
    cerr << "Failed to Save "<< outf << "\n";
  return 0;
}
