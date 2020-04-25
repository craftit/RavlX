// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLLOGIC_NLPAGENDAITEM_HEADER
#define RAVLLOGIC_NLPAGENDAITEM_HEADER 1
///////////////////////////////////////////////////////
//! date="16/12/1996"
//! author="Charles Galambos"
//! userlevel=Develop
//! docentry="Ravl.API.Logic.Planning"
//! rcsid="$Id$"
//! file="Ravl/Logic/NonLinearPlanner/NLPAgendaItem.hh"
//! lib=RavlLogicNLP

#include "Ravl/Logic/MinTerm.hh"
#include "Ravl/Logic/NLPTypes.hh"

namespace RavlLogicN {
  
  class NLPAgendaOpenGoalC;
  class NLPAgendaThreatC;
  class NLPlannerBodyC;
  class NLPActionBodyC;
  class NLPActionC;
  class NonLinearPlanC;
  class NonLinearPlanBodyC;
  
  //! userlevel=Develop
  //: Agent item base body class.

  class NLPAgendaItemBodyC
    : public RCBodyVC 
  {
  public:
    inline NLPAgendaItemBodyC(const NLPStepNodeT &AStep)
      : CStep(AStep), 
	priority(4)
    { RavlAssert(AStep.IsValid()); }
    //: Constructor.
    
    virtual ~NLPAgendaItemBodyC() 
    { RavlAssert(CStep.IsValid()); }
    //: Destructor.
    
    // const PriorityT MaxPriority = 100;
    
    PriorityT Priority() const 
    { return priority; }
    //: Get priority of agenda item.
    // 1- Actual threat. LIFO
    // 2- Impossible OpenCondition.
    // 3- Singular OpenCondition.
    // 4- An open condition. LIFO
    
    virtual bool IsOpenGoal() const 
    { return false; }
    //: Is this an open goal ?
  
    virtual bool IsThreat() const 
    { return false; }
    //: Is this a threat ?
    
    inline const NLPStepNodeT &Step() const 
    { return CStep; }
    //: Get creating step.
  
    virtual NLPActionC GetAction(NonLinearPlanC &Plan);
    //: Get action to remedy problem.
    
  protected:
    NLPStepNodeT CStep; // Step that cause the creation of this agenda item.
    PriorityT priority;
    
  };
  
  //! userlevel=Normal
  //: Agent item base handle class.
  
  class NLPAgendaItemC 
    : public RCHandleC<NLPAgendaItemBodyC>
  {
  public:
    NLPAgendaItemC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
  protected:
    NLPAgendaItemC(NLPAgendaItemBodyC &bod)
      : RCHandleC<NLPAgendaItemBodyC>(bod)
    {}
    //: Body constructor.
    // Creates an invalid handle.
    
    NLPAgendaItemC(const NLPAgendaItemBodyC *bod)
      : RCHandleC<NLPAgendaItemBodyC>(bod)
    {}
    //: Body constructor.
    // Creates an invalid handle.
    
    inline NLPAgendaItemBodyC &Body() 
    { return RCHandleC<NLPAgendaItemBodyC>::Body(); }
    //: Access body.
    
    inline const NLPAgendaItemBodyC &Body() const 
    { return RCHandleC<NLPAgendaItemBodyC>::Body(); }
    //: Access body.
  
  public:
    PriorityT Priority() const 
    { return Body().Priority(); }
    // Get priority of agenda item.
    // 1- Actual threat. LIFO
    // 2- Impossible OpenCondition.
    // 3- Singular OpenCondition.
    // 4- An open condition. LIFO
  
    inline bool IsOpenGoal() const
    { return Body().IsOpenGoal(); }
    // Is this an open goal ?
  
    inline bool IsThreat() const
    { return Body().IsThreat(); }
    // Is this a threat ?
  
    inline const NLPStepNodeT &Step() const 
    { return Body().Step(); }
    // Get creating step.
  
    NLPActionC GetAction(NonLinearPlanC &Plan);
    // Get action to remedy problem.
    
  };

  //: Agenda item body open goal

  class NLPAgendaOpenGoalBodyC 
    : public NLPAgendaItemBodyC 
  {
  public:
    NLPAgendaOpenGoalBodyC(const NLPStepNodeT &AStep,const MinTermC &AMTR,NonLinearPlanBodyC &plan);
    // Constructor.
    
    virtual bool IsOpenGoal() const;
    // This is an open goal.
  
    virtual NLPActionC GetAction(NonLinearPlanC &Plan);
    // Get action to remedy problem.
    
    MinTermC &Cond(void)
    { return aCond; }
    // Get open condition.
    
    DListC<NLPStepC> &NewSteps() 
    { return newSteps; }
    // Get list of possible steps.
    
    MinTermC &OpenCond() 
    { return aCond; }
    // Get the open condition.
    
    bool IsSingle() const 
    { return newSteps.IsEmpty(); }
    // FIXME :- Not quite Single, but means it can only be resolved
    // by something in the 'Start' state.
    
  private:
    DListC<NLPStepC> newSteps;    // Steps that could be added to plan to resolve goal.
    MinTermC aCond; // Precondition which isn't met.

    friend class NLPAgendaOpenGoalC;
  };
  
  //! userlevel=Normal
  //: Agenda item open goal

