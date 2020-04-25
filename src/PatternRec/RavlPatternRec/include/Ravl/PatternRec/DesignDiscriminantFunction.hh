// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DESIGNDISCRIMINANTFUNCTION_HEADER
#define RAVL_DESIGNDISCRIMINANTFUNCTION_HEADER 1
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! docentry="Ravl.API.Pattern Recognition.Classifier.DesignClassifier"
//! file="Ravl/PatternRec/Classify/DesignDiscriminantFunction.hh"
//! author="Charles Galambos"

#include "Ravl/PatternRec/DesignClassifierSupervised.hh"
#include "Ravl/PatternRec/DesignFunctionSupervised.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Design a discriminant function classifier.
  
  class DesignDiscriminantFunctionBodyC
    : public DesignClassifierSupervisedBodyC
  {
  public:
    DesignDiscriminantFunctionBodyC(const DesignFunctionSupervisedC &func);
    //: Default constructor.
    
    DesignDiscriminantFunctionBodyC(const XMLFactoryContextC &factory);
    //: Construct from XML factory

    DesignDiscriminantFunctionBodyC(std::istream &strm);
    //: Load from stream.
    
    DesignDiscriminantFunctionBodyC(BinIStreamC &strm);
    //: Load from binary stream.
    
    virtual bool Save (std::ostream &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual bool Save (BinOStreamC &out) const;
    //: Writes object to stream, can be loaded using constructor

    virtual ClassifierC Apply(const SampleC<VectorC> &in,const SampleC<UIntT> &out);
    //: Create a classifier.
    
    virtual ClassifierC Apply(const SampleC<VectorC> &in,const SampleC<UIntT> &out,const SampleC<RealT> &weight);
    //: Create a classifier with weights for the samples.
    
    const DesignFunctionSupervisedC &FunctionDesigner() const
    { return designFunc; }
    //: Access the current function designer being used.
    
  protected:
    DesignFunctionSupervisedC designFunc;
  };

  //! userlevel=Normal
  //: Design a discriminant function classifier.
  
  class DesignDiscriminantFunctionC
    : public DesignClassifierSupervisedC
  {
  public:
    DesignDiscriminantFunctionC()
    {}
    //: Default constructor.
    // Creates an invalid handle.

    DesignDiscriminantFunctionC(const DesignFunctionSupervisedC &designFunc)
      : DesignClassifierSupervisedC(*new DesignDiscriminantFunctionBodyC(designFunc))
    {}
    //: Create a new designer.
    
    DesignDiscriminantFunctionC(const XMLFactoryContextC &factory) :
        DesignClassifierSupervisedC(*new DesignDiscriminantFunctionBodyC(factory))
    {
    }
    //: Construct from XML factory

    DesignDiscriminantFunctionC(std::istream &strm);
    //: Load from stream.
    
    DesignDiscriminantFunctionC(BinIStreamC &strm);
    //: Load from binary stream.
    
  protected:
    DesignDiscriminantFunctionC(DesignDiscriminantFunctionBodyC &bod)
      : DesignClassifierSupervisedC(bod)
    {}
    //: Body constructor.
    
    DesignDiscriminantFunctionC(DesignDiscriminantFunctionBodyC *bod)
      : DesignClassifierSupervisedC(bod)
    {}
    //: Body ptr constructor.
    
    DesignDiscriminantFunctionBodyC &Body()
    { return static_cast<DesignDiscriminantFunctionBodyC &>(DesignClassifierSupervisedC::Body()); }
    //: Access body.

    const DesignDiscriminantFunctionBodyC &Body() const
    { return static_cast<const DesignDiscriminantFunctionBodyC &>(DesignClassifierSupervisedC::Body()); }
    //: Access body.
    
  public:
    
    const DesignFunctionSupervisedC &FunctionDesigner() const
    { return Body().FunctionDesigner(); }
    //: Access the current function designer being used.
  };
  
  
  inline std::istream &operator>>(std::istream &strm,DesignDiscriminantFunctionC &obj) {
    obj = DesignDiscriminantFunctionC(strm);
    return strm;
  }
  //: Load from a stream.
  // Uses virtual constructor.
  
  inline std::ostream &operator<<(std::ostream &out,const DesignDiscriminantFunctionC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
  inline BinIStreamC &operator>>(BinIStreamC &strm,DesignDiscriminantFunctionC &obj) {
    obj = DesignDiscriminantFunctionC(strm);
    return strm;
  }
  //: Load from a binary stream.
  // Uses virtual constructor.
  
  inline BinOStreamC &operator<<(BinOStreamC &out,const DesignDiscriminantFunctionC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.


}


#endif
