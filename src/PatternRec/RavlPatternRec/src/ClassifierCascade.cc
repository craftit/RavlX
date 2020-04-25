// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/Classify/ClassifierCascade.cc"

#include "Ravl/PatternRec/ClassifierCascade.hh"
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
  
  //: Create classifier from function.
  
  ClassifierCascadeBodyC::ClassifierCascadeBodyC(const SArray1dC<ClassifierC> & classifiers,
      const SArray1dC<RealT> & thresholds,
      const SArray1dC<FuncSubsetC> & features) :
      ClassifierBodyC((UIntT) 2), m_classifiers(classifiers), m_thresholds(thresholds), m_features(features)
  {
  }
  
  //: Load from stream.
  
  ClassifierCascadeBodyC::ClassifierCascadeBodyC(std::istream &strm) :
      ClassifierBodyC(strm)
  {
    IntT version;
    strm >> version;
    if (version != 1)
      throw ExceptionOutOfRangeC("ClassifierCascadeBodyC::ClassifierCascadeBodyC(std::istream &), Unrecognised version number in stream. ");
    strm >> m_classifiers;
    strm >> m_thresholds;
    strm >> m_features;
  }
  
  //: Load from binary stream.
  
  ClassifierCascadeBodyC::ClassifierCascadeBodyC(BinIStreamC &strm) :
      ClassifierBodyC(strm)
  {
    IntT version;
    strm >> version;
    if (version != 1)
      throw ExceptionOutOfRangeC("ClassifierCascadeBodyC::ClassifierCascadeBodyC(BinIStreamC &), Unrecognised version number in stream. ");
    strm >> m_classifiers;
    strm >> m_thresholds;
    strm >> m_features;
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool ClassifierCascadeBodyC::Save(std::ostream &out) const
  {
    if (!ClassifierBodyC::Save(out))
      return false;
    IntT version = 1;
    out << ' ' << version << ' ';
    out << m_classifiers << ' ';
    out << m_thresholds << ' ';
    out << m_features << ' ';
    return true;
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool ClassifierCascadeBodyC::Save(BinOStreamC &out) const
  {
    if (!ClassifierBodyC::Save(out))
      return false;
    IntT version = 1;
    out << version;
    out << m_classifiers;
    out << m_thresholds;
    out << m_features;
    return true;
  }
  
  //: Classifier vector 'data' return the most likely label.
  
  UIntT ClassifierCascadeBodyC::Classify(const VectorC &data) const
  {
    VectorC v = Apply(data);
    return v.MaxIndex().V();
  }
  
  //: Estimate the confidence for each label.
  // The meaning of the confidence assigned to each label depends
  // on the classifier used. The higher the confidence the more likely
  // it is the label is correct.
  
  VectorC ClassifierCascadeBodyC::Apply(const VectorC &data) const
  {

    VectorC v(2);
    v.Fill(0);
    for(SArray1dIter3C<ClassifierC, RealT, FuncSubsetC>it(m_classifiers, m_thresholds, m_features);it;it++) {

      // Reduce vec
      VectorC d = it.Data3().Apply(data);

      // Apply classification
      RealT score = it.Data1().LabelProbability(d, 0);

      //RavlInfo("Stage %s Score %0.4f Threshold %0.4f", StringOf(it.Index()).data(), score, it.Data2());

      if(score < it.Data2()) {
        v[1] = 1.0;
        return v;
      }

    }
    v[0] = 1.0;
    return v;
  }
  
  RAVL_INITVIRTUALCONSTRUCTOR_FULL(ClassifierCascadeBodyC, ClassifierCascadeC, ClassifierC);
  
}
