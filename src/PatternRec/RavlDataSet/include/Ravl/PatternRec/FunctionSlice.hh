// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_FUNCTIONSLICE_HEADER
#define RAVL_FUNCTIONSLICE_HEADER 1
//! lib=RavlPatternRec
//! author="Kieron"
//! docentry="Ravl.API.Pattern Recognition.Numerical Modeling"
//! file="Ravl/PatternRec/Modeling/Basic/FunctionConcatenate.hh"

#include "Ravl/PatternRec/Function.hh"
#include "Ravl/SArray1d.hh"

namespace RavlN {

  //! userlevel=Develop
  //: Takes a sub range of the input vector.
  
  class FunctionSliceBodyC
    : public FunctionBodyC
  {
  public:
    FunctionSliceBodyC();
    //: Default constructor.
    
    FunctionSliceBodyC(const XMLFactoryContextC &factory);
    //: Construct from XML factory

    FunctionSliceBodyC(UIntT inputSize, const IndexRangeC & indexRange);
    //: Constructor from an input size and a range.
    
    FunctionSliceBodyC(std::istream &strm);
    //: Load from stream.
    
    FunctionSliceBodyC(BinIStreamC &strm);
    //: Load from binary stream.
    
    virtual bool Save (std::ostream &out) const;
    //: Writes object to stream.
    
    virtual bool Save (BinOStreamC &out) const;
    //: Writes object to binary stream.
    
    virtual VectorC Apply(const VectorC &data) const;
    //: Apply function to 'data'
    
    const IndexRangeC & IndexRange() const
    { return m_indexRange; }
    //: Access to the index range to slice
   
    
  protected:
    IndexRangeC m_indexRange; //!< The range of the vector to slice
    
  };
  
  //! userlevel=Normal
  //: Takes a sub range of the input vector.
  
  class FunctionSliceC
    : public FunctionC 
  {
  public:
    FunctionSliceC()
    {}
    //: Default constructor.
    
    FunctionSliceC(UIntT inputSize, const IndexRangeC & indexRange)
      : FunctionC(*new FunctionSliceBodyC(inputSize, indexRange))
    {}
    //: Construct from a set of functions
    
    FunctionSliceC(std::istream &is);
    //: Stream constructor.
    
    FunctionSliceC(BinIStreamC &is);
    //: Stream constructor.
    
    FunctionSliceC(const FunctionC &func)
      : FunctionC(dynamic_cast<const FunctionSliceBodyC *>(BodyPtr(func)))
    {}
    //: Attempt to create handle from base class.
    // If object is not a FunctionSliceC, an invalid handle will be created.
    
  protected:
    FunctionSliceC(FunctionSliceBodyC &bod)
      : FunctionC(bod)
    {}
    //: Body constructor.
    
    FunctionSliceC(const FunctionSliceBodyC *bod)
      : FunctionC(bod)
    {}
    //: Body constructor.
    
    FunctionSliceBodyC &Body()
    { return static_cast<FunctionSliceBodyC &>(FunctionC::Body()); }
    //: Access body.

    const FunctionSliceBodyC &Body() const 
    { return static_cast<const FunctionSliceBodyC &>(FunctionC::Body()); }
    //: Access body.
    
  public:
    const IndexRangeC & IndexRange() const
    { return Body().IndexRange(); }
    //: Access functions.
    
    
  };
  
  
  inline std::istream &operator>>(std::istream &strm,FunctionSliceC &obj) {
    obj = FunctionSliceC(strm);
    return strm;
  }
  //: Load from a stream.
  // Uses virtual constructor.
  
  inline std::ostream &operator<<(std::ostream &out,const FunctionSliceC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
  inline BinIStreamC &operator>>(BinIStreamC &strm,FunctionSliceC &obj) {
    obj = FunctionSliceC(strm);
    return strm;
  }
  //: Load from a binary stream.
  // Uses virtual constructor.
  
  inline BinOStreamC &operator<<(BinOStreamC &out,const FunctionSliceC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.

}



#endif
