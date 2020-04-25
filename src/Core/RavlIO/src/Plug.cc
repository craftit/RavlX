// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlIO
//! file="Ravl/Core/IO/Plug.cc"

#include "Ravl/DP/Plug.hh"

namespace RavlN {


  DPPlugBaseBodyC::DPPlugBaseBodyC()
    : m_hold(true)
  {}
  //: Default constructor.

  DPPlugBaseBodyC::DPPlugBaseBodyC(const DPEntityC &nhold)
    : m_hold(nhold)
  {}
  //: Constructor.

  DPPlugBaseBodyC::DPPlugBaseBodyC(const StringC &nPlugId,const DPEntityC &nhold)
    : DPEntityBodyC(nPlugId),
      m_hold(nhold)
  {
    SetEntityName(nPlugId);
    RavlAssert(EntityName() == nPlugId);
  }
  //: Constructor.

  // ------------------------------------------------------------------------
  
  DPIPlugBaseBodyC::DPIPlugBaseBodyC()
  {}
  //: Default constructor.

  DPIPlugBaseBodyC::DPIPlugBaseBodyC(const DPEntityC &nhold)
    : DPPlugBaseBodyC(nhold)
  {}
  //: Constructor.

  DPIPlugBaseBodyC::DPIPlugBaseBodyC(const StringC &nPlugId,const DPEntityC &nhold)
    : DPPlugBaseBodyC(nPlugId,nhold)
  {
    RavlAssert(EntityName() == nPlugId);
  }

  //: Set port.
  
  bool DPIPlugBaseBodyC::ConnectPort(const DPIPortBaseC &port) {
    RavlAssertMsg(0,"DPIPlugBaseBodyC::ConnectPort(), Abstract method called. \n");
    return false;
  }

  //: Return type of port.
  
  const std::type_info &DPIPlugBaseBodyC::InputType() const
  { return typeid(void); }
  
  ////////////////////////////////////////////////////////////////


  DPOPlugBaseBodyC::DPOPlugBaseBodyC()
  {}
  //: Default constructor.

  DPOPlugBaseBodyC::DPOPlugBaseBodyC(const DPEntityC &nhold)
    : DPPlugBaseBodyC(nhold)
  {}
  //: Constructor.
  
  DPOPlugBaseBodyC::DPOPlugBaseBodyC(const StringC &nPlugId,const DPEntityC &nhold)
    : DPPlugBaseBodyC(nPlugId,nhold)
  {
    RavlAssert(EntityName() == nPlugId);
  }
  //: Constructor.


  //: Set port.
  
  bool DPOPlugBaseBodyC::ConnectPort(const DPOPortBaseC &port) {
    RavlAssertMsg(0,"DPOPlugBaseBodyC::ConnectPort(), Abstract method called. \n");
    return false;
  }
  
  //: Return type of port.
  
  const std::type_info &DPOPlugBaseBodyC::OutputType() const 
  { return typeid(void); }

}
