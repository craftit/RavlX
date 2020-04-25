// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2010, OmniPerception Ltd
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlDPMT

#include <vector>

#include "Ravl/DP/OpenPortInventory.hh"
#include "Ravl/Hash.hh"
#include "Ravl/HashIter.hh"
#include "Ravl/RefCounter.hh"
#include "Ravl/CallMethodPtrs.hh"
#include "Ravl/Threads/TimedTriggerQueue.hh"

namespace RavlN {

  static const UIntT g_invalidTaskId = (UIntT)-1;
  
  //: Constructor
  OpenPortInventoryC::OpenPortInventoryC()
   : m_houseKeepingScheduleId(g_invalidTaskId)
  {}

  //: Register a port with the inventory.
  bool OpenPortInventoryC::Register(const StringC &name,DPPortC &port)
  {
    RWLockHoldC hold(m_access,RavlN::RWLOCK_WRITE);
    // Do we need to start house keeping ?
    if(m_houseKeepingScheduleId == g_invalidTaskId) {
      m_houseKeepingScheduleId = GlobalTriggerQueue().SchedulePeriodic(TriggerPtr(RefT(this),&OpenPortInventoryC::DoHouseKeeping),0.5);
    }
    m_ports[name] = port;
    return true;
  }

  //: Lookup a named port in the inventory.

  bool OpenPortInventoryC::Lookup(const StringC &name,DPPortC &port)
  {
    RWLockHoldC hold(m_access,RavlN::RWLOCK_READONLY);
    return m_ports.Lookup(name,port);
  }

  //: Do maintence on open port list.

  bool OpenPortInventoryC::DoHouseKeeping() {
    std::vector<StringC> toDie;
    RWLockHoldC hold(m_access,RavlN::RWLOCK_WRITE);
    for(HashIterC<StringC,DPPortC> it(m_ports);it;it++) {
      if(it.Data().References() == 1) {
        toDie.push_back(it.Key());
      }
    }
    // Remove entries from the table.
    for(unsigned i = 0;i < toDie.size();i++)
      m_ports.Del(toDie[i]);

    // If there's nothing left to scan stop house keeping.
    if(m_ports.IsEmpty()) {
      GlobalTriggerQueue().Cancel(m_houseKeepingScheduleId);
      m_houseKeepingScheduleId = g_invalidTaskId;
    }
    return true;
  }

  //: Access the global open port inventory.
  
  OpenPortInventoryC &GlobalOpenPortInventory() {
    static OpenPortInventoryC::RefT inventory = new OpenPortInventoryC();
    return *inventory;
  }

}
