// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_PROCESS21_HEADER
#define RAVL_PROCESS21_HEADER 1
///////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.Data Processing.Processes" 
//! date="23/7/2002"
//! lib=RavlIO
//! file="Ravl/Core/IO/Process21.hh"

#include "Ravl/DP/Process.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Process body with 2 inputs and one output.
  
  template<class In1T,class In2T,class Out1T>
  class DPProcess21BodyC
    : public DPProcessBaseBodyC
  {
  public:
    DPProcess21BodyC()
    {}
    //: Constructor.

    DPProcess21BodyC(std::istream &in) 
      : DPProcessBaseBodyC(in),
	DPEntityBodyC(in)
    {}
    //: Stream constructor.
    
    DPProcess21BodyC(BinIStreamC &in) 
      : DPProcessBaseBodyC(in),
	DPEntityBodyC(in)
    {}
    //: Stream constructor.
    
    virtual bool Save(std::ostream &out) const
    { return DPProcessBaseBodyC::Save(out); }
    //: Save to std::ostream.
    
    virtual bool Save(BinOStreamC &out) const
    { return DPProcessBaseBodyC::Save(out); }
    //: Save to binary stream.  
    
    virtual UIntT NoInputs() const
    { return 2; }
    //: Get number of inputs..    
    
    virtual const std::type_info &InputType(int n = 0) const {
      switch(n)  {
      case 0: return typeid(In1T);
      case 1: return typeid(In2T);
      }
      return typeid(void);
    }
    //: Get input types.
    
    virtual const std::type_info &OutputType(int n = 0) const {
      if(n == 0) return typeid(Out1T); 
      return typeid(void);
    }
    //: Get output types.
    
    virtual Out1T Apply(const In1T &d1,const In2T &d2) = 0;
    //: Do operation.
    
  protected:
    
  };

  //! userlevel=Advanced
  //: Process body with 2 inputs and one output.
  
  template<class In1T,class In2T,class Out1T>
  class DPProcess21C
    : public DPProcessBaseC
  {
  public:
    DPProcess21C()
    {}
    //: Default constructor.
    
    DPProcess21C(std::istream &in)
      : DPProcessBaseC(in)
    {}
    //: Stream constructor.
    
    DPProcess21C(BinIStreamC &in)
      : DPProcessBaseC(in)
    {}
    //: Binary stream constructor.
    
  protected:
    DPProcess21C(DPProcess21BodyC<In1T,In2T,Out1T> &bod)
      : DPProcessBaseC (bod)
    {}
    //: Body constructor.
    
    DPProcess21BodyC<In1T,In2T,Out1T> &Body()
    { return dynamic_cast<DPProcess21BodyC<In1T,In2T,Out1T> &>(DPEntityC::Body()); }
    //: Access body.

    const DPProcess21BodyC<In1T,In2T,Out1T> &Body() const
    { return dynamic_cast<const DPProcess21BodyC<In1T,In2T,Out1T> &>(DPEntityC::Body()); }
    //: Access body.
    
  public:
    
    Out1T Apply(const In1T &d1,const In2T &d2)
    { return Body().Apply(d1,d2); }
    //: Do operation.
    
  };
  
  
}

#endif
