// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_FUNCPDFNORMAL_HEADER
#define RAVL_FUNCPDFNORMAL_HEADER 1
//! docentry="Ravl.API.Pattern Recognition.Numerical Modeling"
//! author="Charles Galambos"
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/Modeling/Basic/FuncPDFNormal.hh"

#include "Ravl/PatternRec/FuncPDF.hh"
#include "Ravl/Normal.hh"

namespace RavlN {
  
  //! userlevel=Obsolete
  //: Set of normal distributions.

  class FuncPDFNormalBodyC
    : public FuncPDFBodyC
  {
  public:
    FuncPDFNormalBodyC()
    {}
    //: Default constructor.
    
    FuncPDFNormalBodyC(const SArray1dC<NormalC> &ndists);
    //: Create from an array of distributions.
    
    FuncPDFNormalBodyC(std::istream &strm);
    //: Load from stream.
    
    FuncPDFNormalBodyC(BinIStreamC &strm);
    //: Load from binary stream.
    
    virtual bool Save (std::ostream &out) const;
    //: Writes object to stream.
    
    virtual bool Save (BinOStreamC &out) const;
    //: Writes object to binary stream.
    
    virtual VectorC Apply(const VectorC &data) const;
    //: Apply function to 'data'
    
  protected:
    SArray1dC<NormalC> dists; // Array of distributions.
  };
  
  //! userlevel=Obsolete
  //: Set of normal distributions.
  
  class FuncPDFNormalC
    : public FuncPDFC
  {
  public:
    FuncPDFNormalC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    FuncPDFNormalC(std::istream &strm);
    //: Load from stream.
    
    FuncPDFNormalC(BinIStreamC &strm);
    //: Load from binary stream.
    
    FuncPDFNormalC(const SArray1dC<NormalC> &ndists)
      : FuncPDFC(*new FuncPDFNormalBodyC(ndists))
    {}
    //: Create from an array of distributions.
    
  protected:
    FuncPDFNormalC(FuncPDFNormalBodyC &bod)
      : FuncPDFC(bod)
    {}
    //: Body constructor.
    
    FuncPDFNormalC(FuncPDFNormalBodyC *bod)
      : FuncPDFC(bod)
    {}
    //: Body ptr constructor.
    
  };
  
  inline std::istream &operator>>(std::istream &strm,FuncPDFNormalC &obj) {
    obj = FuncPDFNormalC(strm);
    return strm;
  }
  //: Load from a stream.
  // Uses virtual constructor.
  
  inline std::ostream &operator<<(std::ostream &out,const FuncPDFNormalC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
  inline BinIStreamC &operator>>(BinIStreamC &strm,FuncPDFNormalC &obj) {
    obj = FuncPDFNormalC(strm);
    return strm;
  }
  //: Load from a binary stream.
  // Uses virtual constructor.
  
  inline BinOStreamC &operator<<(BinOStreamC &out,const FuncPDFNormalC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.

  
}



#endif
