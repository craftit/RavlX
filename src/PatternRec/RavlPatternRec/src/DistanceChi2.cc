// This file is used in conjunction with RAVL, Recognition And Vision Library
// Copyright (C) 2010, CVSSP.
// This code may be redistributed under the terms of the GNU
// General Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
//! lib=RavlPatternRec

//! author="Teo de Campos"

#include <math.h>

#include "Ravl/PatternRec/DistanceChi2.hh"
#include "Ravl/VectorUtils.hh"
#include "Ravl/StdConst.hh"
#include "Ravl/VirtualConstructor.hh"
#include "Ravl/XMLFactoryRegister.hh"
#include "Ravl/SArray1dIter2.hh"

namespace RavlN {

  //: XMLFactoryC constructor.

  DistanceChi2BodyC::DistanceChi2BodyC(const XMLFactoryContextC &factory)
    : DistanceBodyC(factory)
  {
  }

  //: Load from stream.
  
  DistanceChi2BodyC::DistanceChi2BodyC(std::istream &strm)
    : DistanceBodyC(strm)
  {}
  
  //: Load from binary stream.
  
  DistanceChi2BodyC::DistanceChi2BodyC(BinIStreamC &strm)
    : DistanceBodyC(strm)
  {}
  
  //: Writes object to stream, can be loaded using constructor
  
  bool DistanceChi2BodyC::Save (std::ostream &out) const 
  { return DistanceBodyC::Save(out); }
  
  //: Writes object to stream, can be loaded using constructor

  bool DistanceChi2BodyC::Save (BinOStreamC &out) const 
  { return DistanceBodyC::Save(out); }
  
  //: Measure the distance from d1 to d2.
  
  RealT DistanceChi2BodyC::Measure(const VectorC &d1,const VectorC &d2) const {
    RealT sum(0.0);
    for(SArray1dIter2C<RealT,RealT> it(d1, d2); it; it++){
      RealT bottom(it.Data1()+it.Data2());
      sum += Sqr(it.Data1()-it.Data2())/(bottom!=0.0? bottom : RavlConstN::minReal);
    }
    return .5*sum;
  }
  
  
  //////////////////////////////////////////////////////////////////////
  
  RAVL_INITVIRTUALCONSTRUCTOR_FULL(DistanceChi2BodyC,DistanceChi2C,DistanceC);

  RavlN::XMLFactoryRegisterHandleConvertC<DistanceChi2C, DistanceC> g_registerXMLFactoryDistanceChi2("RavlN::DistanceChi2C");
  
  extern void linkDistanceChi2()
  {}



//----------------------------------------------------
// Function to compute the best Gamma parameter
  RealT ComputeBestChi2Gamma(SampleC<VectorC> smplVec){
    DistanceChi2C dist; 
    RealT sum(0.0);
    for(SampleIterC<VectorC> it(smplVec); it; it++){
      for(SampleIterC<VectorC> itt(smplVec); itt; itt++){
	sum += dist.Measure(it.Data(), itt.Data());
      }
    }
    return(sum/(smplVec.Size()*smplVec.Size()));
  }

} // end of namespace

