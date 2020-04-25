// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlPatternRec
//! author="Kieron Messer"
//! file="Ravl/PatternRec/Classify/DesignBayesNormalQuadratic.cc"

#include "Ravl/PatternRec/DesignBayesNormalQuadratic.hh"
#include "Ravl/PatternRec/SampleVector.hh"
#include "Ravl/PatternRec/SampleIter.hh"
#include "Ravl/PatternRec/ClassifierBayesNormalQuadratic.hh"
#include "Ravl/MatrixRUT.hh"
#include "Ravl/MatrixRS.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/VirtualConstructor.hh"
#include "Ravl/PatternRec/DataSetVectorLabel.hh"
#include "Ravl/SArray1dIter.hh"
#include "Ravl/SArray1dIter2.hh"
#include "Ravl/Exception.hh"
#include "Ravl/XMLFactoryRegister.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {

  DesignBayesNormalQuadraticBodyC::DesignBayesNormalQuadraticBodyC()
   : equalPriors(false)
  {}

  //: Create least squares designer.
  
  DesignBayesNormalQuadraticBodyC::DesignBayesNormalQuadraticBodyC(const SArray1dC<RealT> & p) 
    : priors(p),
      equalPriors(false)
  {}

  DesignBayesNormalQuadraticBodyC::DesignBayesNormalQuadraticBodyC(bool equalP) 
    : equalPriors(equalP)
  {}
  
  DesignBayesNormalQuadraticBodyC::DesignBayesNormalQuadraticBodyC(const XMLFactoryContextC & factory) :
      DesignClassifierSupervisedBodyC(factory), equalPriors(factory.AttributeBool("equalPriors", true))
  {
    if (!equalPriors) {
      throw ExceptionBadConfigC("Non-equal priors not supported by XMLFactory constructor!");
    }
  }

  //: Load from stream.
  
  DesignBayesNormalQuadraticBodyC::DesignBayesNormalQuadraticBodyC(std::istream &strm)
    : DesignClassifierSupervisedBodyC(strm),
      equalPriors(false)
  {
    int ver;
    strm >> ver;
    if(ver < 1 || ver > 2)
      throw RavlN::ExceptionUnexpectedVersionInStreamC("DesignBayesNormalQuadraticBodyC");
    strm >> priors;
    if(ver > 1)
      strm >> equalPriors;
  }
  
  //: Load from binary stream.
  
  DesignBayesNormalQuadraticBodyC::DesignBayesNormalQuadraticBodyC(BinIStreamC &strm)
    : DesignClassifierSupervisedBodyC(strm),
      equalPriors(false)
  {
    char ver;
    strm >> ver;
    if(ver < 1 || ver > 2)
      throw RavlN::ExceptionUnexpectedVersionInStreamC("DesignBayesNormalQuadraticBodyC");
    strm >> priors;
    if(ver > 1)
      strm >> equalPriors;
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool DesignBayesNormalQuadraticBodyC::Save (std::ostream &out) const {
    if(!DesignClassifierSupervisedBodyC::Save(out))
      return false;
    char ver = 2;
    out << ((int) ver) << ' ' << priors << ' ' << equalPriors;
    return true;
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool DesignBayesNormalQuadraticBodyC::Save (BinOStreamC &out) const {
    if(!DesignClassifierSupervisedBodyC::Save(out))
      return false;
    char ver = 2;
    out << ((int) ver) << priors << equalPriors;
    return true;
  }
  
  //: Create new function.C
  
  //: Create function from the given data.
  ClassifierC DesignBayesNormalQuadraticBodyC::Apply(const SampleC<VectorC> &in, const SampleC<UIntT> &out) {
    ONDEBUG(cerr << "DesignBayesNormalQuadraticBodyC::Apply(), Computing coefficients.. \n");
    DataSetVectorLabelC dset(in, out);
    SArray1dC<UIntT>nums = dset.ClassNums();
    if(equalPriors) {
      priors = SArray1dC<RealT>(nums.Size());
      for(SArray1dIterC<RealT>it(priors);it;it++) it.Data1() = 1.0/(RealT)nums.Size();      
    } else if ((!equalPriors) && (priors.Size()<1)) {
      priors = SArray1dC<RealT>(nums.Size());
      RealT tot = (RealT)dset.Size();
      for(SArray1dIter2C<RealT, UIntT>it(priors, nums);it;it++) it.Data1() = (RealT)it.Data2()/tot;
    } else {
      if(priors.Size()!=nums.Size()) 
	RavlIssueError("User set priors not same size as the number of classes");
    }
    ClassifierBayesNormalQuadraticC func(dset.ClassStats(), priors);
    return func;
  }
  
 
  ////////////////////////////////////////////////////////////////////////
  static RavlN::XMLFactoryRegisterHandleConvertC<DesignBayesNormalQuadraticC, DesignClassifierSupervisedC> g_registerXMLFactoryDesignBayesNormalQuadratic("RavlN::DesignBayesNormalQuadraticC");

  RAVL_INITVIRTUALCONSTRUCTOR_FULL(DesignBayesNormalQuadraticBodyC,DesignBayesNormalQuadraticC,DesignClassifierSupervisedC);
  
  void linkDesignBayesNormalQuadratic()
   {}
  
}
