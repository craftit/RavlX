// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DESIGNONEAGAINSTALL_HEADER
#define RAVL_DESIGNONEAGAINSTALL_HEADER 1
//! rcsid="$Id: DesignOneAgainstAll.hh 7585 2010-02-23 09:36:10Z kier $"
//! lib=RavlPatternRec
//! author="Kieron Messer"
//! docentry="Ravl.API.Pattern Recognition.Classifier.DesignClassifier"
//! file="Ravl/PatternRec/Classify/DesignOneAgainstAll.hh"

#include "Ravl/PatternRec/DesignClassifierSupervised.hh"
#include "Ravl/XMLFactory.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Design a one against all classifier
  
  class DesignOneAgainstAllBodyC
    : public DesignClassifierSupervisedBodyC
  {
  public:
    DesignOneAgainstAllBodyC(const DesignClassifierSupervisedC & classifier);
    //: Constructor.

    DesignOneAgainstAllBodyC(const XMLFactoryContextC & factory);
    //: Constructor.

    DesignOneAgainstAllBodyC(std::istream &strm);
    //: Load from stream.
    
    DesignOneAgainstAllBodyC(BinIStreamC &strm);
    //: Load from binary stream.
    
    virtual bool Save (std::ostream &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual bool Save (BinOStreamC &out) const;
    //: Writes object to stream, can be loaded using constructor

    virtual ClassifierC Apply(const SampleC<VectorC> &in,const SampleC<UIntT> &out);
    //: Create a clasifier.
    
    virtual ClassifierC Apply(const SampleC<VectorC> &in,const SampleC<UIntT> &out, const SampleC<RealT> &weight);
    //: Create a clasifier with weights for the samples.
    
  protected:
    DesignClassifierSupervisedC m_design;
  };
  
  //! userlevel=Normal
  //: Design a one against all classifier

  class DesignOneAgainstAllC
    : public DesignClassifierSupervisedC
  {
  public:
    DesignOneAgainstAllC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    DesignOneAgainstAllC(const DesignClassifierSupervisedC & classifier)
      : DesignClassifierSupervisedC(*new DesignOneAgainstAllBodyC(classifier))
    {}
    //: Create a new designer.

    DesignOneAgainstAllC(const XMLFactoryContextC & factory)
      : DesignClassifierSupervisedC(*new DesignOneAgainstAllBodyC(factory))
    {}
    //: Create a new designer.
    
    DesignOneAgainstAllC(std::istream &strm);
    //: Load from stream.
    
    DesignOneAgainstAllC(BinIStreamC &strm);
    //: Load from binary stream.

  protected:
    DesignOneAgainstAllC(DesignOneAgainstAllBodyC &bod)
      : DesignClassifierSupervisedC(bod)
    {}
    //: Body constructor.

    DesignOneAgainstAllC(DesignOneAgainstAllBodyC *bod)
      : DesignClassifierSupervisedC(bod)
    {}
    //: Body ptr constructor.
    
    DesignOneAgainstAllBodyC &Body()
    { return static_cast<DesignOneAgainstAllBodyC &>(DesignClassifierSupervisedC::Body()); }
    //: Access body.
    
    const DesignOneAgainstAllBodyC &Body() const
    { return static_cast<const DesignOneAgainstAllBodyC &>(DesignClassifierSupervisedC::Body()); }
    //: Access body.
    
  public:
    
  };
  
  inline std::istream &operator>>(std::istream &strm,DesignOneAgainstAllC &obj) {
    obj = DesignOneAgainstAllC(strm);
    return strm;
  }
  //: Load from a stream.
  // Uses virtual constructor.
  
  inline std::ostream &operator<<(std::ostream &out,const DesignOneAgainstAllC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
  inline BinIStreamC &operator>>(BinIStreamC &strm,DesignOneAgainstAllC &obj) {
    obj = DesignOneAgainstAllC(strm);
    return strm;
  }
  //: Load from a binary stream.
  // Uses virtual constructor.
  
  inline BinOStreamC &operator<<(BinOStreamC &out,const DesignOneAgainstAllC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.

}

#endif
