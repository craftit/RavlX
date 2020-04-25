// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_CLASSIFYFUNC1THRESHOLD_HEADER
#define RAVL_CLASSIFYFUNC1THRESHOLD_HEADER 1
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! author="Charles Galambos"
//! docentry="Ravl.API.Pattern Recognition.Classifier"
//! date="9/3/2003"
//! file="Ravl/PatternRec/Classify/ClassifierFunc1Threshold.hh"

#include "Ravl/PatternRec/Classifier.hh"
#include "Ravl/PatternRec/Function1.hh"
#include "Ravl/BinStream.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Binary Classifier, based on a function and threshold.
  
  class ClassifierFunc1ThresholdBodyC 
    : public ClassifierBodyC
  {
  public:
    ClassifierFunc1ThresholdBodyC(const Function1C &nfunc,RealT nthreshold);
    //: Constructor.
    
    ClassifierFunc1ThresholdBodyC(std::istream &strm);
    //: Load from stream.
    
    ClassifierFunc1ThresholdBodyC(BinIStreamC &strm);
    //: Load from binary stream.
    
    virtual bool Save (std::ostream &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual bool Save (BinOStreamC &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual UIntT Classify(const VectorC &data) const;
    //: Classifier vector 'data' return the most likely label.
    
    virtual VectorC Confidence(const VectorC &data) const;
    //: Estimate the confidence for each label.
    
    Function1C &Function()
    { return func; }
    //: Access discrminant function.
    
    RealT Threshold() const
    { return threshold; }
    //: Access threshold.
    
  protected:
    Function1C func;
    RealT threshold;
  };
  
  //! userlevel=Normal
  //: Binary Classifier, based on a function and threshold.

  
  class ClassifierFunc1ThresholdC
    : public ClassifierC
  {
  public:
    ClassifierFunc1ThresholdC()
    {}
    //: Default constructor.
    
    ClassifierFunc1ThresholdC(const Function1C &nfunc,RealT nthreshold)
      : ClassifierC(*new ClassifierFunc1ThresholdBodyC(nfunc,nthreshold))
    {}
    //: Constructor.

    ClassifierFunc1ThresholdC(std::istream &strm);
    //: Load from stream.
    
    ClassifierFunc1ThresholdC(BinIStreamC &strm);
    //: Load from binary stream.
    
  protected:
    ClassifierFunc1ThresholdC(ClassifierFunc1ThresholdBodyC &bod)
      : ClassifierC(bod)
    {}
    //: Body constructor.

    ClassifierFunc1ThresholdC(ClassifierFunc1ThresholdBodyC *bod)
      : ClassifierC(bod)
    {}
    //: Body constructor.
    
    ClassifierFunc1ThresholdBodyC &Body()
    { return static_cast<ClassifierFunc1ThresholdBodyC &>(ClassifierC::Body()); }
    //: Access body.
    
    const ClassifierFunc1ThresholdBodyC &Body() const
    { return static_cast<const ClassifierFunc1ThresholdBodyC &>(ClassifierC::Body()); }
    //: Access body.
    
  public:
    
    Function1C &Function()
    { return Body().Function(); }
    //: Access discrminant function.
    
    RealT Threshold() const
    { return Body().Threshold(); }
    //: Access threshold.
    
  };

  
  inline std::istream &operator>>(std::istream &strm,ClassifierFunc1ThresholdC &obj) {
    obj = ClassifierFunc1ThresholdC(strm);
    return strm;
  }
  //: Load from a stream.
  // Uses virtual constructor.
  
  inline std::ostream &operator<<(std::ostream &out,const ClassifierFunc1ThresholdC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
  inline BinIStreamC &operator>>(BinIStreamC &strm,ClassifierFunc1ThresholdC &obj) {
    obj = ClassifierFunc1ThresholdC(strm);
    return strm;
  }
  //: Load from a binary stream.
  // Uses virtual constructor.
  
  inline BinOStreamC &operator<<(BinOStreamC &out,const ClassifierFunc1ThresholdC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.

}



#endif
