// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/Geometry/Projective/2D/FundamentalMatrix2d.cc"

#include "Ravl/config.h"
#include "Ravl/Matrix.hh"
#include "Ravl/Vector.hh"
#include "Ravl/SArray1dIter.hh"
#include "Ravl/SArray1dIter2.hh"
#include "Ravl/SArray1dIter3.hh"
#include "Ravl/LineABC2d.hh"
#include "Ravl/Vector3d.hh"
#include "Ravl/LeastSquares.hh"
#include "Ravl/FundamentalMatrix2d.hh"
#include "Ravl/SVD.hh"
#include "Ravl/Complex.hh"
#include "Ravl/DList.hh"
#include "Ravl/Eigen.hh"

#define DODEBUG 0

#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {

  //: Compute the fundamental matrix from 2 sets of points.
  // This computes tbe fundamental matrix using a linear method (Sometimes called the normalised 8-point alogorithm),
  // you need least 8 points, but more may be used to obtain a least squares fit.

  FundamentalMatrix2dC FundamentalMatrix2dC::FitLinear(const SArray1dC<PPoint2dC> &pnts1,const SArray1dC<PPoint2dC> &pnts2) {
    Vector2dC offset1,offset2;
    RealT scale1,scale2;

    NormalisePointSet(pnts1,offset1,scale1);
    NormalisePointSet(pnts2,offset2,scale2);

    MatrixC A = BuildDesignMatrix(pnts2,offset2,scale2,
				  pnts1,offset1,scale1);

    Matrix3dC norm1(1/scale1,0       ,-offset1[0]/scale1,
		    0       ,1/scale1,-offset1[1]/scale1,
		    0       ,0       ,1);

    Matrix3dC norm2(1/scale2,0       ,-offset2[0]/scale2,
		    0       ,1/scale2,-offset2[1]/scale2,
		    0       ,0       ,1);

    ONDEBUG(cerr << "Norm1=" << norm1 << "\n");
    ONDEBUG(cerr << "Norm2=" << norm2 << "\n");

    VectorC v;
    LeastSquaresEq0Mag1(A,v);

    FundamentalMatrix2dC ret(v[0],v[1],v[2],
			     v[3],v[4],v[5],
			     v[6],v[7],v[8]);

    ret.MakeRank2();

    ((Matrix3dC &)ret) = norm2.TMul(ret) * norm1; // Undo normalisation.

    ret.NormaliseScale();

    return ret;
  }

  //: Normalise scale.

  void FundamentalMatrix2dC::NormaliseScale() {
    // Normalise magnitude.
    RealT norm = 1/Sqrt(SumOfSqr());
    (*this) *= norm;
  }

  //: Compute the error (distance from line) of p2 with respect to p1 in the first image.

  RealT FundamentalMatrix2dC::Error(const PPoint2dC &p1,const PPoint2dC &p2) const {
    PLine2dC lp =  (*this)(p1);
    return Abs(p2[0]*lp[0] + p2[1]*lp[1] + p2[2]*lp[2]) / Sqrt(Sqr(lp[0]) + Sqr(lp[1]));
  }

  //: Compute the epipolar line in image 2 from 'p1' in image 1.

  LineABC2dC FundamentalMatrix2dC::EpipolarLine(const PPoint2dC &p1) const {
    PLine2dC l = (*this)(p1);
    return LineABC2dC(l[0],l[1],l[2]);
  }

  //: Make the matrix rank 2
  // Uses SVD method.

  void FundamentalMatrix2dC::MakeRank2() {
    MatrixC u, v;
    MatrixC me(*this);
    VectorC d = SVD_IP(me,u,v);
    // Generate rank 2 matrix.
    MatrixC newf = (u * MatrixC(d[0],0   ,0,
				0   ,d[1],0,
				0   ,0   ,0)).MulT(v);

    // Copy the result back into this matrix.
    for(int i = 0;i < 3;i++)
      for(int j = 0;j < 3;j++)
	(*this)[i][j] = newf[i][j];
  }

  //: Build a design matrix given two sets of points.

  MatrixC FundamentalMatrix2dC::BuildDesignMatrix(const SArray1dC<PPoint2dC> &pnts1,const Vector2dC &offset1,RealT scale1,
						  const SArray1dC<PPoint2dC> &pnts2,const Vector2dC &offset2,RealT scale2) {
    RavlAssert(pnts1.Size() == pnts2.Size());
    UIntT size = pnts1.Size();
    if(size < 9)
      size = 9;
    MatrixC A(size,9);
    IndexC i = 0;
    PPoint2dC p1,p2;
    for(SArray1dIter2C<PPoint2dC,PPoint2dC> it(pnts1,pnts2);it;it++,i++) {
      p1[0] = (it.Data1()[0] - offset1[0]) / scale1;
      p1[1] = (it.Data1()[1] - offset1[1]) / scale1;
      p1[2] = it.Data1()[2];
      p2[0] = (it.Data2()[0] - offset2[0]) / scale2;
      p2[1] = (it.Data2()[1] - offset2[1]) / scale2;
      p2[2] = it.Data2()[2];
      ONDEBUG(cerr << "P1=" << p1 << "     P2=" << p2 << "\n");
      A[i][0] = p1[0] * p2[0];
      A[i][1] = p1[0] * p2[1];
      A[i][2] = p1[0] * p2[2];
      A[i][3] = p1[1] * p2[0];
      A[i][4] = p1[1] * p2[1];
      A[i][5] = p1[1] * p2[2];
      A[i][6] = p1[2] * p2[0];
      A[i][7] = p1[2] * p2[1];
      A[i][8] = p1[2] * p2[2];
    }
    // Hack to make sure design matrix is at least 9x9
    // Copy first row into last to make sure
    while(size > pnts1.Size()) {
      size--;
      for(i = 0;i < 9;i++)
	A[size][i] = A[0][i];
    }
    return A;
  }

  //: Normalise points to have a mean of zero and have an magnitude of 1

  void FundamentalMatrix2dC::NormalisePointSet(const SArray1dC<PPoint2dC> &pnts,Vector2dC &offset,RealT &scale) {
    // Compute the mean position.
    offset = Vector2dC(0,0);
    scale = 0;
    RealT mscale = 0;
    for(SArray1dIterC<PPoint2dC> it(pnts);it;it++) {
      //cerr << "Point=" << *it << "\n";
      Point2dC p = it->Point2d();
      mscale += (*it)[2];
      offset += p;
    }
    offset /= (RealT) pnts.Size();
    mscale /= (RealT) pnts.Size();

    // Go through and subtract the mean.
    for(SArray1dIterC<PPoint2dC> it(pnts);it;it++) {
      Point2dC p((*it)[0] - offset[0],(*it)[1] - offset[1]);
      scale += Sqrt(p.SumOfSqr());
    }
    scale *= mscale / (RealT) pnts.Size();
    ONDEBUG(cerr << "FundamentalMatrix2dC::NormalisePointSet(), Offset=" << offset << " Scale=" << scale << "\n");
  }

  //: Compute the epipoler point in the second image.
  // This is the point where the line connecting the two opical centres
  // intersect the second image plane.

  PPoint2dC FundamentalMatrix2dC::Epipole1() const {
    FundamentalMatrix2dC tp(T());
    PPoint2dC p1(1,2,3);
    PPoint2dC p2(2,3,1);
    PPointLine2dC l1 = tp(p1);
    PPointLine2dC l2 = tp(p2);
    return PPoint2dC(l1.Intersection(l2));
  }

  //: Compute the epipoler point in the first image.
  // This is the point where the line connecting the two opical centres
  // intersect the first image plane.

  PPoint2dC FundamentalMatrix2dC::Epipole2() const {
    PPoint2dC p1(1,2,3);
    PPoint2dC p2(2,3,1);
    PPointLine2dC l1 = (*this)(p1);
    PPointLine2dC l2 = (*this)(p2);
    return PPoint2dC(l1.Intersection(l2));
  }

  //: Create a transform that will move the specified pnt to the origin
  // Used by CorrectCorrespondence

  static Matrix3dC CreateTransformToOrigin(const PPoint2dC &pnt) {
    return Matrix3dC(1, 0, -pnt.P1()/pnt.P3(),
                      0, 1, -pnt.P2()/pnt.P3(),
                      0, 0, 1);
  }

  //: Measured a normalised epipole for F
  // Used by CorrectCorrespondence

  static Vector3dC NormalizedEpipole(const Matrix3dC &F) {
    SVDC<RealT> svd(F, false, true);
    Slice1dC<RealT> col = svd.GetV().SliceColumn(2);
    Vector3dC v(col[0], col[1], col[2]);
    RealT sum = Sqrt(Sqr(v[0]) + Sqr(v[1]));
    return v / sum;
  }

  //: Create a rotation matrix based on the given normalised epipole
  // Used by CorrectCorrespondence

  static Matrix3dC FormRotation(const Vector3dC &e) {
    return Matrix3dC(e[0], e[1], 0,
                      -e[1], e[0], 0,
                      0, 0, 1);
  }

  //: Calculate the roots of the polynomial define in the sequence
  // p: p[0] * x**n + p[1] * x**(n-1) + ... + p[n-1] * x + p[n]
  // Ported from: http://mail.python.org/pipermail/matrix-sig/1999-April/002736.html

  static SArray1dC<ComplexC> FindRoots(const SArray1dC<RealT> &p) {
    IntT N = p.Size();
    SArray1dC<ComplexC> result(N-1);
    result.Fill(ComplexC());

    DListC<SizeT> nz;
    for (SArray1dIterC<RealT> it(p); it; it++) {
      if (*it != 0.0)
        nz.InsLast(it.Index().V());
    }

    if (nz.Size() == 0)
      return result;

    // Strip leading zeros and trailing zeros
    // Save trailing zeros as roots
    SizeT dim = nz.Last() - nz.First() + 1;
    SizeT offset = nz.First();
    SArray1dC<RealT> subP (p, dim, offset);
    result = SArray1dC<ComplexC>(N-nz.Last()-1);

    // Form companion matrix (eigenvalues are roots)
    IntT n = subP.Size();
    if (n > 1) {
       MatrixC a(n-1, n-1);
       a.Fill(0.0);
       a.SetSubArray(Index2dC(1,0), MatrixC::Identity(n-2));
       a.SetRow(0, subP.From(1, n-1) / (-subP[0]));
       EigenValueC<RealT> eig(a);
       SArray1dC<ComplexC> roots(n-1);
       for (SArray1dIter3C<ComplexC, RealT, RealT> it(roots, eig.EigenValues(), eig.ImgEigenValues()); it; it++) {
         it.Data1() = ComplexC(it.Data2(), it.Data3());
       }
       result.Append(roots);
    }

    return result;
  }

  //: Total squared correspondence distance
  // Expression 12.5 on p317 of Multiple View Geometry, 2nd ed

  static RealT Distance(RealT t, RealT a, RealT b, RealT c, RealT d, RealT f, RealT fp) {
    return Pow(t,2) / (1 + Pow(f,2)*Pow(t,2)) + Pow((c*t + d),2) / (Pow((a*t + b),2) + Pow(fp,2)*Pow((c*t +d),2));
  }

  //: Compute corrected correspondences that minimize geometric error.
  // Subject to epipolar constraint x^TFx = 0. Implemented from Multiple View Geometry, 2nd ed,
  // Richard Hartley and Andrew Zisserman, Cambridge University Press, 2003, p318

  void FundamentalMatrix2dC::CorrectCorrespondence(const PPoint2dC &ipnt1,
                                                         const PPoint2dC &ipnt2,
                                                         PPoint2dC &opnt1,
                                                         PPoint2dC &opnt2)
  {
    // Step (i)
    Matrix3dC T = CreateTransformToOrigin(ipnt1);
    Matrix3dC Tp = CreateTransformToOrigin(ipnt2);

    // Step (ii)
    Matrix3dC F = Tp.I().T() * (*this) * T.I();

    // Step (iii)
    Vector3dC e = NormalizedEpipole(F);
    Vector3dC ep = NormalizedEpipole(F.T());

    // Step (iv)
    Matrix3dC R = FormRotation(e);
    Matrix3dC Rp = FormRotation(ep);

    // Step (v)
    F = Rp * F * R.T();

    // Step (vi)
    RealT f = e[2];
    RealT fp = ep[2];
    RealT a = F[1][1];
    RealT b = F[1][2];
    RealT c = F[2][1];
    RealT d = F[2][2];

    // cache some precomputed terms
    RealT a2 = a*a;
    RealT b2 = b*b;
    RealT c2 = c*c;
    RealT d2 = d*d;
    RealT f2 = f*f;
    RealT f4 = Pow(f,4);
    RealT fp2 = fp*fp;
    RealT fp4 = Pow(fp,4);

    // Step (vii)
    SArray1dC<RealT> p(7);
    p[0] = a*b*c2*f4 - a2*c*d*f4;
    p[1] = -a2*d2*f4 + b2*c2*f4 + Pow(c,4)*fp4 + 2*a2*c2*fp2 + Pow(a,4);
    p[2] = -a*b*d2*f4 + b2*c*d*f4 + 4*Pow(c,3)*d*fp4 - 2*a2*c*d*f2 + 4*a2*c*d*fp2 + 2*a*b*c2*f2 + 4*a*b*c2*fp2 + 4*Pow(a,3)*b;
    p[3] = 6*c2*d2*fp4 - 2*a2*d2*f2 + 2*a2*d2*fp2 + 8*a*b*c*d*fp2 + 2*b2*c2*f2 + 2*b2*c2*fp2 + 6*a2*b2;
    p[4] = 4*c*Pow(d,3)*fp4 - 2*a*b*d2*f2 + 4*a*b*d2*fp2 + 2*b2*c*d*f2 + 4*b2*c*d*fp2 - a2*c*d + 4*a*Pow(b,3) + a*b*c2;
    p[5] = Pow(d,4)*fp4 + 2*b2*d2*fp2 - a2*d2 + Pow(b,4) + b2*c2;
    p[6] = -a*b*d2 + b2*c*d;

    SArray1dC<ComplexC> roots = FindRoots(p);

    // Step (viii)
    RealT cMin = 1/f2 + c2/(a2 + fp2*c2);
    //cost, cMin = create_cost(a, b, c, d, f, fp)
    RealT tMin = RavlConstN::maxReal;
    for (SArray1dIterC<ComplexC> root(roots); root; root++) {
      if (root->Im() == 0.0) {
        RealT thisCost = Distance(root->Re(), a, b, c, d, f, fp);
        if (thisCost < cMin) {
          cMin = thisCost;
          tMin = root->Re();
        }
      }
    }

    // Step (ix)
    PLine2dC l(tMin*f, 1, -tMin);
    PLine2dC lp(-fp*(c*tMin + d), a*tMin + b, c*tMin + d);

    PPoint2dC xCap = l.ClosestPointToOrigin();
    PPoint2dC xpCap = lp.ClosestPointToOrigin();

    // Step (xi)
    opnt1 = T.I() * R.T() * xCap;
    opnt2 = Tp.I() * Rp.T() * xpCap;
  }

}
