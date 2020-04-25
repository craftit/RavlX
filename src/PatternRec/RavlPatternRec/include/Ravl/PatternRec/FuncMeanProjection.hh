// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_FUNCMEANPROJECTION_HEADER
#define RAVL_FUNCMEANPROJECTION_HEADER 1
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! author="Charles Galambos"
//! docentry="Ravl.API.Pattern Recognition.Numerical Modeling"
//! file="Ravl/PatternRec/Modeling/Basic/FuncMeanProjection.hh"

#include "Ravl/PatternRec/Function.hh"
#include "Ravl/PatternRec/FuncLinear.hh"
#include "Ravl/Matrix.hh"
#include "Ravl/Vector.hh"

namespace RavlN {

  //! userlevel=Develop
  //: Subtract a mean and project a vector
  // This is very similar FuncLinear, except the mean is subtracted first then 
  // the vector is projected.
  
  class FuncMeanProjectionBodyC
    : public FunctionBodyC
  {
  public:
    FuncMeanProjectionBodyC();
    //: Default constructor.
    
    FuncMeanProjectionBodyC(const VectorC &nMean,const MatrixC &nProj);
    //: Default constructor.
    
    FuncMeanProjectionBodyC(std::istream &strm);
    //: Load from stream.
    
    FuncMeanProjectionBodyC(BinIStreamC &strm);
    //: Load from binary stream.
    
    virtual bool Save (std::ostream &out) const;
    //: Writes object to stream.
    
    virtual bool Save (BinOStreamC &out) const;
    //: Writes object to binary stream.
    
    virtual VectorC Apply(const VectorC &data) const;
    //: Apply function to 'data'
    
    const VectorC &Mean() const
    { return mean; }
    //: Access mean vector.
    
    const MatrixC &Projection() const
    { return proj; }
    //: Access projection matrix.
    
    FuncLinearC Reverse() const;
    //: Return the function that applies the reverse, i.e. X * proj.T() + mean

  protected:
    VectorC mean;
    MatrixC proj;
  };
  
  //! userlevel=Normal
  //: Subtract a mean and project a vector
  // This is very similar to <a href="RavlN.FuncLinearC.html">FuncLinearC</a>,
  // except the mean is subtracted first, then the vector is projected.
  
  class FuncMeanProjectionC
    : public FunctionC 
  {
  public:
    FuncMeanProjectionC()
    {}
    //: Default constructor.
    
    FuncMeanProjectionC(const VectorC &mean,const MatrixC &proj)
      : FunctionC(*new FuncMeanProjectionBodyC(mean,proj))
    {}
    //: Construct from a mean and a projection matrix.
    
    FuncMeanProjectionC(std::istream &is);
    //: Stream constructor.
    
    FuncMeanProjectionC(BinIStreamC &is);
    //: Stream constructor.
    
    FuncMeanProjectionC(const FunctionC &func)
      : FunctionC(dynamic_cast<const FuncMeanProjectionBodyC *>(BodyPtr(func)))
    {}
    //: Attempt to create handle from base class.
    // If object is not a FuncMeanProjectionC, an invalid handle will be created.
    
  protected:
    FuncMeanProjectionC(FuncMeanProjectionBodyC &bod)
      : FunctionC(bod)
    {}
    //: Body constructor.
    
    FuncMeanProjectionC(const FuncMeanProjectionBodyC *bod)
      : FunctionC(bod)
    {}
    //: Body constructor.
    
    FuncMeanProjectionBodyC &Body()
    { return static_cast<FuncMeanProjectionBodyC &>(FunctionC::Body()); }
    //: Access body.

    const FuncMeanProjectionBodyC &Body() const 
    { return static_cast<const FuncMeanProjectionBodyC &>(FunctionC::Body()); }
    //: Access body.
    
  public:
    const VectorC &Mean() const
    { return Body().Mean(); }
    //: Access mean vector.
    
    const MatrixC &Projection() const
    { return Body().Projection(); }
    //: Access projection matrix.
    
    FuncLinearC Reverse() const {
      return Body().Reverse();
    }
    //: Return the function that applies the reverse, i.e. X * proj.T() + mean
    // For example if this a PCA projection, this will apply the
    // inverse to get your original vector back (minus some variance e.t.c.)

  };
  
  
  inline std::istream &operator>>(std::istream &strm,FuncMeanProjectionC &obj) {
    obj = FuncMeanProjectionC(strm);
    return strm;
  }
  //: Load from a stream.
  // Uses virtual constructor.
  
  inline std::ostream &operator<<(std::ostream &out,const FuncMeanProjectionC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
  inline BinIStreamC &operator>>(BinIStreamC &strm,FuncMeanProjectionC &obj) {
    obj = FuncMeanProjectionC(strm);
    return strm;
  }
  //: Load from a binary stream.
  // Uses virtual constructor.
  
  inline BinOStreamC &operator<<(BinOStreamC &out,const FuncMeanProjectionC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.

}



#endif
