// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! author="Charles Galambos"
//! file="Ravl/PatternRec/Classify/ClassifierBayesNormalLinear.cc"

#include "Ravl/PatternRec/ClassifierBayesNormalLinear.hh"
#include "Ravl/SArray1dIter3.hh"
#include "Ravl/VirtualConstructor.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/DP/FileFormatStream.hh"
#include "Ravl/DP/FileFormatBinStream.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/config.h"

#if RAVL_COMPILER_MIPSPRO 
#include "Ravl/VirtualConstructor.hh"
#pragma instantiate RavlN::ClassifierBayesNormalLinearBodyC* RavlN::VCLoad(RavlN::BinIStreamC&,RavlN::ClassifierBayesNormalLinearBodyC*)
#pragma instantiate RavlN::ClassifierBayesNormalLinearBodyC* RavlN::VCLoad(std::istream &,RavlN::ClassifierBayesNormalLinearBodyC*)
#endif 

namespace RavlN {

  //: Default constructor.
  
  ClassifierBayesNormalLinearBodyC::ClassifierBayesNormalLinearBodyC()
  {}
  
  //: Default constructor.
  
  ClassifierBayesNormalLinearBodyC::ClassifierBayesNormalLinearBodyC(const SArray1dC<VectorC> & means, const MatrixC  & covariance, const SArray1dC<RealT> & priors)
    : ClassifierBodyC(means.Size()),
      mean(means),
      invCov(covariance.Inverse()),
      konst(priors.Size())
  {     
    //: Lets compute the constant
    for(SArray1dIter3C<RealT, VectorC, RealT> it(konst, mean, priors);it;it++) 
      it.Data1() = (-0.5 *  invCov.TMul(it.Data2()).Dot(it.Data2())) + Log(it.Data3());
  }
  
  //: Load from stream.
  
  ClassifierBayesNormalLinearBodyC::ClassifierBayesNormalLinearBodyC(std::istream &strm) 
    : ClassifierBodyC(strm)
  { strm >> mean >> invCov >> konst; }
  
  //: Load from binary stream.
  
  ClassifierBayesNormalLinearBodyC::ClassifierBayesNormalLinearBodyC(BinIStreamC &strm) 
    : ClassifierBodyC(strm)
  { strm >> mean >> invCov >> konst; }
  
  //: Writes object to stream.
  
  bool ClassifierBayesNormalLinearBodyC::Save (std::ostream &out) const {
    if(!ClassifierBodyC::Save(out))
      return false;
    out << ' ' << mean << ' ' << invCov << ' ' << konst;
    return true;    
  }
  
  //: Writes object to binary stream.
  
  bool ClassifierBayesNormalLinearBodyC::Save (BinOStreamC &out) const {
    if(!ClassifierBodyC::Save(out))
      return false;
    out << mean << invCov << konst;
    return true;
  }
  
  //: Classify sample 
  UIntT ClassifierBayesNormalLinearBodyC::Classify(const VectorC &data) const {
    VectorC res(konst.Size());
    for(SArray1dIter3C<RealT, VectorC, RealT>it(res, mean,  konst);it;it++) {
      it.Data1() = invCov.TMul(it.Data2()).Dot(data) + it.Data3();
    }
    return res.MaxIndex().V();
  }
  
  //: Return normalised prob vector
  VectorC ClassifierBayesNormalLinearBodyC::Apply(const VectorC & data) const
  {
    VectorC res(konst.Size());
    for (SArray1dIter3C<RealT, VectorC, RealT> it(res, mean, konst); it; it++) {
      it.Data1() = invCov.TMul(it.Data2()).Dot(data) + it.Data3();
    }
    return res.MakeUnit();
  }

  ///////////////////////////////////////////////////////////
  
  
  RAVL_INITVIRTUALCONSTRUCTOR_FULL(ClassifierBayesNormalLinearBodyC,ClassifierBayesNormalLinearC,ClassifierC);



}
