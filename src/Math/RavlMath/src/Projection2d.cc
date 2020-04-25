// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlMath
//! date="17/10/2002"
//! author="Charles Galambos"
//! file="Ravl/Math/Geometry/Euclidean/2D/Projection2d.cc"

#include "Ravl/Projection2d.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/Matrix.hh"
#include "Ravl/Vector.hh"
#include "Ravl/DLIter.hh"
#include "Ravl/SArray1dIter2.hh"
#include "Ravl/SArray1dIter3.hh"
#include "Ravl/LeastSquares.hh"

namespace RavlN {
  
  
  //: Get homography
  // This returns the projection normalised so that the projective scales are both = 1
  
  Matrix3dC Projection2dC::Homography() const {
    Matrix3dC mat1(1,0,0,
		   0,1,0,
		   0,0,iz);
    Matrix3dC mat2(1,0,0,
		   0,1,0,
		   0,0,oz);
    Matrix3dC ret = mat2.Inverse() * trans * mat1;
    return ret;
  }

  FAffineC<2> Projection2dC::AffineApproximation() const {
    Matrix3dC htrans = Homography();
    RealT t1 = htrans[0][2] / htrans[2][2];
    RealT t2 = htrans[1][2] / htrans[2][2];
    RealT h1 = htrans[0][0] / htrans[2][2]  - t1 * htrans[2][0];
    RealT h2 = htrans[0][1] / htrans[2][2] - t1 * htrans[2][1];
    RealT h3 = htrans[1][0] / htrans[2][2] - t2 * htrans[2][0];
    RealT h4 = htrans[1][1] / htrans[2][2] - t2 * htrans[2][1];
    return FAffineC<2>(Matrix2dC(h1,h2,h3,h4), Vector2dC(t1,t2));
  }
  //: Get an affine approximation of this projective transform
  //!return: the affine approximation
  


  template <class DataContainerT> // assumed to be a container of Point2dC
  bool FitProjectionTempl(const DataContainerT &from,const  DataContainerT &to,Matrix3dC &proj)
  {
    RavlAssert(from.Size() == to.Size());
    UIntT neq = from.Size();
    if(neq < 4) return false;
    
    // Normalise 'from' points.
    
    DataContainerT fromN;
    Matrix3dC fromNorm;
    Normalise(from,fromN,fromNorm);
    
    // Normalise 'to' points.
    
    DataContainerT toN;
    Matrix3dC toNorm;
    Normalise(to,toN,toNorm);
    
    // Build matrix.
    
    MatrixC A(neq * 2,9);
    IntT i = 0;
    for(typename DataContainerT::IteratorT toIt(toN), frIt(fromN);toIt;toIt++, frIt++) {
      const Point2dC &x = frIt.Data();
      
      SizeBufferAccessC<RealT> row1 = A[i++];
      
      RealT r = toIt.Data()[0];
      RealT c = toIt.Data()[1];
      
      row1[0] = 0;
      row1[1] = 0;
      row1[2] = 0;
      
      row1[3] = x[0] * -1;
      row1[4] = x[1] * -1;
      row1[5] = -1;
      
      row1[6] = x[0] * c;
      row1[7] = x[1] * c;
      row1[8] = c;
      
      SizeBufferAccessC<RealT> row2 = A[i++];
      
      row2[0] = x[0];
      row2[1] = x[1];
      row2[2] = 1;
      
      row2[3] = 0;
      row2[4] = 0;
      row2[5] = 0;
      
      row2[6] = x[0] * -r;
      row2[7] = x[1] * -r;
      row2[8] = -r;
    }
    // Should check the rank of A?
    
    VectorC v;
    if(!LeastSquaresEq0Mag1(A,v))
      return false;
    //cerr << "A=" << A << " V=" << v << "\n";
    Matrix3dC mat(v[0],v[1],v[2],
		  v[3],v[4],v[5],
		  v[6],v[7],v[8]);
    
    toNorm.InvertIP();
    proj =  toNorm * mat * fromNorm;
    return true;
  }
  //: Fit projection to a set of points.  4 or point correspondances are required

  template <class DataContainerT>   // DataContainerT assumed to be a PairC<Point2dC>
  bool FitProjectionTempl(const DataContainerT &matchPair, Matrix3dC &proj)
  {
    UIntT neq = matchPair.Size();
    if(neq < 4) return false;
    
    
    BufferC<Point2dC> mpBuf(matchPair.Size()*2, (Point2dC*)&(matchPair[0]));
    Slice1dC<Point2dC> from(mpBuf, matchPair.Size(), 0, 2);
    Slice1dC<Point2dC> to(mpBuf, matchPair.Size(), 1, 2);

    return FitProjectionTempl(from, to, proj);

  }


