// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DESIGNER_HEADER
#define RAVL_DESIGNER_HEADER 1
//! author="Charles Galambos"
//! docentry="Ravl.API.Pattern Recognition.Functions"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/DataSet/Designer.hh"

#include "Ravl/RCHandleV.hh"
#include "Ravl/Vector.hh"
#include "Ravl/XMLFactory.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Designer base class.
  // Abstract Function designer. This is provided to allow the tweeking 
  // of design parameters to be automated
  
  class DesignerBodyC
    : public RCBodyVC
  {
  public:
    DesignerBodyC();
    //: Default constructor.

    DesignerBodyC(const XMLFactoryContextC &factory);
    //: Construct from XML factory

    DesignerBodyC(std::istream &strm);
    //: Load from stream.
    
    DesignerBodyC(BinIStreamC &strm);
    //: Load from binary stream.
    
    virtual bool Save (std::ostream &out) const;
    //: Writes object to stream.
    
    virtual bool Save (BinOStreamC &out) const;
    //: Writes object to binary stream.
    
    virtual void ParameterLimits(VectorC &defaultValues,VectorC &min,VectorC &max,SArray1dC<StringC> &names) const;
    //: Get the default parameter values and their limits.

    virtual VectorC Parameters() const;
    //: Get the current parameters.

    virtual VectorC SetParameters(const VectorC &params);
    //: Set the current parameters.
    // Returns the current parameters, which may not be exactly those
    // set in 'params', but will be the closest legal values.
    
    virtual bool Reset();
    //: Reset the designer to an initial state

    bool IsVerbose() const
    { return m_verbose; }
    //: Test if we're in verbose mode.

    void SetVerbose(bool verbose)
    { m_verbose = verbose; }
    //: Set verbose mode.

  protected:
    bool m_verbose;
  };

  //! userlevel=Normal
  //: Designer base class.
  // Abstract Function designer. This is provided to allow the tweeking 
  // of design parameters to be automated
  
  class DesignerC
    : public RCHandleVC<DesignerBodyC>
  {
  public:
    DesignerC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    DesignerC(const XMLFactoryContextC &factory)
      :  RCHandleVC<DesignerBodyC>(*new DesignerBodyC(factory))
    {}
    //: Construct from XML factory
    
    DesignerC(std::istream &strm);
    //: Load from stream.
    
    DesignerC(BinIStreamC &strm);
    //: Load from binary stream.
    
  protected:
    DesignerC(DesignerBodyC &bod)
      : RCHandleVC<DesignerBodyC>(bod)
    {}
    //: Body constructor.

    DesignerC(DesignerBodyC *bod)
      : RCHandleVC<DesignerBodyC>(bod)
    {}
    //: Body ptr constructor.
    
    DesignerBodyC &Body()
    { return RCHandleVC<DesignerBodyC>::Body(); }
    //: Access body.
    
    const DesignerBodyC &Body() const
    { return RCHandleVC<DesignerBodyC>::Body(); }
    //: Access body.
    
  public:
    VectorC Parameters()
    { return Body().Parameters(); }
    //: Get the current parameters.
    // Returns the current parameters, which may not be exactly those
    // set in 'params', but will be the closest legal values.

    VectorC SetParameters(const VectorC &params)
    { return Body().SetParameters(params); }
    //: Set the current parameters.
    // Returns the current parameters, which may not be exactly those
    // set in 'params', but will be the closest legal values.

    void ParameterLimits(VectorC &defaultValues,VectorC &min,VectorC &max,SArray1dC<StringC> &names) const
    { Body().ParameterLimits(defaultValues,min,max,names); }
    //: Get the default parameter values and their limits.

    bool Reset()
    { return Body().Reset(); }
    //: Reset the designer to an initial state

    bool IsVerbose() const
    { return Body().IsVerbose(); }
    //: Test if we're in verbose mode.

    void SetVerbose(bool verbose)
    { Body().SetVerbose(verbose); }
    //: Set verbose mode.
    
  };
  
  inline std::istream &operator>>(std::istream &strm,DesignerC &obj) {
    obj = DesignerC(strm);
    return strm;
  }
  //: Load from a stream.
  // Uses virtual constructor.
  
  inline std::ostream &operator<<(std::ostream &out,const DesignerC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
  inline BinIStreamC &operator>>(BinIStreamC &strm,DesignerC &obj) {
    obj = DesignerC(strm);
    return strm;
  }
  //: Load from a binary stream.
  // Uses virtual constructor.
  
  inline BinOStreamC &operator<<(BinOStreamC &out,const DesignerC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.

  
}


#endif
