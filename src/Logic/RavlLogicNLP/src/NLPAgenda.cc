// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlLogicNLP
//! date="16/12/1996"
//! author="Charles Galambos"
//! file="Ravl/Logic/NonLinearPlanner/NLPAgenda.cc"

#include "Ravl/Logic/NLPAgenda.hh"

namespace RavlLogicN {
  
  NLPAgendaItemC NLPAgendaC::GetFirst() {
    if(!threats.IsEmpty())
      return threats.Pop();
    OpenGoalCount--;
    if(!singleOpenGoal.IsEmpty())
      return singleOpenGoal.Pop();
    return OpenGoal.Pop();
  }
  
  NLPAgendaItemC NLPAgendaC::First() {
    if(!threats.IsEmpty())
      return threats.First();
    if(!singleOpenGoal.IsEmpty())
      return singleOpenGoal.First();
    return OpenGoal.First();
  }

  void NLPAgendaC::Insert(const NLPAgendaOpenGoalC &item) { 
    RavlAssert(item.IsValid());
    OpenGoalCount++;
    if(item.IsSingle())
      singleOpenGoal.Push(item);
    else
      OpenGoal.Push(item);
  }
  
  void NLPAgendaC::InsFirst(const NLPAgendaItemC &anItem) {
    NLPAgendaItemC item(anItem);
    if(item.IsThreat()) {
      NLPAgendaThreatC anAT(item);
      RavlAssert(anAT.IsValid());
      Insert(anAT);
      return ;
    }
    RavlAssert(item.IsOpenGoal());
    NLPAgendaOpenGoalC anOG(item);
    RavlAssert(anOG.IsValid());
    Insert(anOG);
  }

  
  void NLPAgendaC::Del(NLPStepNodeT &Step) {
    RavlAssert(0); // Obsolete.
#if 1
    while(!threats.IsEmpty()) {
      if(!(threats.Top().Step() == Step))
	break;
      threats.DelFirst();
    }
#else
    for(DLIterC<ThreatRefT> iter(threats);iter.IsElm();iter.Next()) {
      if(!(iter.Data().Step() == Step))
	break;
      iter.Del();
    }
#endif
    while(!singleOpenGoal.IsEmpty()) {
      if(!(singleOpenGoal.Top().Step() == Step))
	break;
      singleOpenGoal.DelFirst();
      OpenGoalCount--;
    }
    while(!OpenGoal.IsEmpty()) {
      if(!(OpenGoal.Top().Step() == Step))
	break;
      OpenGoal.DelFirst();
      OpenGoalCount--;
    }
  }
  
  void NLPAgendaC::DelThreat(NLPCausalLinkT &Victim) {
    RavlAssert(0); // Obsolete.
#if 1 
    while(!threats.IsEmpty()) {
      if(!(threats.Top().TheVictim() == Victim))
	break;
      threats.DelFirst();
    }
#else
    for(DLIterC<ThreatRefT> iter(threats);iter.IsElm();iter.Next()) {
      if(!(iter.Data().TheVictim() == Victim))
	break;
      iter.Del();
    }
#endif
  }
  
  ///////////////////////////////
  // Empty agenda.
  
  void NLPAgendaC::Empty() {
    threats.Empty();
    singleOpenGoal.Empty();
    OpenGoal.Empty();
  }
}

