// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_FUNCTIONCASCADE_HEADER
#define RAVL_FUNCTIONCASCADE_HEADER 1
//! lib=RavlPatternRec
//! author="Kieron"
//! docentry="Ravl.API.Pattern Recognition.Numerical Modeling"
//! file="Ravl/PatternRec/Modeling/Basic/FunctionCascade.hh"

#include "Ravl/PatternRec/Function.hh"
#include "Ravl/SArray1d.hh"

namespace RavlN {

  //! userlevel=Develop
  //: Function which cascades a set of functions
  
  class FunctionCascadeBodyC
    : public FunctionBodyC
  {
  public:
    FunctionCascadeBodyC();
    //: Default constructor.

    FunctionCascadeBodyC(const XMLFactoryContextC &factory);
    //: Construct from XML factory

    FunctionCascadeBodyC(const SArray1dC<FunctionC> & functions);
    //: Construct from array of functions

    FunctionCascadeBodyC(const FunctionC & function1, const FunctionC & function2);
    //: Construct from two functions
    
    FunctionCascadeBodyC(const FunctionC & function1, const FunctionC & function2, const FunctionC & function3);
    //: Construct from three functions
    
    FunctionCascadeBodyC(std::istream &strm);
    //: Load from stream.
    
    FunctionCascadeBodyC(BinIStreamC &strm);
    //: Load from binary stream.
    
    virtual bool Save (std::ostream &out) const;
    //: Writes object to stream.
    
    virtual bool Save (BinOStreamC &out) const;
    //: Writes object to binary stream.
    
    virtual void ApplyInPlace(const VectorC &data,VectorC &out) const;
    //: Apply function in place to 'data', overwrite values in out if its of the correct size.

    virtual VectorC Apply(const VectorC &data) const;
    //: Apply function to 'data'

    virtual void ApplyInPlace(const TVectorC<float> &data,TVectorC<float> &out) const;
    //: Apply function in place to 'data', overwrite values in out if its of the correct size.

    virtual TVectorC<float> Apply(const TVectorC<float> &data) const;
    //: Apply function with float vectors to 'data'

    const SArray1dC<FunctionC> & Functions() const
    { return m_functions; }
    //: Access to the functions
    
    
  protected:
    // The concatenated functions
    SArray1dC<FunctionC> m_functions;
  };
  
  //! userlevel=Normal
  //: Function which applies functions one after another
  
  class FunctionCascadeC
    : public FunctionC 
  {
  public:
    FunctionCascadeC()
    {}
    //: Default constructor.

    FunctionCascadeC(const XMLFactoryContextC &factory)
      : FunctionC(*new FunctionCascadeBodyC(factory))
    {}
    //: Construct from XML factory
    
    FunctionCascadeC(const SArray1dC<FunctionC> & functions)
      : FunctionC(*new FunctionCascadeBodyC(functions))
    {}
    //: Construct from an array of functions

    FunctionCascadeC(const FunctionC & function1, const FunctionC & function2)
      : FunctionC(*new FunctionCascadeBodyC(function1, function2))
    {}
    //: Construct from a set of functions

    FunctionCascadeC(const FunctionC & function1, const FunctionC & function2, const FunctionC & function3)
      : FunctionC(*new FunctionCascadeBodyC(function1, function2, function3))
    {}
    //: Construct from a set of functions
    
    FunctionCascadeC(std::istream &is);
    //: Stream constructor.
    
    FunctionCascadeC(BinIStreamC &is);
    //: Stream constructor.
    
    FunctionCascadeC(const FunctionC &func)
      : FunctionC(dynamic_cast<const FunctionCascadeBodyC *>(BodyPtr(func)))
    {}
    //: Attempt to create handle from base class.
    // If object is not a FunctionCascadeC, an invalid handle will be created.
    
  protected:
    FunctionCascadeC(FunctionCascadeBodyC &bod)
      : FunctionC(bod)
    {}
    //: Body constructor.
    
    FunctionCascadeC(const FunctionCascadeBodyC *bod)
      : FunctionC(bod)
    {}
    //: Body constructor.
    
    FunctionCascadeBodyC &Body()
    { return static_cast<FunctionCascadeBodyC &>(FunctionC::Body()); }
    //: Access body.

    const FunctionCascadeBodyC &Body() const 
    { return static_cast<const FunctionCascadeBodyC &>(FunctionC::Body()); }
    //: Access body.
    
  public:
    const SArray1dC<FunctionC> & Functions() const
    { return Body().Functions(); }
    //: Access functions.
    
    
  };
  
  
  inline std::istream &operator>>(std::istream &strm,FunctionCascadeC &obj) {
    obj = FunctionCascadeC(strm);
    return strm;
  }
  //: Load from a stream.
  // Uses virtual constructor.
  
  inline std::ostream &operator<<(std::ostream &out,const FunctionCascadeC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
  inline BinIStreamC &operator>>(BinIStreamC &strm,FunctionCascadeC &obj) {
    obj = FunctionCascadeC(strm);
    return strm;
  }
  //: Load from a binary stream.
  // Uses virtual constructor.
  
  inline BinOStreamC &operator<<(BinOStreamC &out,const FunctionCascadeC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.

}



#endif
