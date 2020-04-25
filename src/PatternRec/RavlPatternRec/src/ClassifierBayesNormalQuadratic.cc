// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! author="Kieron Messer"
//! file="Ravl/PatternRec/Classify/ClassifierBayesNormalQuadratic.cc"

#include "Ravl/PatternRec/ClassifierBayesNormalQuadratic.hh"
#include "Ravl/SArray1dIter4.hh"
#include "Ravl/SArray1dIter5.hh"
#include "Ravl/VirtualConstructor.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/DP/FileFormatStream.hh"
#include "Ravl/DP/FileFormatBinStream.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/config.h"

#if RAVL_COMPILER_MIPSPRO 
#include "Ravl/VirtualConstructor.hh"
#pragma instantiate RavlN::ClassifierBayesNormalQuadraticBodyC* RavlN::VCLoad(RavlN::BinIStreamC&,RavlN::ClassifierBayesNormalQuadraticBodyC*)
#pragma instantiate RavlN::ClassifierBayesNormalQuadraticBodyC* RavlN::VCLoad(std::istream &,RavlN::ClassifierBayesNormalQuadraticBodyC*)
#endif 

namespace RavlN {

  
  //: Default constructor.
  
  ClassifierBayesNormalQuadraticBodyC::ClassifierBayesNormalQuadraticBodyC(const SArray1dC<MeanCovarianceC> & classStats, const SArray1dC<RealT> & priors)
    : ClassifierBodyC(classStats.Size()),
      mean(classStats.Size()),
      invCov(classStats.Size()),
      konst(classStats.Size())
  {     
    //: Lets compute the constant
    for(SArray1dIter5C<VectorC, MatrixC, RealT, MeanCovarianceC, RealT>it(mean, invCov, konst, classStats, priors);it;it++) {
      it.Data1() = it.Data4().Mean();
      it.Data2() = it.Data4().Covariance().Inverse();
      RealT det = it.Data4().Covariance().Det();
      it.Data3() = (-0.5  *  Log(det)) + Log(it.Data5());
    }
  }
  
  //: Load from stream.
  
  ClassifierBayesNormalQuadraticBodyC::ClassifierBayesNormalQuadraticBodyC(std::istream &strm) 
    : ClassifierBodyC(strm)
{ strm >> mean >> invCov >> konst; }
  
  //: Load from binary stream.
  
  ClassifierBayesNormalQuadraticBodyC::ClassifierBayesNormalQuadraticBodyC(BinIStreamC &strm) 
    : ClassifierBodyC(strm)
{ strm >> mean >> invCov >> konst; }
  
  //: Writes object to stream.
  
  bool ClassifierBayesNormalQuadraticBodyC::Save (std::ostream &out) const {
    if(!ClassifierBodyC::Save(out))
      return false;
    out << ' ' << mean << ' ' << invCov << ' ' << konst;
    return true;    
  }
  
  //: Writes object to binary stream.
  
  bool ClassifierBayesNormalQuadraticBodyC::Save (BinOStreamC &out) const {
    if(!ClassifierBodyC::Save(out))
      return false;
    out << mean << invCov << konst;
    return true;
  }
  
  //: Classify sample 
  UIntT ClassifierBayesNormalQuadraticBodyC::Classify(const VectorC &data) const {
    VectorC res(konst.Size());
    for(SArray1dIter4C<RealT, VectorC, MatrixC, RealT>it(res, mean,  invCov, konst);it;it++) {
      VectorC cent = data - it.Data2();
      it.Data1() = -0.5 * it.Data3().TMul(cent).Dot(cent) + it.Data4();
    }
    return res.MaxIndex().V();
  }
  
  //: Classify sample
  VectorC ClassifierBayesNormalQuadraticBodyC::Apply(const VectorC &data) const
  {
    VectorC res(konst.Size());
    for (SArray1dIter4C<RealT, VectorC, MatrixC, RealT> it(res, mean, invCov, konst); it; it++) {
      VectorC cent = data - it.Data2();
      it.Data1() = -0.5 * it.Data3().TMul(cent).Dot(cent) + it.Data4();
    }
    return res.MakeUnit();
  }

  ///////////////////////////////////////////////////////////
  
  
  RAVL_INITVIRTUALCONSTRUCTOR_FULL(ClassifierBayesNormalQuadraticBodyC,ClassifierBayesNormalQuadraticC,ClassifierC);



}
