// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/Classify/DesignWeakLinear.cc"

#include "Ravl/PatternRec/DesignWeakLinear.hh"
#include "Ravl/PatternRec/ClassifierWeakLinear.hh"
#include "Ravl/VirtualConstructor.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/Vector2d.hh"
#include "Ravl/PatternRec/SampleIter.hh"

namespace RavlN {
  
  //: Load from stream.
  
  DesignWeakLinearBodyC::DesignWeakLinearBodyC(std::istream &strm)
    : DesignClassifierSupervisedBodyC(strm)
  {
    int version;
    strm >> version;
    if(version != 0)
      throw ExceptionOutOfRangeC("DesignWeakLinearBodyC::DesignWeakLinearBodyC(std::istream &), Unrecognised version number in stream. ");
  }
  
  //: Load from binary stream.
  
  DesignWeakLinearBodyC::DesignWeakLinearBodyC(BinIStreamC &strm)
    : DesignClassifierSupervisedBodyC(strm)
  {
    int version;
    strm >> version;
    if(version != 0)
      throw ExceptionOutOfRangeC("DesignWeakLinearBodyC::DesignWeakLinearBodyC(BinIStreamC &), Unrecognised version number in stream. ");
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool DesignWeakLinearBodyC::Save (std::ostream &out) const {
    if(!DesignClassifierSupervisedBodyC::Save(out))
      return false;
    int version = 0;
    out << ' ' << version;
    return true;
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool DesignWeakLinearBodyC::Save (BinOStreamC &out) const {
    if(!DesignClassifierSupervisedBodyC::Save(out))
      return false;
    int version = 0;
    out << version;
    return true;
  }
  
  //: Create a classifier.
  
  ClassifierC DesignWeakLinearBodyC::Apply(const SampleC<VectorC> &in,const SampleC<UIntT> &out) {
    RavlAssertMsg(in[0].Size() == 1,"DesignWeakLinearBodyC::Apply(), Feature vectors can only have 1 element.");
    SArray1dC<IndexC> featureSet(1);
    featureSet[0] = 0;
    // use the other apply method to do the work
    return Apply(in,out,featureSet);
  }
  
  //: Create a classifier with weights for the samples.
  
  ClassifierC DesignWeakLinearBodyC::Apply(const SampleC<VectorC> &in,
					   const SampleC<UIntT> &out,
					   const SampleC<RealT> &weight) {
    RavlAssertMsg(0,"DesignWeakLinearBodyC::Apply(in,out,weight), Not implemented. Send a feature request! ");
    return ClassifierC();
  }
 
  ClassifierC DesignWeakLinearBodyC::Apply(const SampleC<VectorC> &in,
					   const SampleC<UIntT> &out,
					   const SArray1dC<IndexC> &featureSet) {
    RavlAssertMsg(0,"DesignWeakLinearBodyC::Apply(in,out,featureSet), Not implemented. Send a feature request! ");
    return ClassifierC();
  }

  ClassifierC DesignWeakLinearBodyC::Apply(const SampleC<VectorC> &in,
					   const SampleC<UIntT> &out,
					   const SArray1dC<IndexC> &featureSet,
					   const SampleC<RealT> &weight) {
    RavlAssertMsg(in.Size() == out.Size(),"DesignWeakLinearBodyC::Apply(), Sample of vector and labels should be the same size.");
    RavlAssertMsg(featureSet.Size() == 1,"DesignWeakLinearBodyC::Apply(), Reduced feature set may only contain a single feature.");
    IndexC feature = featureSet[0];
    Vector2dC sumX;
    Vector2dC sumX2;
    sumX.Fill(0.0);
    sumX2.Fill(0.0);
    m_weight.Fill(0.0);
    SampleIterC<VectorC> it1(in);
    SampleIterC<UIntT> it2(out);
    SampleIterC<RealT> it3(weight);
    for (; it1; it1++,it2++,it3++) {
      RealT val = (*it1)[feature];
      RealT w = *it3;
      sumX[*it2] += val * w;
      sumX2[*it2] += val * val * w;
      m_weight[*it2] += w;
    }
    RealT threshold1,threshold2;
    RealT parity1,parity2;
    if (m_weight[0] != 0.0 && m_weight[1] != 0.0) {
      m_mean[1] = sumX[1] / m_weight[1];
      m_var[1] = sumX2[1] / m_weight[1] - m_mean[1]*m_mean[1];
      if (IsNan(m_var[1]) || m_var[1] < 0.0) m_var[1] = 0.0;
      m_mean[0] = sumX[0] / m_weight[0];
      m_var[0] = sumX2[0] / m_weight[0] - m_mean[0]*m_mean[0];
      if (IsNan(m_var[0]) || m_var[0] < 0.0) m_var[0] = 0.0;
      ThresholdAndParity(threshold1,parity1,threshold2,parity2);
    }
    else {
      std::cout << "Empty label count in one class!\n";
      parity1 = parity2 = 1;
      threshold1 = threshold2 = 1e6;
    }
    return ClassifierWeakLinearC (threshold1, parity1, threshold2, parity2);
  }

  void DesignWeakLinearBodyC::ThresholdAndParity(RealT &threshold1, RealT &parity1, RealT &threshold2, RealT &parity2) {
    Vector2dC sigma,root;
    // This finds the 2 possible solutions for where the two Gaussian 
    // distributions are equal
    sigma[0] = Sqrt(m_var[0]);
    sigma[1] = Sqrt(m_var[1]);
    RealT a = 0.5 / m_var[0] - 0.5 / m_var[1];
    RealT b = -(m_mean[0] / m_var[0] - m_mean[1] / m_var[1]);
    RealT c = m_mean[0]*m_mean[0] / (2.0*m_var[0]) - m_mean[1]*m_mean[1] / (2.0*m_var[1]) - Log(m_weight[0]/sigma[0]) + Log(m_weight[1]/sigma[1]);
    RealT offset = b*b-4.0*a*c;
    if (IsNan(offset) || offset < 0.0) {
      // set arbitrary threshold and parity
      threshold1 = threshold2 = 1e6;
      parity1 = parity2 = 1.0;
      return;
    }
    offset = Sqrt(offset);
    root[0] = (-b + offset) / (2.0*a);
    root[1] = (-b - offset) / (2.0*a);
    // We now determine which combinations of the roots will give the best
    // discrimination by using the erf function to determine the parity for
    // the thresholds
    Vector2dC erf0(Erf((root[0]-m_mean[0])/sigma[0]),Erf((root[0]-m_mean[1])/sigma[1]));
    Vector2dC erf1(Erf((root[1]-m_mean[0])/sigma[0]),Erf((root[1]-m_mean[1])/sigma[1]));
    VectorC errors(4);
    errors[0] = erf0[0] -erf0[1];
    errors[1] = -erf0[0] + erf0[1];
    errors[2] = erf1[0] -erf1[1];
    errors[3] = -erf1[0] + erf1[1];
    IndexC index = errors.MaxIndex();
    parity1 = index%2? 1.0: -1.0;
    threshold1 = root[index.V()/2];
    parity2 = -parity1;
    threshold2 = root[1-(index.V()/2)];
  }

  //////////////////////////////////////////////////////////
  
  RAVL_INITVIRTUALCONSTRUCTOR_FULL(DesignWeakLinearBodyC,DesignWeakLinearC,DesignClassifierSupervisedC);

}
