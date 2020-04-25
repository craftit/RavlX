// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DPIOPORT_HEADER
#define RAVL_DPIOPORT_HEADER 1
//! docentry="Ravl.API.Core.Data Processing.Ports" 
//! file="Ravl/Core/IO/IOPort.hh"
//! rcsid="$Id$"
//! lib=RavlIO
//! author="Charles Galambos"
//! date="16/06/1998"

#include "Ravl/DP/Port.hh"
#include "Ravl/DP/StreamOp.hh"

namespace RavlN {

  //! userlevel=Develop
  //: Input/Output port body.
  
  template<class InT,class OutT>
  class DPIOPortBodyC 
    : public DPIPortBodyC<InT>, 
      public DPOPortBodyC<OutT>,
      public DPStreamOpBodyC
  {
  public:
    DPIOPortBodyC() 
    {}
    //: Default constructor.
    
    DPIOPortBodyC(const StringC &nportId)
      : DPIPortBodyC<InT>(nportId),
	DPOPortBodyC<OutT>(nportId)
    {}
    //: Constructor with a port id.
    
    DPIOPortBodyC(std::istream &in) 
      : DPIPortBodyC<InT>(in),
	DPOPortBodyC<OutT>(in)
    {}
    //: Stream constructor.
    
    virtual bool Save(std::ostream &out) const  {
      if(!DPIPortBodyC<InT>::Save(out))
	return false;
      return DPOPortBodyC<OutT>::Save(out);
    }
    //: Save to std::ostream.

    virtual DListC<DPIPortBaseC> IPorts() const;
    //: Input ports.
    
    virtual DListC<DPOPortBaseC> OPorts() const;
    //: Output ports
    
  };
  
  //////////////////////////////
  //! userlevel=Normal
  //: Input/Output port.
  
  template<class InT,class OutT>
  class DPIOPortC 
    : public DPIPortC<InT>, 
      public DPOPortC<OutT>,
      public DPStreamOpC
  {
  public:
    DPIOPortC() 
      : DPEntityC(true)
    {}
    // Default constructor.
    
    DPIOPortC(DPIOPortBodyC<InT,OutT> &bod) 
      : DPEntityC(bod),
	DPIPortC<InT>(bod),
	DPOPortC<OutT>(bod),
	DPStreamOpC(bod)
    {}
    //: Body constructor.
    
    DPIOPortC(std::istream &in) 
      : DPEntityC(in)
    {}
    //: Stream constructor.
    
    inline DPIOPortC<InT,OutT> Copy() const  { 
      if(!IsValid())
	return DPIOPortC<InT,OutT>(); // Nothing to copy.
      return DPIOPortC<InT,OutT>(dynamic_cast<DPIOPortBodyC<InT,OutT> &>(Body().Copy())); 
    }
    //: Make a copy of this process.
    
    DPIPortC<InT> &In() 
    { return *this; }
    //: Use as input port.
    // (Get from.)
    
    DPOPortC<OutT> &Out() 
    { return *this; }
    //: Use as output port.
    // (Put to.)
    
  protected:
    DPIOPortBodyC<InT,OutT> &Body() 
    { return dynamic_cast<DPIOPortBodyC<InT,OutT> &>(DPEntityC::Body()); }
    //: Access body.
    
    const DPIOPortBodyC<InT,OutT> &Body() const
    { return dynamic_cast<const DPIOPortBodyC<InT,OutT> &>(DPEntityC::Body()); }
    //: Access body.
  };
  
  template <class InT,class OutT>
  std::ostream & operator<<(std::ostream & s,const DPIOPortC<InT,OutT> &port) { 
    port.Save(s); 
    return s; 
  }
  
  template <class InT,class OutT>
  std::istream & operator>>(std::istream & s, DPIOPortC<InT,OutT> &port) { 
    DPIOPortC<InT,OutT> nport(s); 
    port = nport; 
    return s; 
  }

  //: Input ports.
  
  template<class InT,class OutT>
  DListC<DPIPortBaseC> DPIOPortBodyC<InT,OutT>::IPorts() const {
    DListC<DPIPortBaseC> ret;
    ret.InsLast(DPIOPortC<InT,OutT>(const_cast<DPIOPortBodyC<InT,OutT> &>(*this)));
    return ret;
  }
  
  //: Output ports.
  
  template<class InT,class OutT>
  DListC<DPOPortBaseC> DPIOPortBodyC<InT,OutT>::OPorts() const {
    DListC<DPOPortBaseC> ret;
    ret.InsLast(DPIOPortC<InT,OutT>(const_cast<DPIOPortBodyC<InT,OutT> &>(*this)));
    return ret;
  }

  
}



#endif
