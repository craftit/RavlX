// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_NLPLANNER_HEADER
#define RAVL_NLPLANNER_HEADER 1
////////////////////////////////////////////////////////////
//! file="Ravl/Logic/NonLinearPlanner/NLPlanner.hh"
//! lib=RavlLogicNLP
//! author="Charles Galambos"
//! date="4/1/1997"
//! docentry="Ravl.API.Logic.Planning"
//! rcsid="$Id$"

#include "Ravl/PriQueueL.hh"
#include "Ravl/Logic/NonLinearPlan.hh"
#include "Ravl/Logic/NLPAction.hh"
#include "Ravl/Logic/MinTerm.hh"
#include "Ravl/Logic/State.hh"
#include "Ravl/Logic/NLPTypes.hh"
#include "Ravl/IntC.hh"

namespace RavlLogicN {
  
  //! userlevel=Develop
  //: Planner base clase.
  // Overide the virtual functions to make a real planner.

  class NLPlannerBodyC 
    : public RCBodyC
  {
  public:
    NLPlannerBodyC();
    //: Default constructor.
    
    NLPlannerBodyC(const DListC<NLPStepC> &steps);
    //: Constructor.
    
    DListC<NLPStepC> Apply(const StateC &state,const MinTermC &goals);
    //: Construct plan.
    
    bool NewPlan(const StateC &start,const MinTermC &goal);
    //: Create a new plan.
  
    bool CompletePlan();
    // Try and complete plan,
    // Ret: False=Failed.
  
    bool IsComplete() const { return bestPlan.IsValid(); }
    // Is plan complete.
    
    bool GetLinearPlan(DListC<NLPStepC> &LPlan);
    // Create a linear plan.
  
    DListC<NLPStepC> GetBestLinearPlan(void) const { 
      RavlAssert(bestPlan.IsValid());
      return bestPlan.GetLinear(); 
    }
    // Get best linear plan found so far.
  
    int GetBestSize(void) const { return bestSize; }
    // Get size of best plan found so far.
  
    NonLinearPlanC &NLPlan() { return bestPlan; }
    // Get current plan.
  
    const NonLinearPlanC &NLPlan() const { return bestPlan; }
    // Get current plan.
    
    DListC<NLPStepC> ListSteps(MinTermC &postCond,MinTermC &fullCond);
    // List steps with the given postcondition.
    
    static int Distance(const MinTermC &MT1, const MinTermC &MT2);
    // Find distance between two terms.

    void SetMaxSearch(int limit) { maxSearch = limit; }
    // Limit size of search.
    
    inline StepListIDC PreCondID(const LiteralC &symb);
    // Get ID for step precondition.
  
    inline StepListIDC PostCondID(const LiteralC &symb);
    // Get ID for step postcondition.
  
    IntT GetPlanNo() { return planCount++; }
    // Get an id for the current plan.
    

    void DoDBCheck(NonLinearPlanBodyC &aPlan);
    // Debugging checks.
    
    void DoDBCheck(NonLinearPlanC aPlan) { 
      //DoDBCheck(aPlan.Body()); 
    }
    // Debugging checks.
    
  private:
    //DPModelC<LiteralC,StateC,MinTermC,MinTermC> model;
    //: Model to use in planning.

    DListC<NLPStepC> steps; // List of available steps.
    
    IntT planCount;
  
    PriQueueLC<IntT,NLPActionC> plans; // Priority queue of plan/action pairs.
    
    LiteralIndexC<UIntC> conds;
    // Index by preconditions.
    // Used to establish threats.
    
    UIntC allocCondID; // Starts from 1.
    // Allocation counter for condition ID's.
    
    CallFunc2C<MinTermC&,MinTermC&,DListC<NLPStepC> > listSteps; // Goal condition, Full condition.
    //: List steps that might could be meet a goal condition.
    
    int maxSearch;   // How long to spend searching, Default:-1=Infinit.
    int limSearch;   // Max size of search, after a plan has been found.
    int curSearch;   // Number of plans considered so far.
  
    enum MaxIntT { MaxBest = 0x7fffffff } ;
  
    int bestSize;  // Size of best plan found so far.
    NonLinearPlanC bestPlan;
  };


  //! userlevel=Normal
  //: Planner class.

  class NLPlannerC 
    : public RCHandleC<NLPlannerBodyC> 
  {
  public:
    NLPlannerC()
    {}
    //: Default constructor.
    
    NLPlannerC(const DListC<NLPStepC> &steps)
      : RCHandleC<NLPlannerBodyC>(*new NLPlannerBodyC(steps))
    {}
    //: Constructor.

  protected:
    NLPlannerBodyC &Body() 
    { return static_cast<NLPlannerBodyC &>(RCHandleC<NLPlannerBodyC>::Body()); }
    //: Access body.

    const NLPlannerBodyC &Body() const
    { return static_cast<const NLPlannerBodyC &>(RCHandleC<NLPlannerBodyC>::Body()); }
    //: Access body.
    
  public:
    
    DListC<NLPStepC> ListSteps(MinTermC &goal,MinTermC &full)
    { return Body().ListSteps(goal,full); }
    //: List some possible steps.
    
    DListC<NLPStepC> Apply(const StateC &state,const MinTermC &goals)
    { return Body().Apply(state,goals); }
    //: Construct plan.
  };

  //////////////////////////////////////////

  inline 
  StepListIDC NLPlannerBodyC::PreCondID(const LiteralC &symb) {
    StepListIDC &It = conds[symb];
    if(((UIntT) It) == 0)
      It = allocCondID++;
    return It;
  }

  inline 
  StepListIDC NLPlannerBodyC::PostCondID(const LiteralC &symb) {
    StepListIDC &It = conds[symb];
    if(((UIntT) It) == 0)
      It = allocCondID++;
    return It;  
  }
}

#endif
