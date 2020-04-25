// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/Classify/ClassifierLogisticRegression.cc"

#include "Ravl/PatternRec/ClassifierLogisticRegression.hh"
#include "Ravl/VirtualConstructor.hh"
#include "Ravl/BinStream.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  
  //: Create classifier from function.
  
  ClassifierLogisticRegressionBodyC::ClassifierLogisticRegressionBodyC(const FunctionC &nfunc,
                                                                       const MatrixC &weights,
                                                                       bool prependUnit)
    : ClassifierBodyC(weights.Rows() == 1 ? (unsigned) 2 : (unsigned) weights.Rows().V()),
      m_norm(nfunc),
      m_weights(weights),
      m_prependUnit(prependUnit)
  {}
  
  //: Create classifier from some weights.

  ClassifierLogisticRegressionBodyC::ClassifierLogisticRegressionBodyC(const MatrixC &weights,bool prependUnit)
   : ClassifierBodyC(weights.Rows() == 1 ? (unsigned) 2 : (unsigned) weights.Rows().V()),
     m_weights(weights),
     m_prependUnit(prependUnit)
  {}

  //: Load from stream.
  
  ClassifierLogisticRegressionBodyC::ClassifierLogisticRegressionBodyC(std::istream &strm)
    : ClassifierBodyC(strm),
      m_prependUnit(false)
  { 
    IntT version;
    strm >> version;
    if(version != 1)
      throw ExceptionOutOfRangeC("ClassifierLogisticRegressionBodyC::ClassifierLogisticRegressionBodyC(std::istream &), Unrecognised version number in stream. ");

    bool haveNorm = false;
    strm >> haveNorm;
    if(haveNorm)
      strm >> m_norm;
    strm >> m_weights >> m_prependUnit;
  }
  
  //: Load from binary stream.
  
  ClassifierLogisticRegressionBodyC::ClassifierLogisticRegressionBodyC(BinIStreamC &strm)
    : ClassifierBodyC(strm),
      m_prependUnit(false)
  {
    IntT version;
    strm >> version;
    if(version != 1)
      throw ExceptionOutOfRangeC("ClassifierLogisticRegressionBodyC::ClassifierLogisticRegressionBodyC(BinIStreamC &), Unrecognised version number in stream. ");
    bool haveNorm = false;
    strm >> haveNorm;
    if(haveNorm)
      strm >> m_norm;
    strm >> m_weights >> m_prependUnit;
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool ClassifierLogisticRegressionBodyC::Save(std::ostream &out) const {
    if(!ClassifierBodyC::Save(out))
      return false;
    IntT version = 1;
    out << ' ' << version << ' ';
    out << m_norm.IsValid() << ' ';
    if(m_norm.IsValid())
      out << m_norm << ' ';
    out << m_weights;
    return true;
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool ClassifierLogisticRegressionBodyC::Save(BinOStreamC &out) const {
    if(!ClassifierBodyC::Save(out))
      return false;
    IntT version = 1;
    out << version;
    out << m_norm.IsValid();
    if(m_norm.IsValid())
      out << m_norm;
    out << m_weights << m_prependUnit;
    return true;    
  }
  
  static VectorC MakeBias()
  {
    VectorC bias(1);
    bias[0] = 1;
    return bias;
  }

  static const VectorC &BiasVector()
  {
    static VectorC bias = MakeBias();
    return bias;
  }

  //: Classifier vector 'data' return the most likely label.
  
  UIntT ClassifierLogisticRegressionBodyC::Classify(const VectorC &data) const {
    VectorC vec;
    if(m_norm.IsValid())
      vec = m_norm(data);
    else
      vec = data;
    VectorC result;
    if(m_prependUnit) {
      result = m_weights * VectorC(BiasVector().Join(vec));
    } else {
      result = m_weights * VectorC(vec);
    }
    // Just a binary classifier?
    if(result.Size() == 1) {
      return result[0] > 0.0 ? 1 : 0;
    }
    return result.MaxIndex().V();
  }
  
  //: Estimate the confidence for each label.
  // The meaning of the confidence assigned to each label depends
  // on the classifier used. The higher the confidence the more likely
  // it is the label is correct.
  
  VectorC ClassifierLogisticRegressionBodyC::Apply(const VectorC &data) const {
    VectorC vec;
    if(m_norm.IsValid())
      vec = m_norm(data);
    else
      vec = data;
    VectorC result;
    if(m_prependUnit) {
      result = Sigmoid(m_weights * VectorC(BiasVector().Join(vec)));
    } else {
      result = Sigmoid(m_weights * vec);
    }
    if(result.Size() == 1){ // Just a binary classifier?
      VectorC ret(2);
      ret[0] = 1.0 - result[0];
      ret[1] = result[0];
    }
    return result.MakeUnit();
  }
  
  RAVL_INITVIRTUALCONSTRUCTOR_FULL(ClassifierLogisticRegressionBodyC,ClassifierLogisticRegressionC,ClassifierC);
  
  
}
