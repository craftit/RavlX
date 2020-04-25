// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlImageProc
//! file="Ravl/Image/Processing/Tools/SobolImage.cc"

#include "Ravl/Image/SobolImage.hh"
#include "Ravl/SobolSequence.hh"
#include "Ravl/SquareIter.hh"

namespace RavlImageN {
  
  ImageC<UIntT> SobolImage(const ImageRectangleC &irect) {
    ImageC<UIntT> ret(irect);
    ret.Fill(0);
    int seqn = 1,togo = irect.Area();
    int maxSize = Max(irect.Cols(),irect.Rows());
    for(SobolSequenceC seq(2);seq.IsElm() && seqn < togo;seq.Next(),seqn++) {
      SArray1dC<RealT> s = seq.Data();
      Index2dC at((IndexC) ((RealT) s[0] * irect.Rows()) + irect.Origin().Row(),(IndexC) ((RealT)s[1] * irect.Cols())+ irect.Origin().Col());
      if(ret[at] == 0) { // Has pixel been set ?
	ret[at] = seqn;
	continue;
      }
      // If so the look for closest that isn't.
      bool done = false;
      for(SquareIterC it(maxSize,at);it;it++) {
	if(!ret.Frame().Contains(it.Data()))
	  continue;
	if(ret[*it] == 0) {
	  ret[*it] = seqn;
	  done = true;
	  break;
	}
      }
      if(!done) 
	cerr << "ERROR: Sobol fill missed pixel! \n";
    }
    return ret;
  }
}
