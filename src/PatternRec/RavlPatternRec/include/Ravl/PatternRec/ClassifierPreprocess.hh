// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_CLASSIFIERPREPROCESS_HEADER
#define RAVL_CLASSIFIERPREPROCESS_HEADER 1
///////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! author="Charles Galambos"
//! docentry="Ravl.API.Pattern Recognition.Classifier"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/Classify/ClassifierPreprocess.hh"

#include "Ravl/PatternRec/Classifier.hh"
#include "Ravl/PatternRec/Function.hh"

namespace RavlN {

  //! userlevel=Develop
  //: Preprocess input with another function before classifying.

  class ClassifierPreprocessBodyC: public ClassifierBodyC {
    public:
      ClassifierPreprocessBodyC(const FunctionC &nfunc, const ClassifierC &nclassifier);
      //: Create classifier from a discriminant function.

      ClassifierPreprocessBodyC(std::istream &strm);
      //: Load from stream.

      ClassifierPreprocessBodyC(BinIStreamC &strm);
      //: Load from binary stream.

      virtual bool Save(std::ostream &out) const;
      //: Writes object to stream, can be loaded using constructor

      virtual bool Save(BinOStreamC &out) const;
      //: Writes object to stream, can be loaded using constructor

      virtual UIntT Classify(const VectorC &data) const;
      //: Classifier vector 'data' return the most likely label.

      virtual VectorC Confidence(const VectorC &data) const;
      //: Estimate the confidence for each label.
      // The meaning of the confidence assigned to each label depends
      // on the function used. The higher the confidence the more likely
      // it is the label is correct. The sum of the vector has been normalised
      // to 1.

      virtual VectorC Apply(const VectorC &data) const;
      //: Estimate the confidence for each label.
      // The meaning of the confidence assigned to each label depends
      // on the function used. The higher the confidence the more likely
      // it is the label is correct. The sum of the vector has been normalised
      // to 1.

      const FunctionC &Preprocess() const {
        return func;
      }
      //: Access discriminant function.

    protected:
      FunctionC func;
      ClassifierC classifier;
  };

  //! userlevel=Normal
  //: Preprocess input with another function before classifying.

  class ClassifierPreprocessC: public ClassifierC {
    public:
      ClassifierPreprocessC(const FunctionC &nfunc, const ClassifierC &classifier)
          : ClassifierC(*new ClassifierPreprocessBodyC(nfunc, classifier)) {
      }
      //: Create classifier from a discriminant function.

      ClassifierPreprocessC(std::istream &strm);
      //: Load from stream.

      ClassifierPreprocessC(BinIStreamC &strm);
      //: Load from binary stream.

    protected:
      ClassifierPreprocessC(ClassifierPreprocessBodyC &bod)
          : ClassifierC(bod) {
      }
      //: Body constructor.

      ClassifierPreprocessC(ClassifierPreprocessBodyC *bod)
          : ClassifierC(bod) {
      }
      //: Body constructor.

      ClassifierPreprocessBodyC &Body() {
        return static_cast<ClassifierPreprocessBodyC &>(ClassifierC::Body());
      }
      //: Access body.

      const ClassifierPreprocessBodyC &Body() const {
        return static_cast<const ClassifierPreprocessBodyC &>(ClassifierC::Body());
      }
      //: Access body.

    public:
      const FunctionC &Preprocess() const {
        return Body().Preprocess();
      }
      //: Access discriminant function.

  };

  inline std::istream &operator>>(std::istream &strm, ClassifierPreprocessC &obj) {
    obj = ClassifierPreprocessC(strm);
    return strm;
  }
  //: Load from a stream.
  // Uses virtual constructor.

  inline std::ostream &operator<<(std::ostream &out, const ClassifierPreprocessC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.

  inline BinIStreamC &operator>>(BinIStreamC &strm, ClassifierPreprocessC &obj) {
    obj = ClassifierPreprocessC(strm);
    return strm;
  }
  //: Load from a binary stream.
  // Uses virtual constructor.

  inline BinOStreamC &operator<<(BinOStreamC &out, const ClassifierPreprocessC &obj) {
    obj.Save(out);
    return out;
  }
//: Save to a stream.
// Uses virtual constructor.

}

#endif
