// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/Classify/ClassifierLinearCombination.cc"

#include "Ravl/PatternRec/ClassifierLinearCombination.hh"
#include "Ravl/VirtualConstructor.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/SArray1dIter3.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  
  //: Constructor.
  //!param: weakClassifiers - a set of classifiers to be combined
  //!param: weights         - relative weights for each classifier
  // Classification is label 1 if sum(weight[i]*classify[i](x)) >= 0.5sum(weight[i])
  
  ClassifierLinearCombinationBodyC::ClassifierLinearCombinationBodyC(SArray1dC<ClassifierC> weakClassifiers, SArray1dC<RealT> weights, RealT threshold)
    : m_weakClassifiers(weakClassifiers),
      m_weights(weights),
      m_sumWeights(0.0),
      m_featureSet(weakClassifiers.Size()),
      m_threshold(threshold)
  {
    RavlAssert(weakClassifiers.Size() == weights.Size());
    for(SArray1dIter2C<RealT,IndexC> it(weights,m_featureSet); it; it++) {
      m_sumWeights += it.Data1();
      it.Data2() = it.Index();
    }
    m_sumWeights *= threshold;
  }
  
  //: Load from stream.
  
  ClassifierLinearCombinationBodyC::ClassifierLinearCombinationBodyC(std::istream &strm) 
    : ClassifierBodyC(strm)
  {
    IntT version;
    strm >> version;
    if(version < 0 || version > 1)
      throw ExceptionOutOfRangeC("ClassifierLinearCombinationBodyC(std::istream &), Unrecognised version number in stream. ");
    strm >> m_weakClassifiers >> m_weights >> m_sumWeights >> m_featureSet;
    if(version > 0) {
      strm >> m_threshold;
    } else {
      // Attempt to recover threshold from weights
      RealT sum = 0.0;
      for(SArray1dIterC<RealT> it(m_weights);it;it++)
        sum += *it;
      m_threshold = sum / m_sumWeights;
    }
  }
  
  //: Load from binary stream.
  
  ClassifierLinearCombinationBodyC::ClassifierLinearCombinationBodyC(BinIStreamC &strm) 
    : ClassifierBodyC(strm)
  {
    IntT version;
    strm >> version;
    if(version < 0 || version > 1)
      throw ExceptionOutOfRangeC("ClassifierWeakLinearBodyC(BinIStreamC &), Unrecognised version number in stream. ");
    strm >> m_weakClassifiers >> m_weights >> m_sumWeights >> m_featureSet;
    if(version > 0) {
      strm >> m_threshold;
    } else {
      // Attempt to recover threshold from weights
      RealT sum = 0.0;
      for(SArray1dIterC<RealT> it(m_weights);it;it++)
        sum += *it;
      m_threshold = sum / m_sumWeights;
    }
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool ClassifierLinearCombinationBodyC::Save (std::ostream &out) const {
    if(!ClassifierBodyC::Save(out))
      return false;
    IntT version = 1;
    out << ' ' << version << ' ' << m_weakClassifiers << ' ' << m_weights << ' ' << m_sumWeights << ' ' << m_featureSet << ' ' << m_threshold;
    return true;
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool ClassifierLinearCombinationBodyC::Save (BinOStreamC &out) const {
    if(!ClassifierBodyC::Save(out))
      return false;
    IntT version = 1;
    out << version << m_weakClassifiers << m_weights << m_sumWeights << m_featureSet << m_threshold;
    return true;
  }

  UIntT ClassifierLinearCombinationBodyC::Classify(const VectorC &data) const {
    return Classify(data,m_featureSet);
  }

  UIntT ClassifierLinearCombinationBodyC::Classify(const VectorC &data,const SArray1dC<IndexC> &featureSet) const {
    RealT weightedSumLabels = 0.0;
    SArray1dC<IndexC> oneFeature(1);
    for(SArray1dIter3C<ClassifierC,RealT,IndexC> it(m_weakClassifiers,m_weights,featureSet); it; it++) {
      oneFeature[0] = it.Data3();
      weightedSumLabels += it.Data2() * it.Data1().Classify(data,oneFeature);
    }
    return weightedSumLabels >= m_sumWeights; // threshold is included in sum weights
  }
  
  VectorC ClassifierLinearCombinationBodyC::Apply(const VectorC & data) const {
    RealT weightedSumLabels = 0.0;
    SArray1dC<IndexC> oneFeature(1);
    for(SArray1dIter3C<ClassifierC,RealT,IndexC> it(m_weakClassifiers,m_weights,m_featureSet); it; it++) {
      oneFeature[0] = it.Data3();
      weightedSumLabels += it.Data2() * it.Data1().Classify(data,oneFeature);
    }
    RealT dif = Abs(weightedSumLabels - m_sumWeights);
    VectorC vec(1);
    if(weightedSumLabels > m_sumWeights) {
      vec[0] = -dif;
    } else {
      vec[0] = dif;
    }
    return vec;
  }

  //: Set the array of weights, one for each classifier.
  // Note: The array must be of the same size as the number of weak classifiers.

  void ClassifierLinearCombinationBodyC::SetWeakClassifierWeights(const SArray1dC<RealT> &weights) {
    RavlAssert(m_weakClassifiers.Size() == m_weights.Size());
    for(SArray1dIterC<RealT> it(weights); it; it++) {
      m_sumWeights += it.Data();
    }
    m_sumWeights *= m_threshold;
    m_weights = weights; 
  }
      
  RAVL_INITVIRTUALCONSTRUCTOR_FULL(ClassifierLinearCombinationBodyC,ClassifierLinearCombinationC,ClassifierC);

  void InitRavlClassifierLinearCombinationIO() {
  }

}
