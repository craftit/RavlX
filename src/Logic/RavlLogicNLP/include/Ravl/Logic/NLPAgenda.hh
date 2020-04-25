// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLLOGIC_NLPAGENDA_HEADER
#define RAVLLOGIC_NLPAGENDA_HEADER 1
///////////////////////////////////////////////////////
//! author="Charles Galambos"
//! date="16/12/1996"
//! docentry="Ravl.API.Logic.Planning"
//! userlevel=Develop
//! file="Ravl/Logic/NonLinearPlanner/NLPAgenda.hh"
//! lib=RavlLogicNLP

#include "Ravl/BStack.hh"
#include "Ravl/Logic/NLPAgendaItem.hh"

namespace RavlLogicN { 

  //: Agenda for non linear plan.
  // This is a small object, but its cheap to copy.

  class NLPAgendaC 
  {
  public:  
    NLPAgendaC()
      : OpenGoalCount(0) 
    {}
    //: Default constructor.
    // Creates an empty agenda.
    
    void Insert(const NLPAgendaThreatC &Item)
    { threats.InsFirst(Item); }
    // Insert Threat into agenda.
    
    void Insert(const NLPAgendaOpenGoalC &Item);
    // Insert Open Goal into agenda.
    
    void InsFirst(const NLPAgendaItemC &Item);
    // Insert item into agenda.
    
    NLPAgendaItemC GetFirst();
    // Get First item from agenda.
  
    NLPAgendaItemC First();
    // Look at first item in agenda.
    
    void Del(NLPStepNodeT &Step);
    // Remove item from the agenda that are caused by this step.
    
    void DelThreat(NLPCausalLinkT &Victim);
    // Remove threats to causal link 'victim'.
  
    bool IsEmpty() const 
    { return threats.IsEmpty() && singleOpenGoal.IsEmpty() && OpenGoal.IsEmpty(); }
    // Is agenda empty ?
    
    void Empty(void);
    // Empty agenda.
  
    int GetNoOpenGoals(void) const { return OpenGoalCount; }
    // Get number of open goals.
  
  protected:
    BStackC<NLPAgendaThreatC> threats;
    BStackC<NLPAgendaOpenGoalC> singleOpenGoal;
    BStackC<NLPAgendaOpenGoalC> OpenGoal;  
    int OpenGoalCount;
  };

  /////////////////////////////////////////////////////////////////////////
  
}
#endif
