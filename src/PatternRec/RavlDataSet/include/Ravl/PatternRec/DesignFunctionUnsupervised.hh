// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DESIGNFUNCTIONUNSUPERVISED_HEADER
#define RAVL_DESIGNFUNCTIONUNSUPERVISED_HEADER 1
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! author="Charles Galambos"
//! docentry="Ravl.API.Pattern Recognition.Functions"
//! file="Ravl/PatternRec/DataSet/DesignFunctionUnsupervised.hh"

#include "Ravl/PatternRec/Function.hh"
#include "Ravl/PatternRec/Sample.hh"
#include "Ravl/PatternRec/Designer.hh"
#include "Ravl/PatternRec/SampleStream.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Unsupervised function designer.
  
  class DesignFunctionUnsupervisedBodyC
    : public DesignerBodyC
  {
  public:
    DesignFunctionUnsupervisedBodyC();
    //: Default constructor.
    
    DesignFunctionUnsupervisedBodyC(std::istream &strm);
    //: Load from stream.
    
    DesignFunctionUnsupervisedBodyC(BinIStreamC &strm);
    //: Load from binary stream.
    
    virtual bool Save (std::ostream &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual bool Save (BinOStreamC &out) const;
    //: Writes object to stream, can be loaded using constructor

    virtual FunctionC Apply(const SampleC<VectorC> &in);
    //: Create function from the given data.
    
    virtual FunctionC Apply(const SampleC<VectorC> &in,const SampleC<RealT> &weight);
    //: Create function from the given data, and sample weights.
    
    virtual FunctionC Apply(SampleStreamC<VectorC> &in);
    //: Create function from the given data.
    // Note: Construction from a sample stream may not be implemented by all designers.
    
    virtual FunctionC Apply(SampleStream2C<VectorC,RealT> &in);
    //: Create function from the given data, and sample weights.
    // Note: Construction from a sample stream may not be implemented by all designers.

    virtual FunctionC Apply(const SampleC<TVectorC<float> > &in);
    //: Create function from the given data.

    virtual FunctionC Apply(const SampleC<TVectorC<float> > &in,const SampleC<float> &weight);
    //: Create function from the given data, and sample weights.

    virtual FunctionC Apply(SampleStreamC<TVectorC<float> > &in);
    //: Create function from the given data.
    // Note: Construction from a sample stream may not be implemented by all designers.
    
    virtual FunctionC Apply(SampleStream2C<TVectorC<float>,float> &in);
    //: Create function from the given data, and sample weights.
    // Note: Construction from a sample stream may not be implemented by all designers.

  };
  
  //! userlevel=Normal
  //: Unsupervised function designer.
  
  class DesignFunctionUnsupervisedC
    : public DesignerC
  {
  public:
    DesignFunctionUnsupervisedC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    DesignFunctionUnsupervisedC(std::istream &strm);
    //: Load from stream.
    
    DesignFunctionUnsupervisedC(BinIStreamC &strm);
    //: Load from binary stream.

  protected:
    DesignFunctionUnsupervisedC(DesignFunctionUnsupervisedBodyC &bod)
      : DesignerC(bod)
    {}
    //: Body constructor.
    
    DesignFunctionUnsupervisedC(DesignFunctionUnsupervisedBodyC *bod)
      : DesignerC(bod)
    {}
    //: Body ptr constructor.
    
    DesignFunctionUnsupervisedBodyC &Body()
    { return static_cast<DesignFunctionUnsupervisedBodyC &>(DesignerC::Body()); }
    //: Access body.

    const DesignFunctionUnsupervisedBodyC &Body() const
    { return static_cast<const DesignFunctionUnsupervisedBodyC &>(DesignerC::Body()); }
    //: Access body.
    
  public:
    FunctionC Apply(const SampleC<VectorC> &in)
    { return Body().Apply(in); }
    //: Create function from the given data.
    
    FunctionC Apply(const SampleC<VectorC> &in,const SampleC<RealT> &weight)
    { return Body().Apply(in,weight); }
    //: Create function from the given data, and sample weights.
    
    FunctionC Apply(SampleStreamC<VectorC> &in)
    { return Body().Apply(in); }
    //: Create function from the given data.
    // Note: Construction from a sample stream may not be implemented by all designers.
    
    FunctionC Apply(SampleStream2C<VectorC,RealT> &in)
    { return Body().Apply(in); }
    //: Create function from the given data, and sample weights.
    // Note: Construction from a sample stream may not be implemented by all designers.

    FunctionC Apply(const SampleC<TVectorC<float> > &in)
    { return Body().Apply(in); }
    //: Create function from the given data.

    FunctionC Apply(const SampleC<TVectorC<float> > &in,const SampleC<float> &weight)
    { return Body().Apply(in,weight); }
    //: Create function from the given data, and sample weights.

    FunctionC Apply(SampleStreamC<TVectorC<float> > &in)
    { return Body().Apply(in); }
    //: Create function from the given data.
    // Note: Construction from a sample stream may not be implemented by all designers.
    
    FunctionC Apply(SampleStream2C<TVectorC<float>,float> &in)
    { return Body().Apply(in); }
    //: Create function from the given data, and sample weights.
    // Note: Construction from a sample stream may not be implemented by all designers.

  };
  
  inline std::istream &operator>>(std::istream &strm,DesignFunctionUnsupervisedC &obj) {
    obj = DesignFunctionUnsupervisedC(strm);
    return strm;
  }
  //: Load from a stream.
  // Uses virtual constructor.
  
  inline std::ostream &operator<<(std::ostream &out,const DesignFunctionUnsupervisedC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
  inline BinIStreamC &operator>>(BinIStreamC &strm,DesignFunctionUnsupervisedC &obj) {
    obj = DesignFunctionUnsupervisedC(strm);
    return strm;
  }
  //: Load from a binary stream.
  // Uses virtual constructor.
  
  inline BinOStreamC &operator<<(BinOStreamC &out,const DesignFunctionUnsupervisedC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.

}


#endif
