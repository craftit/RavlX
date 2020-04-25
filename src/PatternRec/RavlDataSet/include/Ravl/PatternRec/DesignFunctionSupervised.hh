// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DESIGNFUNCTIONSUPERVISED_HEADER
#define RAVL_DESIGNFUNCTIONSUPERVISED_HEADER 1
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! author="Charles Galambos"
//! docentry="Ravl.API.Pattern Recognition.Functions"
//! file="Ravl/PatternRec/DataSet/DesignFunctionSupervised.hh"

#include "Ravl/PatternRec/Function.hh"
#include "Ravl/PatternRec/Sample.hh"
#include "Ravl/PatternRec/Designer.hh"
#include "Ravl/PatternRec/SampleStream.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Supervised function designer.
  
  class DesignFunctionSupervisedBodyC
    : public DesignerBodyC
  {
  public:
    DesignFunctionSupervisedBodyC()
    {}
    //: Default constructor.
    
    DesignFunctionSupervisedBodyC(const XMLFactoryContextC &factory);
    //: Construct from XML factory
    
    DesignFunctionSupervisedBodyC(std::istream &strm);
    //: Load from stream.
    
    DesignFunctionSupervisedBodyC(BinIStreamC &strm);
    //: Load from binary stream.
    
    virtual bool Save (std::ostream &out) const;
    //: Writes object to stream.
    
    virtual bool Save (BinOStreamC &out) const;
    //: Writes object to binary stream.

    virtual FunctionC Apply(const SampleC<VectorC> &in,const SampleC<VectorC> &out);
    //: Create function from the given data.
    
    virtual FunctionC Apply(const SampleC<VectorC> &in,const SampleC<VectorC> &out,const SampleC<RealT> &weight);
    //: Create function from the given data, and sample weights.
    
    virtual FunctionC Apply(SampleStream2C<VectorC,VectorC > &in);
    //: Create function from the given data.
    // Note: Construction from a sample stream may not be implemented by all designers.
    
    virtual FunctionC Apply(SampleStream3C<VectorC,VectorC,RealT> &in);
    //: Create function from the given data, and sample weights.
    // Note: Construction from a sample stream may not be implemented by all designers.

    virtual FunctionC Apply(const SampleC<TVectorC<float> > &in,const SampleC<TVectorC<float> > &out);
    //: Create function from the given data.

    virtual FunctionC Apply(const SampleC<TVectorC<float> > &in,const SampleC<TVectorC<float> > &out,const SampleC<float> &weight);
    //: Create function from the given data, and sample weights.

    virtual FunctionC Apply(SampleStream2C<TVectorC<float>,TVectorC<float> > &in);
    //: Create function from the given data.
    // Note: Construction from a sample stream may not be implemented by all designers.
    
    virtual FunctionC Apply(SampleStream3C<TVectorC<float>,TVectorC<float>,float> &in);
    //: Create function from the given data, and sample weights.
    // Note: Construction from a sample stream may not be implemented by all designers.

    virtual bool Reset();
    //: Reset the designer to an initial state
    
  };
  
  //! userlevel=Normal
  //: Supervised function designer.
  
  class DesignFunctionSupervisedC
    : public DesignerC
  {
  public:
    DesignFunctionSupervisedC()
    {}
    //: Default constructor.
    // Creates an invalid handle.

    DesignFunctionSupervisedC(const DesignFunctionSupervisedC &other)
     : DesignerC(other)
    {}
    //: Copy constructor.

    DesignFunctionSupervisedC(const XMLFactoryContextC &factory)
      :  DesignerC(*new DesignFunctionSupervisedBodyC(factory))
    {}
    //: Construct from XML factory
    
    DesignFunctionSupervisedC(std::istream &strm);
    //: Load from stream.
    
    DesignFunctionSupervisedC(BinIStreamC &strm);
    //: Load from binary stream.
    
  protected:
    DesignFunctionSupervisedC(DesignFunctionSupervisedBodyC &bod)
      : DesignerC(bod)
    {}
    //: Body constructor.

    DesignFunctionSupervisedC(DesignFunctionSupervisedBodyC *bod)
      : DesignerC(bod)
    {}
    //: Body ptr constructor.
    
    DesignFunctionSupervisedBodyC &Body()
    { return static_cast<DesignFunctionSupervisedBodyC &>(DesignerC::Body()); }
    //: Access body.
    
    const DesignFunctionSupervisedBodyC &Body() const
    { return static_cast<const DesignFunctionSupervisedBodyC &>(DesignerC::Body()); }
    //: Access body.
    
  public:
    DesignFunctionSupervisedC Copy() const
    { return DesignFunctionSupervisedC(dynamic_cast<DesignFunctionSupervisedBodyC &>(Body().Copy())); }
     //: Make copy of body.

    FunctionC Apply(const SampleC<VectorC> &in,const SampleC<VectorC> &out)
    { return Body().Apply(in,out); }
    //: Create function from the given data.
    
    FunctionC Apply(const SampleC<VectorC> &in,const SampleC<VectorC> &out,const SampleC<RealT> &weight)
    { return Body().Apply(in,out,weight); }
    //: Create function from the given data, and sample weights.
    
    FunctionC Apply(SampleStream2C<VectorC,VectorC > &in)
    { return Body().Apply(in); }
    //: Create function from the given data (in,out)
    // Note: Construction from a sample stream may not be implemented by all designers.
    
    FunctionC Apply(SampleStream3C<VectorC,VectorC,RealT> &in)
    { return Body().Apply(in); }
    //: Create function from the given data, and sample weights. (in,out,weight)
    // Note: Construction from a sample stream may not be implemented by all designers.

    FunctionC Apply(const SampleC<TVectorC<float> > &in,const SampleC<TVectorC<float> > &out)
    { return Body().Apply(in,out); }
    //: Create function from the given data.

    FunctionC Apply(const SampleC<TVectorC<float> > &in,const SampleC<TVectorC<float> > &out,const SampleC<float> &weight)
    { return Body().Apply(in,out,weight); }
    //: Create function from the given data, and sample weights.

    FunctionC Apply(SampleStream2C<TVectorC<float>,TVectorC<float> > &in)
    { return Body().Apply(in); }
    //: Create function from the given data.
    // Note: Construction from a sample stream may not be implemented by all designers.
    
    FunctionC Apply(SampleStream3C<TVectorC<float>,TVectorC<float>,float> &in)
    { return Body().Apply(in); }
    //: Create function from the given data, and sample weights.
    // Note: Construction from a sample stream may not be implemented by all designers.

  };
  
  // DesignFunctionSupervisedC
  
  inline std::istream &operator>>(std::istream &strm,DesignFunctionSupervisedC &obj) {
    obj = DesignFunctionSupervisedC(strm);
    return strm;
  }
  //: Load from a stream.
  // Uses virtual constructor.
  
  inline std::ostream &operator<<(std::ostream &out,const DesignFunctionSupervisedC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
  inline BinIStreamC &operator>>(BinIStreamC &strm,DesignFunctionSupervisedC &obj) {
    obj = DesignFunctionSupervisedC(strm);
    return strm;
  }
  //: Load from a binary stream.
  // Uses virtual constructor.
  
  inline BinOStreamC &operator<<(BinOStreamC &out,const DesignFunctionSupervisedC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.

}


#endif
