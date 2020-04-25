// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlImageProc
//! author="Charles Galambos"
//! date="22/11/2002"
//! file="Ravl/Image/Processing/Filters/WarpThinPlateSpline.cc"

#include "Ravl/Image/WarpThinPlateSpline.hh"
#include "Ravl/MatrixRS.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlImageN {
  
  //: Calculate Ud.
  
  void WarpThinPlateSplineBaseC::ComputeWUd(const Array1dC<Point2dC> &orgPos,const Point2dC &pos,const MatrixC &w,Point2dC &at) const {
    RealT &x = at[0];
    RealT &y = at[1];
    x = 0;
    y = 0;
    for(BufferAccessIter2C<Point2dC,BufferAccessC<RealT> > it(orgPos,w);it;it++) {
      RealT uv = U(pos.SqrEuclidDistance(it.Data1()));
      x += it.Data2()[0] * uv;
      y += it.Data2()[1] * uv;
    }
    IntT s = (IntT) orgPos.Size();
    x += w[s][0];
    y += w[s][1];
    s++;
    x += w[s][0] * pos[0];
    y += w[s][1] * pos[0];
    s++;
    x += w[s][0] * pos[1];
    y += w[s][1] * pos[1];
  }
  
  //: Compute weight matrix.
  
  MatrixC WarpThinPlateSplineBaseC::ComputeW(const Array1dC<Point2dC> &orgPos,const Array1dC<Point2dC> &newPos) const {
    ONDEBUG(cerr << "WarpThinPlateSplineBaseC::ComputeW(), Sigma=" << sigma << " Size="  << newPos.Size() << "\n");
    IntT size = (IntT) newPos.Size();
    RavlAssert((IntT) orgPos.Size() == size);
    MatrixC Ld(size+3,size+3);
    MatrixC w(size+3,2);
    int i;
    for(i = 0;i < size;i++) {
      const Point2dC &pos = orgPos[i];
      for(int j = 0;j < size;j++) {
	Ld[i][j] = U(pos.SqrEuclidDistance(orgPos[j]));
      }
      Ld[i][size] = 1;
      Ld[i][size+1] = pos[0];
      Ld[i][size+2] = pos[1];
      
      Ld[size][i] = pos[1];
      Ld[size+1][i] = pos[0];
      Ld[size+2][i] = 1;
      
      const Point2dC &npos = newPos[i];
      w[i][0] = npos[0];
      w[i][1] = npos[1];
    }
    ONDEBUG(cerr << "Ld=" << Ld << "\n");
    for(i = size;i < size+3;i++) {
      w[i][0] = 0;
      w[i][1] = 0;
      
      for(int j = size;j < size+3;j++)
	Ld[i][j] = 0;
    }
    if(!SolveIP(Ld,w)) {
      cerr << "WarpThinPlateSplineBaseC::ComputeW(), Failed to compute weight matrix. \n";
      return MatrixC();
    }
    return w;
  }
  
}
