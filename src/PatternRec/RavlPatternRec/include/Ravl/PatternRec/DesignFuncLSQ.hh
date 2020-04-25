// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DESIGNFUNCLSQ_HEADER
#define RAVL_DESIGNFUNCLSQ_HEADER 1
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! author="Charles Galambos"
//! docentry="Ravl.API.Pattern Recognition.Numerical Modeling"
//! file="Ravl/PatternRec/Modeling/Basic/DesignFuncLSQ.hh"

#include "Ravl/PatternRec/DesignFunctionSupervised.hh"
#include "Ravl/PatternRec/FuncLinearCoeff.hh"

namespace RavlN {
  class MatrixRUTC;
  
  //! userlevel=Develop
  //: Design function with Least Squares Fitting.
  
  class DesignFuncLSQBodyC
    : public DesignFunctionSupervisedBodyC
  {
  public:
    DesignFuncLSQBodyC();
    //: Default constructor.
    
    DesignFuncLSQBodyC(const XMLFactoryContextC &factory);
    //: Construct from XML factory

    DesignFuncLSQBodyC(std::istream &strm);
    //: Load from stream.
    
    DesignFuncLSQBodyC(BinIStreamC &strm);
    //: Load from binary stream.
    
    virtual bool Save (std::ostream &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual bool Save (BinOStreamC &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    DesignFuncLSQBodyC(UIntT norder,bool northogonal);
    //: Create least squares designer.
    
    virtual FuncLinearCoeffC CreateFunc(UIntT nin,UIntT nout);
    //: Create new function.
    
    virtual FunctionC Apply(const SampleC<VectorC> &in,const SampleC<VectorC> &out);
    //: Create function from the given data.
    
    virtual FunctionC Apply(const SampleC<VectorC> &in,const SampleC<VectorC> &out,const SampleC<RealT> &weight);
    //: Create function from the given data, and sample weights.
    
    virtual FunctionC Apply(SampleStream2C<VectorC,VectorC > &in);
    //: Create function from the given data.
    // Note: Construction from a sample stream may not be implemented by all designers.
    
    SArray1dC<IntT> FindCorrelatedParameters(const MatrixRUTC &mat,RealT thresh = 1e-6);
    //: Find correlated parameters.
    // The array contains -1 if the parameter is independent or the number of the
    // Parameter it correlates with.
    
  protected:
    UIntT order;
    bool orthogonal;
  };

  //! userlevel=Normal
  //: Design function with Least Squares Fitting.
  
  class DesignFuncLSQC
    : public DesignFunctionSupervisedC
  {
  public:
    DesignFuncLSQC()
    {}
    //: Default constructor.
    // Creates an invalid handle.

    DesignFuncLSQC(std::istream &strm);
    //: Load from stream.
    
    DesignFuncLSQC(BinIStreamC &strm);
    //: Load from binary stream.
    
    DesignFuncLSQC(UIntT order,bool orthogonal)
      : DesignFunctionSupervisedC(*new DesignFuncLSQBodyC(order,orthogonal))
    {}
    //: Create designer.
    
    DesignFuncLSQC(const XMLFactoryContextC &factory) :
        DesignFunctionSupervisedC(*new DesignFuncLSQBodyC(factory))
    {
    }
    //: Construct from XML factory

  protected:
    DesignFuncLSQC(DesignFuncLSQBodyC &bod)
      : DesignFunctionSupervisedC(bod)
    {}
    //: Body constructor.
    
    DesignFuncLSQC(DesignFuncLSQBodyC *bod)
      : DesignFunctionSupervisedC(bod)
    {}
    //: Body ptr constructor.
    
  };
  
  inline std::istream &operator>>(std::istream &strm,DesignFuncLSQC &obj) {
    obj = DesignFuncLSQC(strm);
    return strm;
  }
  //: Load from a stream.
  // Uses virtual constructor.
  
  inline std::ostream &operator<<(std::ostream &out,const DesignFuncLSQC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
  inline BinIStreamC &operator>>(BinIStreamC &strm,DesignFuncLSQC &obj) {
    obj = DesignFuncLSQC(strm);
    return strm;
  }
  //: Load from a binary stream.
  // Uses virtual constructor.
  
  inline BinOStreamC &operator<<(BinOStreamC &out,const DesignFuncLSQC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.

}


#endif
