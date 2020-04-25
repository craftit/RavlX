// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/Classify/ClassifierPreprocess.cc"

#include "Ravl/PatternRec/ClassifierPreprocess.hh"
#include "Ravl/VirtualConstructor.hh"
#include "Ravl/BinStream.hh"

namespace RavlN {

  //: Create classifier from function.

  ClassifierPreprocessBodyC::ClassifierPreprocessBodyC(const FunctionC &nfunc, const ClassifierC &nclassifier)
      : ClassifierBodyC(nclassifier.OutputSize()), func(nfunc), classifier(nclassifier) {
  }

  //: Load from stream.

  ClassifierPreprocessBodyC::ClassifierPreprocessBodyC(std::istream &strm)
      : ClassifierBodyC(strm) {
    IntT version;
    strm >> version;
    if (version != 0)
      throw ExceptionOutOfRangeC(
          "ClassifierPreprocessBodyC::ClassifierPreprocessBodyC(std::istream &), Unrecognised version number in stream. ");

    strm >> func >> classifier;
  }

  //: Load from binary stream.

  ClassifierPreprocessBodyC::ClassifierPreprocessBodyC(BinIStreamC &strm)
      : ClassifierBodyC(strm) {
    IntT version;
    strm >> version;
    if (version != 0)
      throw ExceptionOutOfRangeC(
          "ClassifierPreprocessBodyC::ClassifierPreprocessBodyC(BinIStreamC &), Unrecognised version number in stream. ");
    strm >> func >> classifier;
  }

  //: Writes object to stream, can be loaded using constructor

  bool ClassifierPreprocessBodyC::Save(std::ostream &out) const {
    if (!ClassifierBodyC::Save(out))
      return false;
    IntT version = 0;
    out << version << ' ' << func << ' ' << classifier;
    return true;
  }

  //: Writes object to stream, can be loaded using constructor

  bool ClassifierPreprocessBodyC::Save(BinOStreamC &out) const {
    if (!ClassifierBodyC::Save(out))
      return false;
    IntT version = 0;
    out << version << func << classifier;
    return true;
  }

  //: Classifier vector 'data' return the most likely label.

  UIntT ClassifierPreprocessBodyC::Classify(const VectorC &data) const {
    return classifier.Classify(func.Apply(data));
  }

  //: Estimate the confidence for each label.
  // The meaning of the confidence assigned to each label depends
  // on the classifier used. The higher the confidence the more likely
  // it is the label is correct.

  VectorC ClassifierPreprocessBodyC::Confidence(const VectorC &data) const {
    return classifier.Confidence(func.Apply(data));
  }

  //: Estimate the confidence for each label.
  // The meaning of the confidence assigned to each label depends
  // on the classifier used. The higher the confidence the more likely
  // it is the label is correct.

  VectorC ClassifierPreprocessBodyC::Apply(const VectorC &data) const {
    return classifier.Apply(func.Apply(data));
  }

  RAVL_INITVIRTUALCONSTRUCTOR_FULL(ClassifierPreprocessBodyC, ClassifierPreprocessC, ClassifierC);

  void InitRavlClassifierPreprocess() {
  }

}
