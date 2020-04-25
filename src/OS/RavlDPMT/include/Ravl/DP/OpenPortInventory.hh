// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2010, OmniPerception Ltd
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_SHAREINVENTORY_HH
#define	RAVL_SHAREINVENTORY_HH
//! lib=RavlDPMT

#include "Ravl/DP/Port.hh"
#include "Ravl/Hash.hh"
#include "Ravl/SmartPtr.hh"
#include "Ravl/Threads/RWLock.hh"

namespace RavlN {

  //: Inventory of currently open ports.
  // This is useful where there are auxiliary channels
  // that may also be opened.   The 'name' is typically
  // the name passed to open stream.  The inventory only
  // keeps handles to open ports, when the number of handles
  // drops to 1 (only in the inventory) the handle will be
  // deleted within half a second.
  
  class OpenPortInventoryC
    : public RCBodyVC
  {
  public:
    OpenPortInventoryC();
    //: Constructor
    
    bool Register(const StringC &name,DPPortC &port);
    //: Register a port with the inventory.

    bool Lookup(const StringC &name,DPPortC &port);
    //: Lookup a named port in the inventory.

    typedef RavlN::SmartPtrC<OpenPortInventoryC> RefT;
    //: Smart pointer to an inventory.
    
  protected:
    bool DoHouseKeeping();
    //: Scan port list of those ready to be closed.

    RWLockC m_access;
    HashC<StringC,DPPortC> m_ports;
    UIntT m_houseKeepingScheduleId;
  };

  //! Access the global open port inventory.
  OpenPortInventoryC &GlobalOpenPortInventory();
}


#endif	/* SHAREINVENTORY_HH */

