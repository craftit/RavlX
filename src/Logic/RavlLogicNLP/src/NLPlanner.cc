// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! date="4/1/1997"
//! author="Charles Galambos" 
//! lib=RavlLogicNLP
//! file="Ravl/Logic/NonLinearPlanner/NLPlanner.cc"

#include "Ravl/Logic/NLPlanner.hh"
#include "Ravl/PriQueueL.hh"
#include "Ravl/Logic/NLPAction.hh"
#include "Ravl/Logic/State.hh"
#include "Ravl/Logic/MinTerm.hh"

#include <stdio.h>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlLogicN {
  
  static DListC<NLPStepC> FindStep(MinTermC &goal,MinTermC &full,DListC<NLPStepC> &list) {
    DListC<NLPStepC> ret;
    ONDEBUG(cerr << "FindStep(), Called. Find goal:" << goal << " \n");
    BindSetC bs(true);
    for(DLIterC<NLPStepC> it(list);it;it++) {
      bs.Empty();
      MinTermC postCond = it->PostCondition();
      RavlAssert(postCond.IsValid());
      //cerr << "Comparing " << postCond << " Goal=" << goal << "\n";
      if(postCond.Covers(goal,bs)) {
	NLPStepC newstep;
	it->Substitute(bs,newstep);
	ONDEBUG(cerr << "FindStep(), Got step: " << *it << " -> " << newstep << " Binds=" << bs << "\n");
	ret.InsLast(newstep);
      }
    }
    ONDEBUG(if(ret.IsEmpty()) cerr << "No steps found to meet goal.\n");
    return ret;
  }
		  
  //////////////////////////
  // Default constructor.
  
  NLPlannerBodyC::NLPlannerBodyC() 
    : planCount(0),
      allocCondID(1),
      maxSearch(-1),
      limSearch(-1),
      curSearch(0),
      bestSize(0)
  { RavlAssert(0); }

  //: Default constructor.
  
  NLPlannerBodyC::NLPlannerBodyC(const DListC<NLPStepC> &steps) 
    : planCount(0),
      allocCondID(1),
      maxSearch(-1),
      limSearch(-1),
      curSearch(0),
      bestSize(0)
  {
    DListC<NLPStepC> xSteps(steps);
    MinTermC tmp;
    listSteps = CallFunc3C<MinTermC &,MinTermC &,DListC<NLPStepC> &,DListC<NLPStepC> >(&FindStep,tmp,tmp,xSteps);
  }
  
  ///////////////////////
  //: Construct plan.

  DListC<NLPStepC> NLPlannerBodyC::Apply(const StateC &state,const MinTermC &goals) {
    ONDEBUG(cerr << "NLPlannerBodyC::Apply(), Called. Goal=" << goals <<"\n");
    NewPlan(state,goals);
    if(!CompletePlan()) {
      ONDEBUG(cerr << "NLPlannerBodyC::Apply(), No Complete plan found. \n");
      return DListC<NLPStepC>(); // No plan found.
    }
    ONDEBUG(cerr << "NLPlannerBodyC::Apply(), Generating linear plan. \n");
    // Get linear plan.
    return GetBestLinearPlan();
  }

  ////////////////////
  // Create a new plan.
  
  bool NLPlannerBodyC::NewPlan(const StateC &start,const MinTermC &goal) {
    // Generate MinTerm reflecting all thats asserted in state,
    // this does NOT include negations.
    // Anything not assert in the initial state is assumed to be
    // false by the planner.
    // FIXME :- A better way of doing this.
    //          Make start state special ??
    planCount = 0;
    MinTermC initial(true); // Clear initial state.
    RavlAssert(initial.IsValid());
    for(LiteralIterC it(start.List());it;it++)
      initial *= it.Data();
    ONDEBUG(cerr << "NLPlannerBodyC::NewPlan(), Initial state:" << initial << " Goal=" << goal << "\n");
    
    // Create new plan.
    NonLinearPlanC plan(initial,goal,listSteps);
    
    curSearch = 0;
    limSearch = maxSearch;
    bestSize = MaxBest;
    bestPlan.Invalidate(); // No Best plan yet.
    
    //printf("Initial Step:%p \n",&(*initS));
    DoDBCheck(plan);
    ONDEBUG(cerr << "\nNLPlannerBodyC::NewPlan(), ------------ Goal:" << goal.Name() << " ---------- \n");
    ONDEBUG(cerr << "NLPlannerBodyC::NewPlan(), Start:" << initial.Name() << " \n");
    if(plan.IsComplete()) {
      // No Goal ?
      return true;
    }
    // Get item from agenda and put into the queue.
    NLPActionC act = plan.GetTopOfAgenda().GetAction(plan);
    plans.Empty();
    plans.Insert(plan.Score(),act);
    RavlAssert(plans.Size() == 1);
    
    // And we're ready !
    return true;
  }

  //////////////////////
  // Complete a plan.

  bool NLPlannerBodyC::CompletePlan() {
    ONDEBUG(cerr << "NLPlannerBodyC::CompletePlan(), Called. \n");
    NonLinearPlanC newPlan;
    bestSize = MaxBest;
    while(!plans.IsEmpty()) {
      ONDEBUG(cerr << "NLPlannerBodyC::CompletePlan(), Top plan. Score:" << plans.Top().Plan().Score() << "\n");
      if(!plans.Top().Next(newPlan)) {
	plans.DelTop();
	continue; // Out of actions.
      }
      DoDBCheck(newPlan);
      if(newPlan.IsComplete()) {
	ONDEBUG(cerr << "NLPlannerBodyC::CompletePlan(), !!!!! Complete plan found !!!!!! \n");
	IntT newSize = newPlan.Steps();
	if(newSize < bestSize) {
	  bestPlan = newPlan;
	  bestSize = newSize;
	  limSearch = (int) ((double) curSearch * 1.5);
	  break; // Done.
	}
      } else {
	// Check new plan is an actual improvement on the old one.
      
	// Put new plan back into queue.
	NLPActionC act = newPlan.GetTopOfAgenda().GetAction(newPlan);
	plans.Insert(newPlan.Score(),act);
	DoDBCheck(newPlan);
      }
      curSearch++;
      if(limSearch > 0) {
	if(curSearch >= limSearch) {
	  ONDEBUG(cerr << "NLPlannerBodyC::CompletePlan(), Search limit exceeded. \n");
	  break; // Give-up and return results.
	}
      }
    }
    if(!IsComplete() && plans.IsEmpty()) {
      ONDEBUG(cerr << "NLPlannerBodyC::CompletePlan(), plans exhausted. \n");
    }
    return IsComplete();
  }

  //////////////////////
  // Create a linear plan.

  bool NLPlannerBodyC::GetLinearPlan(DListC<NLPStepC> &lPlan) {
    if(!IsComplete()) {
      printf("NLPlannerBodyC::GetLinearPlan(), Called on incomplete plan. \n");
      RavlAssert(0);
      lPlan.Empty();
      return false;
    }
    lPlan = bestPlan.GetLinear();
    return true;
  }

  
  //: List some possible steps.
  
  DListC<NLPStepC> NLPlannerBodyC::ListSteps(MinTermC &goal,MinTermC &full) {
    DListC<NLPStepC> ret;
    RavlAssert(listSteps.IsValid());
    ONDEBUG(cerr << "NLPlannerBodyC::ListSteps(), Called for " << goal << "\n");
    for(DLIterC<NLPStepC> it(listSteps(goal,full));it;it++) {
      BindSetC bs;
      if(it->PostCondition().Covers(goal,bs)) {
	std::cerr << "Binds=" << bs << "\n";
	NLPStepC newStep;
	it->Substitute(bs,newStep);
	ret += newStep;
      }
    }
    ONDEBUG(cerr << "NLPlannerBodyC::ListSteps(), Got " << ret.Size() <<" steps. \n");
    return ret;
  }
  

  /////////////////////////////////////////////
  // Find distance between minterms.
  
  template<class DataT>
  bool Contains(const SArray1dC<DataT> &sa,const DataT &item) {
    for(SArray1dIterC<DataT> it(sa);it;it++)
      if(*it == item)
	return true;
    return false;
  }
  // 0=Identical.
  
  int NLPlannerBodyC::Distance(const MinTermC &MT1, const MinTermC &MT2) {
    int dist = 0;
    SArray1dIterC<LiteralC> It(MT1.Pos());
    for(;It.IsElm();It.Next()) {
      if(Contains(MT2.Neg(),It.Data()))
	dist += 5;
      if(!Contains(MT2.Pos(),It.Data()))
	dist += 1;
    }
    It = MT1.Neg();
    for(;It.IsElm();It.Next()) {
      if(Contains(MT2.Pos(),It.Data()))
	dist += 5;
      if(!Contains(MT2.Neg(),It.Data()))
	dist += 1;
    }
    It = MT2.Pos();
    for(;It.IsElm();It.Next()) {
      if(!Contains(MT1.Pos(),It.Data()))
	dist += 1;
    }
    It = MT2.Neg();
    for(;It.IsElm();It.Next()) {
      if(!Contains(MT1.Neg(),It.Data()))
	dist += 1;
    }
    return dist;
  }
  
  ///////////////////////////
  // Debuging checks.

  void NLPlannerBodyC::DoDBCheck(NonLinearPlanBodyC &/*APlan*/) {
#if 0
    BindSetC BTmp;
    if(APlan.Plan.NodeData(startNode.ID()).PostCond().Unify(*Init,BTmp) 
       && (&(*InitS) == &(*APlan.Plan.NodeData(startNode.ID()))))
      return ; // Alls ok.
    printf("NLPlannerBodyC::DoDBCheck(), FAILED. Step:%p (%d) InitS:%p \n",&(*APlan.Plan.NodeData(startNode.ID())),startNode.ID().V(),&(*InitS));
    APlan.Dump();
    printf("Post should be:'%s' \n",init.GetName().chars());
    RavlAssert(0);
#endif
  }


}
