// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_FUNCPDF_HEADER
#define RAVL_FUNCPDF_HEADER 1
//! docentry="Ravl.API.Pattern Recognition.Numerical Modeling"
//! author="Charles Galambos"
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/Modeling/Basic/FuncPDF.hh"

#include "Ravl/PatternRec/Function.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Abstract Probability Distribution Function.
  // The integral of these functions must be 1.
  
  class FuncPDFBodyC 
    : public FunctionBodyC
  {
  public:
    FuncPDFBodyC()
    {}
    //: Default constructor.

    FuncPDFBodyC(std::istream &strm);
    //: Load from stream.
    
    FuncPDFBodyC(BinIStreamC &strm);
    //: Load from binary stream.
    
    virtual bool Save (std::ostream &out) const;
    //: Writes object to stream.
    
    virtual bool Save (BinOStreamC &out) const;
    //: Writes object to binary stream.
    
    FuncPDFBodyC(UIntT inSize,UIntT outSize);
    //: Constructor.
    
  };
  
  //! userlevel=Normal
  //: Abstract Probability Distribution Function.
  // The integral of these functions must be 1.
  
  class FuncPDFC
    : public FunctionC
  {
  public:
    FuncPDFC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    FuncPDFC(std::istream &strm);
    //: Load from stream.
    
    FuncPDFC(BinIStreamC &strm);
    //: Load from binary stream.
    
  protected:
    FuncPDFC(FuncPDFBodyC &bod)
      : FunctionC(bod)
    {}
    //: Body constructor.
    
    FuncPDFC(FuncPDFBodyC *bod)
      : FunctionC(bod)
    {}
    //: Body ptr constructor.
    
    FuncPDFBodyC& Body()
    { return static_cast<FuncPDFBodyC &>(FunctionC::Body()); }
    //: Access body.

    const FuncPDFBodyC& Body() const
    { return static_cast<const FuncPDFBodyC &>(FunctionC::Body()); }
    //: Access body.
    
  public:
    
  };
  
  inline std::istream &operator>>(std::istream &strm,FuncPDFC &obj) {
    obj = FuncPDFC(strm);
    return strm;
  }
  //: Load from a stream.
  // Uses virtual constructor.
  
  inline std::ostream &operator<<(std::ostream &out,const FuncPDFC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
  inline BinIStreamC &operator>>(BinIStreamC &strm,FuncPDFC &obj) {
    obj = FuncPDFC(strm);
    return strm;
  }
  //: Load from a binary stream.
  // Uses virtual constructor.
  
  inline BinOStreamC &operator<<(BinOStreamC &out,const FuncPDFC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.

}


#endif
