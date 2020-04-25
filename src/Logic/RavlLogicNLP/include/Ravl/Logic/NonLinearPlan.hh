// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLLOGIC_NONLINEARPLAN_HEADER
#define RAVLLOGIC_NONLINEARPLAN_HEADER 1
////////////////////////////////////////////////////////////
//! file="Ravl/Logic/NonLinearPlanner/NonLinearPlan.hh"
//! date="12/12/1996"
//! author="Charles Galambos"
//! userlevel=Advanced
//! lib=RavlLogicNLP
//! docentry="Ravl.API.Logic.Planning"
//! example=exNonlinearPlanner.cc

#include "Ravl/Logic/BMinTermListIndex.hh"
#include "Ravl/BList.hh"
#include "Ravl/Logic/Tuple.hh"
#include "Ravl/Logic/LiteralIndex.hh"
#include "Ravl/Logic/NLPCausalLink.hh"
#include "Ravl/Logic/NLPAgenda.hh"
#include "Ravl/Logic/NLPTypes.hh"
#include "Ravl/Calls.hh"

namespace RavlLogicN {
  
  class NonLinearPlanC;
  
  //! userlevel=Develop
  //: The body of a non-linear plan.

  class NonLinearPlanBodyC
    : public RCBodyVC 
  {
  public:
    NonLinearPlanBodyC(const MinTermC &initCond,
		       const MinTermC &goalCond,
		       const CallFunc2C<MinTermC&,MinTermC&,DListC<NLPStepC> > &step);
    //: Constructor, 
    
    ~NonLinearPlanBodyC();
    //: Destructor.
    
    bool Init(const MinTermC &Start,const MinTermC &Goal);
    //: Initialise plan.
    
    NLPStepNodeT InsStep(const NLPStepC &Step,const MinTermC &Goal);
    //: Insert a step into the plan
  
    NLPStepNodeT InsStep(const NLPStepC &Step);
    //: Insert a step into the plan.
    
    NLPCausalLinkT InsCausalLink(NLPStepNodeT from,NLPStepNodeT to,MinTermC &Cond);
    //: Insert a causal link into the plan. Used to resolve open 
    //: goals.
  
    NLPOrderLinkT InsOrderLink(NLPStepNodeT from,NLPStepNodeT to);
    //: Insert an order link into the plan. Used to resolve threats.
  
    inline bool IsComplete(void) const 
    { return agenda.IsEmpty(); }
    //: Is plan complete.
    
    DListC<NLPStepC> GetLinear(void) const;
    //: Get a linear version of the plan.
  
    virtual void Dump(void) const;
    //: Dump current plan to stdout.
  
    inline IntT Score(void) const 
    { return steps + agenda.GetNoOpenGoals(); }
    //: Give the plan a score for A* search.
    
    const NLPStepNodeT &StartNode() const 
    { return startNode; }
    //: Const access to start node.
    
    const NLPStepNodeT &GoalNode() const
    { return goalNode; }
    //: Const access to goal node.
    
    inline BMinTermListIndexC<NLPStepNodeT> &PreConds()
    { return preConds; }
    //: Access PreConditions.
    
    inline const BMinTermListIndexC<NLPStepNodeT> &PreConds() const
    { return preConds; }
    //: Access PreConditions.
    
    inline BMinTermListIndexC<NLPStepNodeT> &PostConds()
    { return postConds; }
    //: Access PostConditions.
    
    inline const BMinTermListIndexC<NLPStepNodeT> &PostConds() const
    { return postConds; }
    //: Access PostConditions.
    
    inline IntT PlanID() const 
    { return planID; }
    //: Get a unique ID for the plan.
    
    inline NLPStepNodeT NodeObjH(BGraphTypesC::NodeHandleT H)
    { return plan.NodeObjH(H); }
    //: Convert a node handle to a ptr.
    
    inline bool IsValid(const NLPStepNodeT &Oth) const 
    { return plan.IsValid(Oth.ID()); }
    //: Check the handle is valid. (For debugging. )
    
    inline bool IsValid() const 
    { return plan.IsValid(); }
    //: Check plan is valid.
    
    bool IsUsefullNewStep(const NLPStepC &mt,const NLPStepNodeT &sn) const;
    //: Test if is useful to add postcondition to this plan.
    // Use to filter out the addition of useless steps.<p>
    // sn is the step which has the open goal that caused mt to be considered
    
    DListC<NLPStepC> ListSteps(const MinTermC &goalCond,const MinTermC &fullCond) { 
      RavlAssert(listSteps.IsValid());
      return listSteps(const_cast<MinTermC &>(goalCond),const_cast<MinTermC &>(fullCond)); 
    }
    //: List steps with the given postcondition.
    // GoalCond :- Condition that must be meet by Step,
    // FullCond :- 'Wish list' of conditions that also will be needed.
    // Heuristic ordering of steps, place the most likely to
    // be useful first.
    
    void DoDBCheck();
    //: debugging checks.
  
  protected:
    bool PostOpenCond(NLPStepNodeT &AStep,const MinTermC &OpenCond);
    //: Insert an open condition into the agenda.
    
    bool PostThreat(NLPStepNodeT &AStep,NLPCausalLinkT ALink);
    //: Insert an threat into the agenda.
    
    inline NLPAgendaItemC TopOfAgenda() 
    { return agenda.First(); }
    //: Look at current agenda item.
    
    inline NLPAgendaItemC GetTopOfAgenda() 
    { return agenda.GetFirst(); }
    //: Return current agenda item and remove it.
  
    inline IntT GetSteps() const 
    { return steps; }
    //: Get the number of steps in the current plan.
    
    inline IntT Steps() const 
    { return steps; }
    //: Get the number of steps in the current plan.
    
    CallFunc2C<MinTermC&,MinTermC&,DListC<NLPStepC> > &ListSteps()
    { return listSteps; }
    //: Access listStep methods.
    
  private:  
    IntT planID;
    //: Only really used to help in debugging.
  
    BDAGraphC<NLPStepC,NLPCausalLinkC> plan;
    //: Graph of states.
    // Links -> CausalLinkInfT.
    // Nodes -> Steps in plan.
    
    BindSetC bnds;
    //: List of variable binds in the plan.
    
    //BHashC<IntT,VLBind> negBnds;
    //: List of Negated binds.
  
    BMinTermListIndexC<BGraphNodeHC<NLPStepC,NLPCausalLinkC> > preConds;// Index by preconditions.
    //: Used to establish threats.
  
    BMinTermListIndexC<BGraphNodeHC<NLPStepC,NLPCausalLinkC> > postConds;// Index by postconditions.
    //: Used to help establish causal links.
    
    NLPAgendaC agenda;
    //: Agenda of open conditions.
  
    int steps; 
    //: Count of steps in the plan.
    
    NLPStepNodeT startNode; 
    // Inital node in plan.
    
    NLPStepNodeT goalNode;
    // Final node in plan.
    
    CallFunc2C<MinTermC&,MinTermC&,DListC<NLPStepC> > listSteps; // Goal condition, Full condition.
    //: List steps that could satisfy meet a goal condition.
    
    friend class NLPAgendaOpenGoal;
    friend class NLPAgendaOpenThreat;
    friend class NonLinearPlanC;
  };
  
