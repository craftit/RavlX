// This file is part of Ravl
// Copyright (C) 2006, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlMath

#include "Ravl/ThinPlateSpline1d.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {

  //: Warp 'value' 
  // Returns the matching value.
  
  RealT ThinPlateSpline1dC::Apply(const RealT &value) const {
    RealT ret = 0;
    for(BufferAccessIter2C<RealT,RealT > it(refPos,w);it;it++)
      ret += it.Data2() * U(Sqr(value - it.Data1()));
    IntT s = (IntT) refPos.Size();
    ret += w[s++];
    ret += w[s] * value;
    return ret;
  }
  
  //: Design the thin plate spline.
  
  bool ThinPlateSpline1dC::Design(const Array1dC<RealT> &orgPos,const Array1dC<RealT> &newPos) {
    refPos = newPos;
    
    IntT size = (IntT) newPos.Size();
    RavlAssert((IntT) orgPos.Size() == size);
    MatrixC Ld(size+2,size+2);
    w = VectorC(size+2);
    int i;
    for(i = 0;i < size;i++) {
      RealT pos = orgPos[i];
      for(int j = 0;j < size;j++) {
	Ld[i][j] = U(Sqr(pos - newPos[j]));
      }
      Ld[i][size] = 1;
      Ld[i][size+1] = pos;
      
      Ld[size][i] = pos;
      Ld[size+1][i] = 1;
      
      w[i] = newPos[i];
    }
    for(int i = size;i < size+2;i++) {
      w[i] = 0;
      for(int j = size;j < size+2;j++)
	Ld[i][j] = 0;
    }
    ONDEBUG(std::cerr << "Ld=" << Ld << "\n");
    ONDEBUG(std::cerr << "W=" << w << "\n");
    //MatrixC old = Ld.Copy();
    //VectorC ow = w.Copy();
    if(!SolveIP(Ld,w)) {
      std::cerr << "WarpThinPlateSpline1dC::Design(), Failed to compute weight vector. \n";
      return false;
    }
    ONDEBUG(std::cerr << "W=" << w << "\n");
    //cerr << "Ld*w=" << old * w << "\n";
    return true;
  }
  

}