  template <class DataContainerT> // DataContainerT must be a container of Point2dC
  Projection2dC FitProjectionTempl(const DataContainerT &org,const DataContainerT &newPos,RealT &residual) {
    
    //for some contianers it is slow to find size so cache it here    
    UIntT orgSize(org.Size());

    RavlAssertMsg(orgSize == newPos.Size(),"Projection2dC FitProjection(), Point arrays must have the same size.");
    // we need at least four points to fit a 2D homography
    if (orgSize < 4)
      throw ExceptionC("Sample size too small in Projection2dC. ");
    RealT zh1 = 1.0,zh2 = 1.0;
    if (orgSize == 4) {
      // FIXME:- Pick better values for zh1 and zh2 !!
      
      // initialise homography P by fitting to four point pairs, assuming that
      // bottom-right element P[2][2] is not zero.

      // Construct 8x8 matrix of linear equations
      MatrixC A(8,8);
      A.Fill(0.0);
      VectorC b(8);

      // distinguish between explicit and implicit forms of point observations
      IntT i=0;
      typename DataContainerT::IteratorT it1(org);
      typename DataContainerT::IteratorT it2(newPos);
      for(;it1;it1++,it2++,i++) {
        RealT x1, y1, x2, y2;
        x1=(*it1)[0]; y1=(*it1)[1];
        x2=(*it2)[0]; y2=(*it2)[1];

        A[i*2][0] = x1*zh2; A[i*2][1] = y1*zh2; A[i*2][2] = zh1*zh2;
        A[i*2][6] = -x1*x2; A[i*2][7] = -y1*x2;
        b[i*2] = zh1*x2;
        A[i*2+1][3] = x1*zh2; A[i*2+1][4] = y1*zh2; A[i*2+1][5] = zh1*zh2;
        A[i*2+1][6] = -x1*y2; A[i*2+1][7] = -y1*y2;
        b[i*2+1] = zh1*y2;
      }
      
      // solve for solution vector
      if(!SolveIP(A,b))
        throw ExceptionNumericalC("Dependent linear equations in Projection2dC FitProjection(). ");
      residual = 0.0;
      Matrix3dC P(b[0], b[1], b[2],
                  b[3], b[4], b[5],
                  b[6], b[7], 1.0);
      return Projection2dC (P,zh1,zh2);
    }
    
    residual = -1.0; // Make it obvious we're not computing it!
    Matrix3dC P(1.0,0.0,0.0,
                0.0,1.0,0.0,
                0.0,0.0,1.0);
    FitProjectionTempl(org,newPos,P);
    return Projection2dC (P,zh1,zh2);
  }  
  //: Fit a projective transform given to the mapping between original and newPos.
  // Note: In the current version of the routine 'residual' isn't currently computed.

  template <class DataContainerT> // Assumes DataContainerT is a container of PairC<Point2dC>
  Projection2dC FitProjectionTempl(const DataContainerT &matchPairs, RealT &residual) {
    
    BufferC<Point2dC> mpBuf(matchPairs.Size()*2, (Point2dC*)&(matchPairs[0]));
    Slice1dC<Point2dC> org(mpBuf, matchPairs.Size(), 0, 2);
    Slice1dC<Point2dC> newPos(mpBuf, matchPairs.Size(), 1, 2);    

    return FitProjectionTempl(org, newPos, residual);
  }  
  //: Fit a projective transform given to the mapping between original and newPos.
  // Note: In the current version of the routine 'residual' isn't currently computed.

  Projection2dC FitProjection(const SArray1dC<PairC<Point2dC> > &matchPairs, RealT &residual)
  {
    return FitProjectionTempl(matchPairs, residual);
  }

  Projection2dC FitProjection(const DListC<Point2dC> &org,const DListC<Point2dC> &newPos,RealT &residual)
  {
    return FitProjectionTempl(org, newPos, residual);
  }

  Projection2dC FitProjection(const SArray1dC<Point2dC> &org,const SArray1dC<Point2dC> &newPos,RealT &residual)
  {
    return FitProjectionTempl(org, newPos, residual);
  }

  //: Fit a projective transform given to the mapping between original and newPos.
  
