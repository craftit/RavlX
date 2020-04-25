// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2012, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlPatternRec
#ifndef RAVL_PATTERNREC_COSTLOGISTICREGRESSION_HEADER
#define RAVL_PATTERNREC_COSTLOGISTICREGRESSION_HEADER 1

#include "Ravl/PatternRec/DesignClassifierLogisticRegression.hh"
#include "Ravl/VirtualConstructor.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/XMLFactoryRegister.hh"
#include "Ravl/PatternRec/Cost.hh"
#include "Ravl/PatternRec/DataSet2.hh"
#include "Ravl/PatternRec/DataSet3.hh"
#include "Ravl/PatternRec/DataSet2Iter.hh"
#include "Ravl/PatternRec/DataSet3Iter.hh"
#include "Ravl/PatternRec/SampleVector.hh"
#include "Ravl/PatternRec/OptimiseConjugateGradient.hh"
#include "Ravl/PatternRec/OptimiseDescent.hh"
#include "Ravl/PatternRec/SampleLabel.hh"
#include "Ravl/PatternRec/FunctionCascade.hh"
#include "Ravl/SysLog.hh"
#include "Ravl/MeanCovariance.hh"
#include "Ravl/PatternRec/ClassifierLogisticRegression.hh"
#include "Ravl/SysLog.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#include "Ravl/StrStream.hh"
#include "Ravl/OS/Date.hh"
#else
#define ONDEBUG(x)
#endif

namespace RavlN {

  //! Cost function for logistic regression.

  class CostLogisticRegressionC
   : public CostBodyC
  {
  public:
    //! Constructor
    CostLogisticRegressionC(UIntT label,
                            UIntT vecSize,
                            const SampleC<VectorC> &in,
                            const SampleC<UIntT> &out,
                            RealT regularisation,
                            bool verbose)
     : m_label(label),
       m_in(in),
       m_out(out),
       m_regularisation(regularisation),
       m_verbose(verbose)
    {
      ParametersC parameters(vecSize,true);
      SetParameters(parameters);
      RavlAssert(parameters.StartX().Size() == vecSize);
    }

    //! Compute the cost of a solution.
    virtual RealT Cost (const VectorC &X) const;

    //! Compute the jacobian.
    virtual VectorC Jacobian1(const VectorC &X) const;

  protected:
    UIntT m_label;
    SampleC<VectorC> m_in;
    SampleC<UIntT> m_out;
    SampleC<RealT> m_weight;
    RealT m_regularisation;
    bool m_verbose;
  };


}

#endif
