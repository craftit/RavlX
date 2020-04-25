// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlLogicNLP
//! date="18/3/1997"
//! author="Charles Galambos"
//! file="Ravl/Logic/NonLinearPlanner/NLPAction.cc"

#include "Ravl/Assert.hh"
#include "Ravl/Collection.hh"
#include "Ravl/Logic/NLPAction.hh"
#include "Ravl/Logic/BMinTermIndexIter.hh"
#include <stdlib.h>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlLogicN {
  
  //////////////////////////////////
  // Construct an action on a plan to address agenda item.

  NLPActionBodyC::NLPActionBodyC(const NonLinearPlanC &APlan,const NLPAgendaItemC &/*Item*/)
    : plan(APlan)
  {
    //VLOCKOBJC(APlan);
  }
  
  //: Get agenda item being addressed.
  
  NLPAgendaItemC NLPActionBodyC::AgendaItem() {
    RavlAssertMsg(0,"NLPActionBodyC::AgendaItem(), Abstract method called.");
    return NLPAgendaItemC();
  }
  
  //////////////////////////////////
  // Get next solution.

  bool NLPActionBodyC::Next(NonLinearPlanC &/*aplan*/)  {
    RavlAssertMsg(0,"NLPActionBodyC::Next(), Abstract method called.");
    return false;
  }
  
  ///////////////////////////////////////////////////////////////////////////////////
  // Construct action.

  NLPActionOpenGoalBodyC::NLPActionOpenGoalBodyC(const NonLinearPlanC &plan,const NLPAgendaOpenGoalC &nOG)
    : NLPActionBodyC(plan,nOG),
      openGoal(nOG)
  {
    newSteps = openGoal.NewSteps();
    BuildPossSteps(Plan());
  }
  
  ///////////////////////////
  // Build list of exiting steps in plan that may help.
  
  void NLPActionOpenGoalBodyC::BuildPossSteps(NonLinearPlanC &NLPlan) {
    MinTermC &cnd = OpenCond();
    DListC<NLPStepNodeT> someSteps;
    ONDEBUG(cerr << "NLPActionOpenGoal::BuildPossSteps() Cnd:'" << cnd.Name() <<"' \n");
    
    CollectionC<NLPStepNodeT> rBag(128);
    
    for(BMinTermIndexIterC<BListC<NLPStepNodeT> > it(NLPlan.PostConds(),MinTermC(cnd));it;it++) {
      for(BListIterC<NLPStepNodeT > sIt(it.Data());sIt;sIt++) {
	RavlAssert(NLPlan.IsValid(sIt.Data()));
	ONDEBUG(cerr << "NLPActionOpenGoal::BuildPossSteps() Found ('" << sIt.Data().Data().PostCondition().Name() << "') \n");
	rBag.Insert(sIt.Data());
      }
    }
    
    // The precondition for the plan only lists asserted symbols,
    // so if we're looking for a negated condition we just
    // have to check for a contridiction with it. 
    // Positive conditions should already be in the PostConds() index.
    
    if(cnd.Pos().IsEmpty()) { // Looking for a negated condition ?
      const MinTermC &theStart = NLPlan.StartNode().Data().PostCondition();
      RavlAssert(cnd.Neg().Size() == 1); // Check our assumtion.
      LiteralC target = cnd.Neg()[0]; // We only look at one condition at a time...
      BindSetC bnds;
      ONDEBUG(cerr << "NLPActionOpenGoal::BuildPossSteps(), Unify '" << theStart.Name() << "' and '" << cnd.Name() << "' \n");
      bool fnd = false;
      for(SArray1dIterC<LiteralC> it(theStart.Pos());it;it++) {
	if(target.Unify(it.Data(),bnds)) {
	  fnd = true;
	  break;
	}
      }
      if(!fnd) { // If there's no condradition, then add.
	RavlAssert(NLPlan.IsValid(NLPlan.StartNode()));
	ONDEBUG(cerr << "NLPActionOpenGoal::BuildPossSteps() Inserting plan precondition. ('" << theStart.Name() << "') \n");
	rBag.Insert(NLPlan.StartNode());
      }
    }
    // Make a randomised list.
    while(!rBag.IsEmpty())
      someSteps.InsLast(rBag.Pick()); // Pick randomly from bag and put it in list.
    possSteps = someSteps;
  }

  ///////////////////////////
  // Get next solution.

  bool NLPActionOpenGoalBodyC::Next(NonLinearPlanC &newPlan) {
    NLPCausalLinkT CLink;
    NLPStepNodeT NStep;
    Plan().DoDBCheck();
    newPlan = Plan().Copy(); // Create a new plan to modify.
    
    ONDEBUG(cerr << "NLPActionOpenGoal::Next() " << newPlan.PlanID() << " from " << Plan().PlanID() << "  Step(" << Step().Hash() << "):" << Step().Data().Name() << " OpenCond:" << OpenCond().Name() <<  "\n");
    bool CanAddSteps = true; //NLPlan.GetSteps() < NLPlan.GetStepLimit();
    // Is it poassible to add steps to current plan ?
    
    // Try something else.
    for(bool Done = false;!Done;) {
      if(!possSteps && (!newSteps || !CanAddSteps)) {
	ONDEBUG(cerr << "NLPActionOpenGoal::Next() " << newPlan.PlanID() << "  Failed. \n");
	newPlan.Invalidate(); // Delete it.
	Plan().DoDBCheck();
	return false; // Failed, nothing can be done.
      }
      if((rand() % 10 != 0 && possSteps) 
	 || !newSteps
	 || !CanAddSteps) { // Random for the moment.
	// Causal link...
	RavlAssert(possSteps.IsElm());
	if((CLink = newPlan.InsCausalLink(possSteps.Data(),
					  Step(),
					  OpenCond())).IsValid()) {
	  //VLOCKOBJ(*newPlan);
	  Done = true;
	}
	newPlan.DoDBCheck();
	possSteps.Next();
      } else {
	RavlAssert(newSteps);
	// Before we add steps, check that the new step makes
	// sense. It must add at least one new postcondition
	// that has a different state (In terms of dependancies
	// than one that already exists.) Otherwise its pointless,
	// and may lead to a spinner planner.
	if(!newPlan.IsUsefullNewStep(newSteps.Data(),openGoal.Step())) {
	  newSteps.Next();
	  continue; // Step doesn't add anything new so try something else.
	}
	// Attempt to add new step into plan.
	NStep = newPlan.InsStep(newSteps.Data());
	newSteps.Next();
	if(!NStep.IsValid())
	  continue;
	CLink = newPlan.InsCausalLink(NStep,Step(),OpenCond());
	RavlAssert(CLink.IsValid());
	//VLOCKOBJ(*newPlan);
	Done = true;
      }
    }
    newPlan.DoDBCheck();
    Plan().DoDBCheck();
    return true;
  }

  ///////////////////////////////////////////////////////////////////////////////////
  // Construct action.

  NLPActionThreatBodyC::NLPActionThreatBodyC(const NonLinearPlanC &Plan,const NLPAgendaThreatC &Thr) 
    : NLPActionBodyC(Plan,Thr),
      threat(Thr),
      donePre(false),
      donePost(false)
  {}
  
  ///////////////////////////
  // Get next solution.
  
  bool NLPActionThreatBodyC::Next(NonLinearPlanC &newPlan) {
    NLPCausalLinkT CLink;
    Plan().DoDBCheck();
    newPlan = Plan().Copy(); // Create a new plan to modify.
  
    for(bool Done = false;!Done;) {
      if(donePre && donePost) {
	ONDEBUG(cerr << "NLPActionThreat::Next(), Failed. \n");
	newPlan.Invalidate(); // Don't need it anymore.
	Plan().DoDBCheck();
	return false;
      }
      if(rand() & 1 || donePost ) {
	// Force threat before condition is established.
	if(donePre)
	  continue; // FIXME :- Sort it out !
	donePre = true;
	if((CLink = newPlan.InsOrderLink(Step(),StepPre())).IsValid()) {
	  ONDEBUG(cerr << "NLPActionThreat::Next(), Called.\n  Force Before. " << Step().Data().Name() << " -> " << StepPre().Data().Name() << "  \n");
	  Done = true;
	  //VLOCKOBJ(*newPlan);
	}
      } else {
	// Force threat after condition is used.
	donePost = true;
	if((CLink = newPlan.InsOrderLink(StepPost(),Step())).IsValid()) {
	  ONDEBUG(cerr << "NLPActionThreat::Next(), Called.\n  Force After. " << StepPost().Data().Name() << " -> " << Step().Data().Name() << " \n");
	  Done = true;
	  //VLOCKOBJ(*newPlan);
	}
      }
    }
    newPlan.DoDBCheck();
    Plan().DoDBCheck();
    return true; // Something worked !
  }
  
}