  class NLPAgendaOpenGoalC 
    : public NLPAgendaItemC 
  {
  public:
    NLPAgendaOpenGoalC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    NLPAgendaOpenGoalC(const NLPStepNodeT &aStep,const MinTermC &aMTR,NonLinearPlanBodyC &plan)
      : NLPAgendaItemC(*new NLPAgendaOpenGoalBodyC(aStep,aMTR,plan))
    {}
    //: Constructor.
    
    NLPAgendaOpenGoalC(NLPAgendaItemC &handle)
      : NLPAgendaItemC(dynamic_cast<const NLPAgendaOpenGoalBodyC *>(BodyPtr(handle)))
    {}
    //: Base class constructor.
    
  protected:
    NLPAgendaOpenGoalC(NLPAgendaOpenGoalBodyC &bod)
      : NLPAgendaItemC(bod)
    {}
    //: Body constructor.
  
    NLPAgendaOpenGoalBodyC &Body() 
    { return static_cast<NLPAgendaOpenGoalBodyC &>(NLPAgendaItemC::Body()); }
    //: Access body.
    
    const NLPAgendaOpenGoalBodyC &Body() const
    { return static_cast<const NLPAgendaOpenGoalBodyC &>(NLPAgendaItemC::Body()); }
    //: Access body.
    
  public:
    MinTermC &Cond() 
    { return Body().Cond(); }
    //: Get open condition.
    
    DListC<NLPStepC> &NewSteps() 
    { return Body().NewSteps(); }
    //: Get list of possible steps.
    
    MinTermC &OpenCond() 
    { return Body().OpenCond(); }
    //: Get the open condition.
    
    bool IsSingle() const 
    { return Body().IsSingle(); }
    // FIXME :- Not quite Single, but means it can only be resolved
    // by something in the 'Start' state.
    
    friend class NLPAgendaOpenGoalBodyC;    
  };

  //: Agenda item body threat.

  class NLPAgendaThreatBodyC 
    : public NLPAgendaItemBodyC 
  {
  public:
    inline NLPAgendaThreatBodyC(NLPStepNodeT &aPost,NLPCausalLinkT aVictim)
      : NLPAgendaItemBodyC(aPost),
	Victim(aVictim)
    { RavlAssert(aVictim.IsValid()); }
    //: Constructor.
    
    virtual bool IsThreat() const;
    //: Yep this is a threat.
    
    virtual NLPActionC GetAction(NonLinearPlanC &Plan);
    //: Get action to remedy problem.
    
    inline BGraphTypesC::NodeHandleT StepPre(void) 
    { return Victim.Target(); }
    //: Step causing threat.
    
    inline BGraphTypesC::NodeHandleT StepPost(void) 
    { return Victim.Source(); }
    //: Step thats threatened.
    
    inline MinTermC &Threat(void) 
    { return Victim.Data().Cond(); }
    //: Threatening condition.
    
    inline NLPCausalLinkT &TheVictim() 
    { return Victim; }
    //: Get victim of threat. 
    // (FIXME :- should be const.)
    
    inline const NLPCausalLinkT &TheVictim() const 
    { return Victim; }
    //: Get victim of threat. 
    // (FIXME :- should be const.)
    
  private:
    NLPCausalLinkT Victim; // Victim of threat.
    
    friend class NLPAgendaThreadC;
  };

  //: Agenda item threat.

  class NLPAgendaThreatC 
    : public NLPAgendaItemC 
  {
  public:
    NLPAgendaThreatC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
  
    NLPAgendaThreatC(NLPStepNodeT &APost,NLPCausalLinkT AVictim)
      : NLPAgendaItemC(*new NLPAgendaThreatBodyC(APost,AVictim))
    {}
    //: Constructor.

    NLPAgendaThreatC(NLPAgendaItemC &handle)
      : NLPAgendaItemC(dynamic_cast<const NLPAgendaThreatBodyC *>(BodyPtr(handle)))
    {}
    //: Converting constructor.

  protected:
    NLPAgendaThreatC(NLPAgendaThreatBodyC &bod)
      : NLPAgendaItemC(bod)
    {}
    //: Body constructor.
  
    NLPAgendaThreatBodyC &Body() 
    { return static_cast<NLPAgendaThreatBodyC &>(NLPAgendaItemC::Body()); }
    //: Access body.
    
    const NLPAgendaThreatBodyC &Body() const
    { return static_cast<const NLPAgendaThreatBodyC &>(NLPAgendaItemC::Body()); }
    //: Access body.
  
  public:
    inline BGraphTypesC::NodeHandleT StepPre(void) 
    { return Body().StepPre(); }
    // Step causing threat.
  
    inline BGraphTypesC::NodeHandleT StepPost(void) 
    { return Body().StepPost(); }
    // Step thats threatened.
  
    inline MinTermC Threat(void) 
    { return Body().Threat(); }
    // Threatening condition.
    
    inline NLPCausalLinkT &TheVictim() 
    { return Body().TheVictim(); }
    // Get victim of threat. (FIXME :- should be const.)
    
    inline const NLPCausalLinkT &TheVictim() const
    { return Body().TheVictim(); }
    // Get victim of threat. (FIXME :- should be const.)
    
    friend class NLPAgendaThreatBodyC;    
  };
  
}
#endif
