// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLLOGIC_NLPACTION_HEADER
#define RAVLLOGIC_NLPACTION_HEADER 1
///////////////////////////////////////////////////////
//! author="Charles Galambos"
//! date="18/3/1997"
//! docentry="Ravl.API.Logic.Planning"
//! userlevel=Develop
//! file="Ravl/Logic/NonLinearPlanner/NLPAction.hh"
//! lib=RavlLogicNLP

#include "Ravl/Logic/NLPTypes.hh"
#include "Ravl/Logic/NLPAgendaItem.hh"
#include "Ravl/Logic/NonLinearPlan.hh"

namespace RavlLogicN {
  
  class NLPlannerC;

  //////////////////////////
  //: Actions taken to remedy AgendaItems.

  class NLPActionBodyC 
    : public RCBodyVC 
  {
  public:
    NLPActionBodyC(const NonLinearPlanC &Plan,const NLPAgendaItemC &Item);
    //: Construct an action on a plan to address agenda item.
  
    virtual bool Next(NonLinearPlanC &Plan);
    //: Get next solution.
    
    virtual NLPAgendaItemC AgendaItem();
    //: Get agenda item being addressed.
    
    NonLinearPlanC &Plan() 
    { return plan; }
    //: Access plan.
    
  private:
    NonLinearPlanC plan; // Plan to take action on.
  };
  
  //: Actions taken to remedy AgendaItems.
  
  class NLPActionC 
    : public RCHandleC<NLPActionBodyC>
  {
  public:
    NLPActionC()
    {}
    //: Default constructor.
    // Creates an invalid handle.

    NLPActionC(NLPActionBodyC &bod)
      : RCHandleC<NLPActionBodyC>(bod)
    {}
    //: Body constructor.

  protected:
    inline NLPActionBodyC &Body() 
    { return RCHandleC<NLPActionBodyC>::Body(); }
    //: Access.
  
    inline const NLPActionBodyC &Body() const 
    { return RCHandleC<NLPActionBodyC>::Body(); }
    //: Access.
  
  public:
  
    bool Next(NonLinearPlanC &Plan)
    { return Body().Next(Plan); }
    //: Get next solution.
  
    NLPAgendaItemC AgendaItem()
    { return Body().AgendaItem(); }
    //: Get agenda item being addressed.
    
    NonLinearPlanC &Plan() 
    { return Body().Plan(); }
    //: Access plan.
  };

  //////////////////////////
  //: Remedy an open goal.

  class NLPActionOpenGoalBodyC
    : public NLPActionBodyC 
  {
  public:
    NLPActionOpenGoalBodyC(const NonLinearPlanC &Plan,const NLPAgendaOpenGoalC &OG);
    //: Construct action.
  
    virtual NLPAgendaItemC AgendaItem() 
    { return openGoal; }
    //: Get agenda item being addressed.
    
    virtual bool Next(NonLinearPlanC &Plan);
    //: Get next solution.
  
    const NLPStepNodeT &Step() const 
    { return openGoal.Step(); }
    //: Get creating step.
    
    void BuildPossSteps(NonLinearPlanC &aNLPlan);
    //: Build list of exiting steps in plan that may help.
    
    MinTermC &OpenCond() 
    { return openGoal.OpenCond(); }
    //: Get the open condition.
    
  private:
    DLIterC<NLPStepC> newSteps;
    DLIterC<NLPStepNodeT> possSteps;
    NLPAgendaOpenGoalC openGoal;
  };

  //////////////////////////
  //: Remedy an open goal handle.

  class NLPActionOpenGoalC 
    : public NLPActionC 
  {
  public:
    NLPActionOpenGoalC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    NLPActionOpenGoalC(NonLinearPlanC &plan,const NLPAgendaOpenGoalC &og)
      : NLPActionC(*new NLPActionOpenGoalBodyC(plan,og))
    {}
    //: Default constructor.
  };

  //////////////////////////
  //: Remedy an Threat.

  class NLPActionThreatBodyC 
    : public NLPActionBodyC 
  {
  public:
    NLPActionThreatBodyC(const NonLinearPlanC &Plan,const NLPAgendaThreatC &Thr);
    //: Construct action.
    
    virtual NLPAgendaItemC AgendaItem() 
    { return threat; }
    //: Get agenda item being addressed.
    
    virtual bool Next(NonLinearPlanC &Plan);
    //: Get next solution.
    
    const NLPStepNodeT &Step() const 
    { return threat.Step(); }
    //: Get creating step.
    
    inline NLPStepNodeT StepPre() 
    { return Plan().NodeObjH(threat.TheVictim().Source()); }
    //: Step causing threat.
    
    inline NLPStepNodeT StepPost() 
    { return Plan().NodeObjH(threat.TheVictim().Target()); }
    //: Step thats threatened.
    
  private:
    NLPAgendaThreatC threat;
    bool donePre,donePost;
  };
  
  //////////////////////////
  //: Remedy an Threat handle

  class NLPActionThreatC
    : public NLPActionC 
  {
  public:
    NLPActionThreatC()
    {}
    //: Default action.
    // Creates an invalid handle.
    
    NLPActionThreatC(NonLinearPlanC &plan,const NLPAgendaThreatC &thr)
      : NLPActionC(*new NLPActionThreatBodyC(plan,thr))
    {}
    //: Constructor.
  };
}
#endif

