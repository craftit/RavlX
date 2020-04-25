// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DESIGNFUNCREDUCE_HEADER
#define RAVL_DESIGNFUNCREDUCE_HEADER 1
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! author="Charles Galambos"
//! docentry="Ravl.API.Pattern Recognition.Dimension Reduction"
//! file="Ravl/PatternRec/Modeling/DimensionReduction/DesignFuncReduce.hh"

#include "Ravl/PatternRec/DesignFunctionUnsupervised.hh"
#include "Ravl/Matrix.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Body for base class for designing a dimension reducing function
  
  class DesignFuncReduceBodyC
    : public DesignFunctionUnsupervisedBodyC
  {
  public:
    DesignFuncReduceBodyC()
      : varPreserved(1)
    {}
    //: Default constructor.
    
    DesignFuncReduceBodyC(RealT variationPreserved);
    // "variationPreserved" is amount of variation to attempt to preserve in reduced set:<br>
    // 0 -> None; 1 -> All; >1 (truncated to int) -> Size of set preserved.
    
    DesignFuncReduceBodyC(istream &strm);
    //: Load from stream.
    
    DesignFuncReduceBodyC(BinIStreamC &strm);
    //: Load from binary stream.
    
    virtual bool Save (ostream &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual bool Save (BinOStreamC &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    RealT VariationPreserved() const 
    { return varPreserved; }
    //: Returns amount of variation to attempt to preserve in reduced set.
    // 0 -> None; 1 -> All; >1 (truncated to int) -> Size of set preserved.
    
  protected:
    RealT varPreserved; // Amount of variation to preserve.
  };
  
  //! userlevel=Normal
  //: Base class for designing a dimension reducing function
  
  class DesignFuncReduceC
    : public DesignFunctionUnsupervisedC
  {
  public:
    DesignFuncReduceC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
  protected:
    DesignFuncReduceC(DesignFuncReduceBodyC &bod)
      : DesignFunctionUnsupervisedC(bod)
    {}
    //: Body constructor.
    
    DesignFuncReduceBodyC &Body()
    { return static_cast<DesignFuncReduceBodyC &>(DesignerC::Body()); }
    //: Access body.
    
    const DesignFuncReduceBodyC &Body() const
    { return static_cast<const DesignFuncReduceBodyC &>(DesignerC::Body()); }
    //: Access body.
    
  public:    
    RealT VariationPreserved() const 
    { return Body().VariationPreserved(); }
    //: Returns amount of variation to attempt to preserve in reduced set.
    // 0 -> None; 1 -> All; >1 (truncated to int) -> Size of set preserved.
  };

}



#endif
