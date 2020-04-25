// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DPEVENTSET_HEADER
#define RAVL_DPEVENTSET_HEADER 1
///////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Core/IO/EventSet.hh"
//! lib=RavlIO
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.Data Processing.Events"

#include "Ravl/DP/Event.hh"
#include "Ravl/HSet.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Set of events.
  
  class DPEventSetBodyC 
    : public DPEventBodyC
  {
  public:
    DPEventSetBodyC()
    {}
    //: Default constructor.
    // Creates an empty set.
    
    virtual bool Wait();
    //: Wait for all events in the set to occurre at least once.
    
    virtual IntT Occurred();
    //: Has event occured ?
    
    void operator+=(const DPEventC &event) 
    { set += event; }
    //: Add event.
    
    UIntT Size() const { return (UIntT) set.Size(); }
    //: Get size of set.
    
  protected:
    HSetC<DPEventC> set;
  };
  
  //! userlevel=Normal
  //: Set of events.
  
  class DPEventSetC 
    : public DPEventC
  {
  public:
    DPEventSetC()
      : DPEventC(*new DPEventSetBodyC())
    {}
    //: Default constructor.
    // Creates an empty set.
    
  protected:
    DPEventSetBodyC &Body() 
    { return static_cast<DPEventSetBodyC &>(DPEntityC::Body()); }
    //: Access body.
    
    const DPEventSetBodyC &Body() const
    { return static_cast<const DPEventSetBodyC &>(DPEntityC::Body()); }
    //: Access body.
    
  public:
    void operator+=(const DPEventC &event) 
    { Body().operator+=(event); }
    //: Add event.
    
    UIntT Size() const { return Body().Size(); }
    //: Get size of set.
  };
}
  
#endif
