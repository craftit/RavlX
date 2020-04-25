// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_FUNCSUBSET_HEADER
#define RAVL_FUNCSUBSET_HEADER 1
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! docentry="Ravl.API.Pattern Recognition.Dimension Reduction"
//! author="Charles Galambos"
//! file="Ravl/PatternRec/Modeling/DimensionReduction/FuncSubset.hh"

#include "Ravl/PatternRec/Function.hh"

namespace RavlN {

  //! userlevel=Develop
  //: Reduce dimension by taking a subset of features.
  
  class FuncSubsetBodyC 
    : public FunctionBodyC
  {
  public:
    FuncSubsetBodyC()
    {}
    //: Default constructor.
    
    FuncSubsetBodyC(const SArray1dC<IndexC> &ninds,UIntT inSize);
    //: Constructor from an array of indexes.
    
    FuncSubsetBodyC(std::istream &strm);
       //: Load from stream.

    FuncSubsetBodyC(BinIStreamC &strm);
    //: Load from binary stream.

    virtual bool Save(std::ostream &out) const;
    //: Writes object to stream, can be loaded using constructor

    virtual bool Save(BinOStreamC &out) const;
    //: Writes object to stream, can be loaded using constructor

    virtual VectorC Apply(const VectorC &data) const;
    //: Reduce the dimension of 'data'.
    
    SArray1dC<IndexC> &Indexes()
    { return inds; }
    //: Get set of indexes that will be used.
    
  protected:
    SArray1dC<IndexC> inds; // Indexes of features we're interested in.
  };

  //! userlevel=Normal
  //: Reduce dimension by taking a subset of features.

  class FuncSubsetC 
    : public FunctionC
  {
  public:
    FuncSubsetC()
    {}
    //: Default constuctor.
    // Creates an invalid handle.
    
    FuncSubsetC(const SArray1dC<IndexC> &ninds,UIntT inSize)
      : FunctionC(*new FuncSubsetBodyC(ninds,inSize))
    {}
    //: Constuctor from an array of indexes.
    

    FuncSubsetC(std::istream &strm);
    //: Load from stream.

    FuncSubsetC(BinIStreamC &strm);
    //: Load from binary stream.

  protected:
    FuncSubsetC(FuncSubsetBodyC &bod)
      : FunctionC(bod)
    {}
    //: Body constructor.
    
    FuncSubsetBodyC &Body() 
    { return static_cast<FuncSubsetBodyC &>(FunctionC::Body()); }
    //: Access body.
    
    const FuncSubsetBodyC &Body() const
    { return static_cast<const FuncSubsetBodyC &>(FunctionC::Body()); }
    //: Access body.
    
  public:
    
    SArray1dC<IndexC> &Indexes()
    { return Body().Indexes(); }
    //: Get set of indexes that will be used.
  };

  inline std::istream &operator>>(std::istream &strm, FuncSubsetC &obj)
  {
    obj = FuncSubsetC(strm);
    return strm;
  }
  //: Load from a stream.
  // Uses virtual constructor.

  inline std::ostream &operator<<(std::ostream &out, const FuncSubsetC &obj)
  {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.

  inline BinIStreamC &operator>>(BinIStreamC &strm, FuncSubsetC &obj)
  {
    obj = FuncSubsetC(strm);
    return strm;
  }
  //: Load from a binary stream.
  // Uses virtual constructor.

  inline BinOStreamC &operator<<(BinOStreamC &out, const FuncSubsetC &obj)
  {
    obj.Save(out);
    return out;
  }
//: Save to a stream.
// Uses virtual constructor.


}


#endif
