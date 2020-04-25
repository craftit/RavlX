// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/Distance/DistanceRobust.cc"

#include "Ravl/PatternRec/DistanceRobust.hh"
#include "Ravl/PatternRec/DistanceEuclidean.hh"
#include "Ravl/VirtualConstructor.hh"
#include "Ravl/SArray1dIter3.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/Stream.hh"
#include "Ravl/SArray1dIter2.hh"
#include "Ravl/XMLFactoryRegister.hh"

namespace RavlN {
  
  //: Constructor.
  
  DistanceRobustBodyC::DistanceRobustBodyC(RealT nClipLimit,const DistanceC &nmetric) 
    : clipLimit(nClipLimit),
      metric(nmetric)
  {}


  DistanceRobustBodyC::DistanceRobustBodyC(const XMLFactoryContextC &factory)
    : DistanceBodyC(factory),
      clipLimit(factory.AttributeReal("clip_limit", 1.0))
  {
    if(!factory.UseComponent("metric", metric))
      RavlIssueError("No metric specified in XML factory");
    
  }

  
  //: Load from stream.
  
  DistanceRobustBodyC::DistanceRobustBodyC(std::istream &strm) 
    : DistanceBodyC(strm)
  { 
    int ver;
    // We might change this in the future, so add version info.
    strm >> ver;
    if(ver != 1)
      std::cerr << "DistanceRobustBodyC::DistanceRobustBodyC(), WARNING: Unknown format version. \n";
    strm >> clipLimit >> metric; 
  }
  
  //: Load from binary stream.
  
  DistanceRobustBodyC::DistanceRobustBodyC(BinIStreamC &strm) 
    : DistanceBodyC(strm)
  { 
    char ver;
    strm >> ver;
    if(ver != 1)
      std::cerr << "DistanceRobustBodyC::DistanceRobustBodyC(), WARNING: Unknown format version. \n";    
    strm >> clipLimit >> metric; 
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool DistanceRobustBodyC::Save (std::ostream &out) const {
    if(!DistanceBodyC::Save(out))
      return false;
    int ver = 1;
    out << ver << ' ' << clipLimit << metric << ' ';
    return true;
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool DistanceRobustBodyC::Save (BinOStreamC &out) const {
    if(!DistanceBodyC::Save(out))
      return false;
    char ver = 1;
    out << ver << clipLimit << metric;    
    return true;
  }
  
  //: Measure the distance from d1 to d2.
  // Each value of d2 is limited to be within 'clipLimit' of d1.
  
  RealT DistanceRobustBodyC::Measure(const VectorC &d1,const VectorC &d2) const {
    VectorC nd2(d2.Size());
    for(SArray1dIter3C<RealT,RealT,RealT> it(nd2,d1,d2);it;it++) {
      if(Abs(it.Data2() - it.Data3()) > clipLimit) {
	if(it.Data2() < it.Data3()) 
	  it.Data1() = it.Data2() + clipLimit;
	else
	  it.Data1() = it.Data2() - clipLimit;
      } else
	it.Data1() = it.Data3();
    }
    return metric.Measure(d1,nd2);
  }
  
  //: Measure the magnitude of d1.
  
  RealT DistanceRobustBodyC::Magnitude(const VectorC &d1) const {
    VectorC vec(d1.Size());
    for(BufferAccessIter2C<RealT,RealT> it(vec,d1);it;it++) {
      if(Abs(it.Data2()) > clipLimit)
	it.Data1() = Sign(it.Data2()) * clipLimit;
      else
	it.Data1() = it.Data2(); 
    }
    return metric.Magnitude(d1);
  }
  
  //: Calculates Jacobian matrix at X
  
  MatrixC DistanceRobustBodyC::Jacobian (const VectorC &X) const {
    VectorC vec(X.Size());
    for(BufferAccessIter2C<RealT,RealT> it(vec,X);it;it++) {
      if(Abs(it.Data2()) > clipLimit)
	it.Data1() = Sign(it.Data2()) * clipLimit;
      else
	it.Data1() = it.Data2(); 
    }
    MatrixC dSdX = metric.Jacobian (vec);
    for(SArray1dIterC<RealT> itr(X);itr;itr++)
      if(Abs(*itr) > clipLimit)
	dSdX[0][itr.Index()] = 0.0;    
    return dSdX;
  }

  //////////////////////////////////////////////////////////////////////
  
  RAVL_INITVIRTUALCONSTRUCTOR_FULL(DistanceRobustBodyC,DistanceRobustC,DistanceC);

  RavlN::XMLFactoryRegisterHandleConvertC<DistanceRobustC, DistanceC> g_registerXMLFactoryDistanceRobust("RavlN::DistanceRobustC");

  extern void linkDistanceRobust()
  {}
             
}
