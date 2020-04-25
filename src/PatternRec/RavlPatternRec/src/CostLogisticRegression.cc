// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2012, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/Classify/DesignClassifierLogisticRegression.cc"

#include "Ravl/PatternRec/CostLogisticRegression.hh"
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

  //! Compute the cost of a solution.

  RealT CostLogisticRegressionC::Cost (const VectorC &theta) const
  {
    RealT cost = 0;
    ONDEBUG(RavlDebug("Theta:%s",RavlN::StringOf(theta).c_str()));
    for(DataSet2IterC<SampleC<VectorC>,SampleC<UIntT> > it(m_in,m_out);it;it++)
    {
      ONDEBUG(RavlDebug("Data:%s Theta:%s ",RavlN::StringOf(it.Data1()).c_str(),RavlN::StringOf(theta).c_str()));
      RealT dotProdS = Sigmoid(it.Data1().Dot(theta));
      ONDEBUG(RavlDebug("Dot %f ",dotProdS));
      if(m_label == it.Data2()) {
        cost += -Log(dotProdS);
      } else {
        cost += -Log(1-dotProdS);
      }
    }
    // This assumes the first value of X is just the bias.
    if(m_regularisation > 0) {
      RealT sum = 0;
      for(unsigned i = 1;i < theta.Size();i++)
        sum += Sqr(theta[i]);
      cost += m_regularisation * sum;
    }

    //J = sum(-y .* log( sigmoid(X * theta) ) - (1 -y) .* log(1 - sigmoid(X * theta)))/ m;
    RealT fcost = cost / ((RealT) m_in.Size());
    //ONDEBUG(RavlDebug("Theta=%s Cost=%f",RavlN::StringOf(theta).c_str(),fcost));
    if(m_verbose)
      RavlDebug("Cost=%f",fcost);
    //RavlN::Sleep(1.0);
    return fcost;
  }

  //! Compute the jacobian.

  VectorC CostLogisticRegressionC::Jacobian1(const VectorC &theta) const
  {
    ONDEBUG(RavlDebug("Theta %s",RavlN::StringOf(theta).c_str()));
    VectorC grad(theta.Size());
    grad.Fill(0);
    for(DataSet2IterC<SampleC<VectorC>,SampleC<UIntT> > it(m_in,m_out);it;it++)
    {
      RealT num = Sigmoid(it.Data1().Dot(theta));
      if(it.Data2() == m_label) {
        num -= 1.0;
      }
      for(unsigned i = 0;i < grad.Size();i++)
        grad[i] += it.Data1()[i] * num;
    }
    //J = sum(-y .* log( sigmoid(X * theta) ) - (1 -y) .* log(1 - sigmoid(X * theta)))/ m + theta(2:size(theta))' * theta(2:size(theta)) * (lambda/(2 * m));

    // Include regularisation term.
    if(m_regularisation > 0) {
      for(unsigned i = 1;i < grad.Size();i++) {
        grad[i] += m_regularisation * theta[i];
      }
    }
    //grad = (1 / m) * X' * (sigmoid(X * theta) - y) ;
    grad = grad / ((RealT) m_in.Size());

    ONDEBUG(RavlDebug("Grad @ %s = %s (%f) ",RavlN::StringOf(theta).c_str(),RavlN::StringOf(grad).c_str(),m_regularisation));
    return grad;
  }

}
