// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlImageProc
//! file="Ravl/Image/Processing/Lines/doArcs.cc"

#include "Ravl/config.h"
#include "Ravl/Image/ArcDetector.hh"
#include "Ravl/Image/EdgeDetector.hh"
#include "Ravl/Option.hh"
#include "Ravl/DP/SequenceIO.hh"

using namespace RavlImageN;
using namespace RavlN;

#if RAVL_COMPILER_MIPSPRO 
static SArray1dIterC<EdgelC> sgi_bug_fix1 ;
#endif 


int main(int nargs,char **argv) {
  OptionC option(nargs,argv);
  bool verb = option.Boolean("v",false,"Verbose. ");
  StringC inType = option.String("it","","Input file type. ");
  StringC outType = option.String("ot","","Output file type. ");
  RealT hystLower = option.Real("hl",10,"Lower hysterisis threshold. ");
  RealT hystUpper = option.Real("hu",17,"Upper hysterisis threshold. ");
  StringC inFile = option.String("","img.pgm","Input filename. ");
  StringC outFile = option.String("","edges.abs","Output filename");
  option.Check();
  
  DPIPortC<ImageC<RealT> > in;
  if(!OpenISequence(in,inFile,inType,verb)) {
    cerr << "Failed to open input '" << inFile << "' \n";
    return 1;
  }
  
#if 0
  DPIPortC<DListC<Arc2dC> > out;
  if(!OpenOSequence(in,outFile,outType,verb)) {
    cerr << "Failed to open input '" << inFile << "' \n";
    return 1;
  }
#endif
  
  // Setup edge detector.
  EdgeDetectorC edgeDet(true,hystLower,hystUpper);

  // Setup arc detector.
  ArcDetectorC arcDet;
  
  ImageC<RealT> img;
  EdgeLinkC edgeLnk;
  while(1) {
    if(!in.Get(img))
      break;
    edgeDet.Apply(img,edgeLnk);
    DListC<DListC<Index2dC> > edgeChains = edgeLnk.LinkEdges();
    cerr << "Edge chains=" << edgeChains.Size() << "\n";
    DListC<Arc2dSegmentC> arcs = arcDet.Apply(edgeChains);
    cerr << "Found " << arcs.Size() << " arcs:\n";
#if 1
    for(DLIterC<Arc2dSegmentC> it(arcs);it;it++) {
      cerr << " " << it->Range() << "\n";
    }
#endif
  }
  cerr << "Done sequence. \n";
  return 0;
}