  Projection2dC FitProjection(const DListC<Point2dC> &org,const DListC<Point2dC> &newPos) {
    RealT residual;
    return FitProjection(org,newPos,residual);
  }
  
  //: Fit a projective transform given to the mapping between original and newPos.
  
  Projection2dC FitProjection(const SArray1dC<Point2dC> &org,const SArray1dC<Point2dC> &newPos) {
    RealT residual;
    return FitProjection(org,newPos,residual);    
  }
  
  


  //: Fit a projective transform given to the mapping between original and newPos with weighting for points.
  
  Projection2dC FitProjection(const SArray1dC<Point2dC> &org,const SArray1dC<Point2dC> &newPos,const SArray1dC<RealT> &weight) {
    RealT zh1 = 1.0,zh2 = 1.0;
    Matrix3dC P(1.0,0.0,0.0,
                0.0,1.0,0.0,
                0.0,0.0,1.0);
    FitProjection(org,newPos,weight,P);
    return Projection2dC (P,zh1,zh2); 
  }
  
  //: Fit projection to a set of points.  4 or point correspondences are required

  bool FitProjection(const SArray1dC<Point2dC> &from,const SArray1dC<Point2dC> &to,const SArray1dC<RealT> &weight,Matrix3dC &proj) {
    RavlAssert(from.Size() == to.Size());
    RavlAssert(from.Size() == weight.Size());
    UIntT neq = from.Size();
    if(neq < 4) return false;
    
    // Normalise 'from' points.
    
    SArray1dC<Point2dC> fromN;
    Matrix3dC fromNorm;
    Normalise(from,fromN,fromNorm);
    
    // Normalise 'to' points.
    
    SArray1dC<Point2dC> toN;
    Matrix3dC toNorm;
    Normalise(to,toN,toNorm);
    
    // Compute normalisation for weights.
    
    RealT meanWeight = weight.Sum() / static_cast<RealT>(weight.Size());
    if(meanWeight <= 0)
      meanWeight = 1;
    
    // Build matrix.
    
    MatrixC A(neq * 2,9);
    IntT i = 0;
    for(SArray1dIter3C<Point2dC,Point2dC,RealT> it(toN,fromN,weight);it;it++) {
      const Point2dC &x = it.Data2();
      RealT w = it.Data3() / meanWeight; // Normalise weight.
      SizeBufferAccessC<RealT> row1 = A[i++];
      
      RealT r = it.Data1()[0] * w;
      RealT c = it.Data1()[1] * w;
      
      row1[0] = 0;
      row1[1] = 0;
      row1[2] = 0;
      
      row1[3] = x[0] * -1 * w;
      row1[4] = x[1] * -1 * w;
      row1[5] = -w;
      
      row1[6] = x[0] * c;
      row1[7] = x[1] * c;
      row1[8] = c;
      
      SizeBufferAccessC<RealT> row2 = A[i++];
      
      row2[0] = x[0] * w;
      row2[1] = x[1] * w;
      row2[2] = w;
      
      row2[3] = 0;
      row2[4] = 0;
      row2[5] = 0;
      
      row2[6] = x[0] * -r;
      row2[7] = x[1] * -r;
      row2[8] = -r;
    }
    
    // Should check the rank of A?
    
    VectorC v;
    if(!LeastSquaresEq0Mag1(A,v))
      return false;
    //cerr << "A=" << A << " V=" << v << "\n";
    Matrix3dC mat(v[0],v[1],v[2],
		  v[3],v[4],v[5],
		  v[6],v[7],v[8]);
    
    toNorm.InvertIP();
    proj =  toNorm * mat * fromNorm;
    return true;    
  }
  

  //: Read from a stream.
  
  istream &operator>>(istream &s,Projection2dC &proj) {
    s >> proj.Matrix() >> proj.IZ() >> proj.OZ();
    return s;
  }

  //: Write to a stream.
  
  ostream &operator<<(ostream &s,const Projection2dC &proj) {
    s << proj.Matrix() << ' ' << proj.IZ() << ' ' << proj.OZ();
    return s;
  }
  
  //: Read from a binary stream.
  
  BinIStreamC &operator>>(BinIStreamC &s,Projection2dC &proj) {
    s >> proj.Matrix() >> proj.IZ() >> proj.OZ();
    return s;
  }
  
  //: Write to a binary stream.
  
  BinOStreamC &operator<<(BinOStreamC &s,const Projection2dC &proj) {
    s << proj.Matrix() << proj.IZ() << proj.OZ();
    return s;    
  }
  
  
}
