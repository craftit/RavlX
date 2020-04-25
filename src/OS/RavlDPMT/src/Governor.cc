// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! author="Charles Galambos"
//! date="12/10/98"
//! lib=RavlDPMT
//! file="Ravl/OS/DataProc/Governor.cc"

#include "Ravl/DP/Governor.hh"

namespace RavlN {
  
  //: Wait for timeup.
  
  void DPGovernorBaseBodyC::WaitForTimeup() {
    // Any delay set ?
    if(delay <= 0 && minDelay <= 0)
      return ;
    // What time is it now ?
    DateC now(true);
    RealT diff = (next - now).Double();
    // enforce a minimum delay
    // this ensures that the maximum rate is limited ie for GUI applications
    // it also allows other threads to execute...
    if(diff < minDelay) {
      if(minDelay > 0) {
        next = now + minDelay;
        diff =(next-now).Double();        
      }
    }
    // now wait and setup for next.
    if (diff > 0)
     { m_bypass.Wait(diff); }

    if (!m_persistBypass)
      m_bypass.Reset();

    next.SetToNow();
    next += delay;
    frameCnt++;
  }

  //: Set new delay.
  bool DPGovernorBaseBodyC::SetDelay(RealT newDelay) { 
    if(newDelay < 0) {
      delay = 0;
      //m_bypass.Post();
      return false;
    }
    delay = newDelay;
    //m_bypass.Post();
    return true;
  }


  //: Toggle bypass mode.
  bool DPGovernorBaseBodyC::Bypass(bool bypass, bool persist)
  {
    m_persistBypass = persist;
    if (bypass)
      m_bypass.Post();
    else
      m_bypass.Reset();
    return true; 
  }

  
  
}
