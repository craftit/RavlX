// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlDPMT
//! file="Ravl/OS/DataProc/exMTDataProc.cc"
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.Data Processing"
//! userlevel=Normal

#include "Ravl/Option.hh" 
#include "Ravl/DP/FileIO.hh"
#include "Ravl/DP/ProcIStream.hh"
#include "Ravl/DP/Compose.hh"
#include "Ravl/DP/Multiplex.hh"
#include "Ravl/DP/ThreadPipe.hh"
#include "Ravl/DP/MTIOConnect.hh"
#include "Ravl/DP/PlayControl.hh"

using namespace RavlN;

//#include "Ravl/DP/RunningAve.hh"
//#include "Ravl/DP/OffsetScale.hh" 

int main(int argc,char **argv) {  
  OptionC option(argc,argv,true);
  StringC infile = option.String("i","in.dat","Input filename");
  StringC outfile = option.String("o","out.dat","Output filename");
  option.Check();
  
  //  >> DPMultiplex(DPThread(DPOffsetScale(2.0,2.0) >> DPRunningAverage(1.0,5)),4) >>
#if 0
  cerr << "Building Multiplex." << endl;
  //DPIOPortC<RealT,RealT> mult =;  
  DPIOPortC<RealT,RealT> mult = DPThread(DPOffsetScale(2.0,2.0) >> DPRunningAverage(1.0,5));
  cerr << "Building c1." << endl;
  DPMTIOConnectBaseC c1(DPIOConnect(mult,DPOFileC<RealT>(outfile)));
  cerr << "Building c2." << endl;
  DPMTIOConnectBaseC c2(DPIOConnect(DPIFileC<RealT>(infile),mult));
  cerr << "Waiting for processing to complete." << endl;
  c1.Wait(); // Wait for IO to finish.
#else
  DPMTIOConnectBaseC c2(DPMTIOConnect(DPIFileC<RealT>(infile),DPOFileC<RealT>(outfile)));
  c2.Wait();
  
  DPIPlayControlC<int> x;
#endif  

  return 1;
}

