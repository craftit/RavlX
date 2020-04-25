// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////
//! author="Charles Galambos"
//! rcsid="$Id$"
//! lib=RavlIO
//! file="Ravl/Core/IO/exDataProc.cc"
//! userlevel=Normal
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.Data Processing"

//: Composing a processing pipeline
// The input file for the example should be a white space separated list of numbers

#include "Ravl/Option.hh"
#include "Ravl/DP/FileIO.hh"
#include "Ravl/DP/Compose.hh"

using namespace RavlN;

RealT MultiplyBy2(const RealT &val)
{ return val * 2; }


int main(int argc,char **argv) {  
  OptionC option(argc,argv,true);
  StringC infile = option.String("","in.dat","Input filename");
  StringC outfile = option.String("","out.dat","Output filename");
  option.Check();
  
  DPIFileC<RealT>(infile) >>  Process(MultiplyBy2) >> DPOFileC<RealT>(outfile);
  
  return 1;
}
