// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/Distance/DistanceMahalanobis.cc"

#include "Ravl/PatternRec/DistanceMahalanobis.hh"
#include "Ravl/VirtualConstructor.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/XMLFactoryRegister.hh"

namespace RavlN {


  //: Constructor.
  
  DistanceMahalanobisBodyC::DistanceMahalanobisBodyC(const MatrixC &covVar)
  {
    iCovar = covVar.Inverse();
  }

  //: XMLFactoryC constructor.

  DistanceMahalanobisBodyC::DistanceMahalanobisBodyC(const XMLFactoryContextC &factory)
    : DistanceBodyC(factory)
  {
    MatrixC mat;
    if(!factory.UseComponent("covariance_matrix", mat))
      RavlIssueError("Unable to initialise covar matrix in XML factory constructor");
    iCovar = mat.Inverse();   
  }

  //: Load from stream.
  
  DistanceMahalanobisBodyC::DistanceMahalanobisBodyC(std::istream &strm)
    : DistanceBodyC(strm)
  { strm >> iCovar; }
  
  //: Load from binary stream.
  
  DistanceMahalanobisBodyC::DistanceMahalanobisBodyC(BinIStreamC &strm)
    : DistanceBodyC(strm)
  { strm >> iCovar; }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool DistanceMahalanobisBodyC::Save (std::ostream &out) const {
    if(!DistanceBodyC::Save(out))
      return false;
    out << iCovar << ' ';
    return true;
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool DistanceMahalanobisBodyC::Save (BinOStreamC &out) const {
    if(!DistanceBodyC::Save(out))
      return false;
    out << iCovar;
    return true;
  }
  
  //: Measure the distance from d1 to d2.
  
  RealT DistanceMahalanobisBodyC::Measure(const VectorC &d1,const VectorC &d2) const {
    VectorC x(d1 - d2); 
    return Sqrt(x.Dot(iCovar * x));
  }
  
  //: Measure the magnitude of d1.
  
  RealT DistanceMahalanobisBodyC::Magnitude(const VectorC &x) const {
    return Sqrt(x.Dot(iCovar * x));
  }

  // dS/dX = (X^T CxdI^T + X^T CxdI)/(2 sqrt(X^T CxdI X))
  // using multiplication rule since S = sqrt(X^T CxdI X)
  // where CxdI is the inverted covariance matrix
  
  MatrixC DistanceMahalanobisBodyC::Jacobian (const VectorC &X) const {
    MatrixC V = X;
    RealT divisor = 1/(2.0*Sqrt(X.Dot (iCovar * X)));
    MatrixC dSdX = (V.TMul (iCovar.T()) + V.TMul (iCovar)) * divisor;
    return dSdX;
  }

  //////////////////////////////////////////////////////////////////////
  
  RAVL_INITVIRTUALCONSTRUCTOR_FULL(DistanceMahalanobisBodyC,DistanceMahalanobisC,DistanceC);

  RavlN::XMLFactoryRegisterHandleConvertC<DistanceMahalanobisC, DistanceC> g_registerXMLFactoryDistanceMahalanobis("RavlN::DistanceMahalanobisC");

  extern void linkDistanceMahalanobis()
  {}

  
}