  /////////////////////////////////////////////
  
  //! userlevel=Normal
  //: non-linear plan handle

  class NonLinearPlanC 
    : public RCHandleC<NonLinearPlanBodyC>
  {
  public:
    NonLinearPlanC()
    {}
    //: Default constructor.
    
    NonLinearPlanC(const MinTermC &initalCond,
		   const MinTermC &goalCond,
		   const CallFunc2C<MinTermC&,MinTermC&,DListC<NLPStepC> > &listSteps)
      : RCHandleC<NonLinearPlanBodyC>(*new NonLinearPlanBodyC(initalCond,goalCond,listSteps))
    {}
    //: Create a new plan with the given initial and goal condition.
    // initalCond - Initial condition.
    // goalCond   - Goal condition
    // listSteps  - Method for finding new steps for plan.
    
    bool IsValid() const 
    { return RCHandleC<NonLinearPlanBodyC>::IsValid(); }
    //: Is valid handle ?
    
    NonLinearPlanC Copy() const 
    { return NonLinearPlanC(*new NonLinearPlanBodyC(Body())); }
    //: Copy operator.
    
    inline bool Init(const MinTermC &Start,const MinTermC &Goal)
    { return Body().Init(Start,Goal); }
    //: Initialise plan.
  
    inline NLPStepNodeT InsStep(const NLPStepC &Step,const MinTermC &Goal)
    { return Body().InsStep(Step,Goal); }
    //: Insert a step into the plan
  
    inline NLPStepNodeT InsStep(const NLPStepC &Step)
    { return Body().InsStep(Step); }
    //: Insert a step into the plan.
  
    //  inline NLPStepNodeT InsStep(DPModelStepC<VLSymbC,MinTermC,MinTermC> &Step) { VLBindLst Tmp; return InsStep(Step,Tmp); }
    // Insert a step into the plan.
  
    inline NLPCausalLinkT InsCausalLink(NLPStepNodeT from,NLPStepNodeT to,MinTermC &Cond)
    { return Body().InsCausalLink(from,to,Cond); }
    //: Insert a causal link into the plan. Used to resolve open 
    //: goals.
  
    inline NLPOrderLinkT InsOrderLink(NLPStepNodeT from,NLPStepNodeT to)
    { return Body().InsOrderLink(from,to); }
    //: Insert an order link into the plan. Used to resolve threats.

    inline bool IsUsefullNewStep(const NLPStepC &mt,const NLPStepNodeT &sn) const
    { return Body().IsUsefullNewStep(mt,sn); }
    //: Test if is useful to add postcondition to this plan.
    // Use to filter out the addition of useless steps.
  
    inline bool IsComplete() const 
    { return Body().IsComplete(); }
    //: Is plan complete.
  
    inline DListC<NLPStepC> GetLinear() const
    { return Body().GetLinear(); }
    //: Get a linear version of the plan.
    
    inline void Dump() const
    { Body().Dump(); }
    //: Dump current plan to stdout.
  
    inline IntT Score(void) const
    { return Body().Score(); }
    //: Give the plan a score for A* search.
  
    inline const NLPStepNodeT &StartNode() const
    { return Body().StartNode(); }
    //: Const access to start node.
  
    inline const NLPStepNodeT &GoalNode() const
    { return Body().GoalNode(); }
    //: Const access to goal node.

  
    inline BMinTermListIndexC<NLPStepNodeT> &PreConds()
    { return Body().PreConds(); }
    //: Access PreConditions.
  
    inline BMinTermListIndexC<NLPStepNodeT> &PostConds()
    { return Body().PostConds(); }
    //: Access PostConditions.
  
    inline IntT PlanID() const
    { return Body().PlanID(); }
    //: Get a unique ID for the plan.
  
    inline NLPStepNodeT NodeObjH(BGraphTypesC::NodeHandleT H) 
    { return Body().NodeObjH(H) ; }
    //: Convert a node handle to a ptr.
  
    inline bool IsValid(const NLPStepNodeT &Oth) const
    { return Body().IsValid(Oth); }
    //: Check the handle is valid. (For debugging. )
  
    inline void DoDBCheck()
    { Body().DoDBCheck(); }
    //: debugging checks.
    
    inline IntT Steps() const 
    { return Body().Steps(); }
    //: Get the number of steps in the current plan.
    
    DListC<NLPStepC> ListSteps(const MinTermC &goalCond,const MinTermC &fullCond)
    { return Body().ListSteps(goalCond,fullCond); }
    //: Get a list of steps that will meet 'goalCond'.
    // GoalCond :- Condition that must be meet by Step,
    // FullCond :- 'Wish list' of conditions that also will be needed.
    // Heuristic ordering of steps, place the most likely to
    // be useful first.
    
    CallFunc2C<MinTermC&,MinTermC&,DListC<NLPStepC> > &ListSteps()
    { return Body().ListSteps(); }
    //: Access listStep methods.

    inline NLPAgendaItemC GetTopOfAgenda() 
    { return Body().GetTopOfAgenda(); }
    //: Return current agenda item and remove it.
    
  protected:
    NonLinearPlanC(NonLinearPlanBodyC &bod)
      : RCHandleC<NonLinearPlanBodyC>(bod)
    {}
    //: Body constructor.
    
    inline NLPAgendaItemC TopOfAgenda() 
    { return Body().TopOfAgenda(); }
    //: Look at current agenda item.
    
    friend class NonLinearPlanBodyC;
  };
  
}

#endif
