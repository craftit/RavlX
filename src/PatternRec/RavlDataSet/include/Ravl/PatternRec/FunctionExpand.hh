// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_FunctionExpand_HEADER
#define RAVL_FunctionExpand_HEADER 1
//! lib=RavlPatternRec
//! author="Kieron"
//! docentry="Ravl.API.Pattern Recognition.Numerical Modeling"
//! file="Ravl/PatternRec/Modeling/Basic/FunctionExpand.hh"

#include "Ravl/PatternRec/Function.hh"
#include "Ravl/SArray1d.hh"

namespace RavlN {

  //! userlevel=Develop
  //: Take one input and output the concatenation of several functions
  
  class FunctionExpandBodyC
    : public FunctionBodyC
  {
  public:
    FunctionExpandBodyC();
    //: Default constructor.
    
    FunctionExpandBodyC(const XMLFactoryContextC &factory);
    //: Construct from XML factory

    FunctionExpandBodyC(const SArray1dC<FunctionC> & functions);
    //: Use an array of functions.
    
    FunctionExpandBodyC(const FunctionC & function1, const FunctionC & function2);
    //: Construct from a set of functions.

    FunctionExpandBodyC(const FunctionC & function1, const FunctionC & function2, const FunctionC & function3);
    //: Construct from a set of functions.
    
    FunctionExpandBodyC(std::istream &strm);
    //: Load from stream.
    
    FunctionExpandBodyC(BinIStreamC &strm);
    //: Load from binary stream.
    
    virtual bool Save (std::ostream &out) const;
    //: Writes object to stream.
    
    virtual bool Save (BinOStreamC &out) const;
    //: Writes object to binary stream.
    
    virtual VectorC Apply(const VectorC &data) const;
    //: Apply function to 'data'
    
    const SArray1dC<FunctionC> & Functions() const
    { return m_functions; }
    //: Access to the functions
    
  protected:
    //! Compute input and output sizes.
    void ComputeSizes();

    // The concatenated functions
    SArray1dC<FunctionC> m_functions;
  };
  
  //! userlevel=Normal
  //: Take one input and output the concatenation of several functions
  
  class FunctionExpandC
    : public FunctionC 
  {
  public:
    FunctionExpandC()
    {}
    //: Default constructor.
    
    FunctionExpandC(const XMLFactoryContextC &factory)
      : FunctionC(*new FunctionExpandBodyC(factory))
    {}
    //: Construct from XML factory

    FunctionExpandC(const SArray1dC<FunctionC> & functions)
      : FunctionC(*new FunctionExpandBodyC(functions))
    {}
    //: Construct from a set of functions

    FunctionExpandC(const FunctionC & function1, const FunctionC & function2)
      : FunctionC(*new FunctionExpandBodyC(function1, function2))
    {}
    //: Construct from a set of functions

    FunctionExpandC(const FunctionC & function1, const FunctionC & function2, const FunctionC & function3)
      : FunctionC(*new FunctionExpandBodyC(function1, function2, function3))
    {}
    //: Construct from a set of functions
    
    FunctionExpandC(std::istream &is);
    //: Stream constructor.
    
    FunctionExpandC(BinIStreamC &is);
    //: Stream constructor.
    
    FunctionExpandC(const FunctionC &func)
      : FunctionC(dynamic_cast<const FunctionExpandBodyC *>(BodyPtr(func)))
    {}
    //: Attempt to create handle from base class.
    // If object is not a FunctionExpandC, an invalid handle will be created.
    
  protected:
    FunctionExpandC(FunctionExpandBodyC &bod)
      : FunctionC(bod)
    {}
    //: Body constructor.
    
    FunctionExpandC(const FunctionExpandBodyC *bod)
      : FunctionC(bod)
    {}
    //: Body constructor.
    
    FunctionExpandBodyC &Body()
    { return static_cast<FunctionExpandBodyC &>(FunctionC::Body()); }
    //: Access body.

    const FunctionExpandBodyC &Body() const
    { return static_cast<const FunctionExpandBodyC &>(FunctionC::Body()); }
    //: Access body.
    
  public:
    const SArray1dC<FunctionC> & Functions() const
    { return Body().Functions(); }
    //: Access functions.
    
    
  };
  
  
  inline std::istream &operator>>(std::istream &strm,FunctionExpandC &obj) {
    obj = FunctionExpandC(strm);
    return strm;
  }
  //: Load from a stream.
  // Uses virtual constructor.
  
  inline std::ostream &operator<<(std::ostream &out,const FunctionExpandC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
  inline BinIStreamC &operator>>(BinIStreamC &strm,FunctionExpandC &obj) {
    obj = FunctionExpandC(strm);
    return strm;
  }
  //: Load from a binary stream.
  // Uses virtual constructor.
  
  inline BinOStreamC &operator<<(BinOStreamC &out,const FunctionExpandC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.

}



#endif
