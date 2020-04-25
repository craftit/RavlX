// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////
//! author="Charles Galambos"
//! rcsid="$Id$"
//! lib=RavlDPMT
//! docentry="Ravl.API.Core.Data Processing.Split"
//! date="21/07/1998"
//! file="Ravl/OS/DataProc/exDPSplit.cc"

#include "Ravl/Option.hh"
#include "Ravl/DP/FileIO.hh"
#include "Ravl/DP/OffsetScale.hh"
#include "Ravl/DP/Compose.hh"
#include "Ravl/DP/SplitI.hh"
#include "Ravl/DP/MTIOConnect.hh"

using namespace RavlN;

RealT Monitor(const RealT &val) {
  //cerr << val << " " ;
  return val;
}

int main(int argc,char **argv) {  
  OptionC option(argc,argv,true);
  StringC infile = option.String("i","in.dat","Input filename");
  StringC outfile = option.String("o","out.dat","Output filename");
  option.Check();
  DPSplitIC<RealT> split;
  
  cout << "Setting up pipes.\n";
  
  DPEventC event1 = DPIFileC<RealT>(infile,false) >> Process(&Monitor) >> split >> DPOffsetScale(2.0,2.0) >>= DPOFileC<RealT>(outfile);
  DPEventC event2 = split >> DPOffsetScale(2.0,2.0) >>= DPOFileC<RealT>(outfile+".xxx");
  cout << "Waiting for processing to complete.\n";
  
  event1.Wait();
  event2.Wait();
  cout << "Done.\n";
  return 1;
}
