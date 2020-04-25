// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////
//! date="12/12/1996"
//! author="Charles Galambos"
//! rcsid="$Id$"
//! lib=RavlLogicNLP
//! file="Ravl/Logic/NonLinearPlanner/NonLinearPlan.cc"

#include "Ravl/Logic/BMinTermListIndex.hh"

#include "Ravl/Assert.hh"
#include "Ravl/BGraphLinearIter.hh"
#include "Ravl/Logic/MinTermIter.hh"
#include "Ravl/Logic/NonLinearPlan.hh"
#include "Ravl/Logic/NLPAgenda.hh"
#include "Ravl/Logic/BMinTermIndexIter.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x) 
#endif


namespace RavlLogicN { 
  
  //////////////////////////
  // Constructor, Empty plan.
  
  NonLinearPlanBodyC::NonLinearPlanBodyC(const MinTermC &initCond,
					 const MinTermC &goalCond,
					 const CallFunc2C<MinTermC&,MinTermC&,DListC<NLPStepC> > &step) 
    : bnds(true),
      steps(0),
      listSteps(step)
  {
    ONDEBUG(cerr << "NonLinearPlanBodyC::NonLinearPlanBodyC(), Called. Inital=" << initCond.Name() << " Goal=" << goalCond.Name() << "\n");
    MinTermC NA_MT(true);
    LiteralC NA_Act("NA_Act");
    startNode = InsStep(NLPStepC(NA_Act,NA_MT,initCond));
    goalNode  = InsStep(NLPStepC(NA_Act,goalCond,NA_MT));
    InsOrderLink(startNode,goalNode);
  }
  
  ////////////////////////
  // Destructor.

  NonLinearPlanBodyC::~NonLinearPlanBodyC() {
    ONDEBUG(std::cerr << "NonLinearPlanBodyC::~NonLinearPlanBodyC(), Called.\n");
    agenda.Empty();   // Only really needed for debugging.
  }
  
  ///////////////////////////
  // Insert a step into the plan
  
  NLPStepNodeT NonLinearPlanBodyC::InsStep(const NLPStepC &step,const MinTermC &goal) {
    // Duplicate step and its variables, then insert it into graph.
    HashC<LiteralC,LiteralC> newBnds;
    NLPStepC newStep(step.ReplaceVars(newBnds));
    BindSetC bnds(newBnds);
    if(!newStep.PreCondition().Unify(goal,bnds))
      return NLPStepNodeT(); // Unify failed.
    return InsStep(newStep);
  }
  
  ///////////////////////////
  // Insert a step into the plan.

  NLPStepNodeT NonLinearPlanBodyC::InsStep(const NLPStepC &step) {
    //RavlAssert(IsNotConst());
    //VLOCKOBJC(step);
    NLPStepNodeT CurNode(plan,plan.InsNode(step));
    RavlAssert(IsValid(CurNode));
    steps++;
    ONDEBUG(std::cerr << "NonLinearPlanBodyC::InsStep() " << PlanID() << " Inserting step '" << step.Name() << "' (" << CurNode.Hash() << ") (NoSteps:" << steps <<") \n");
    // Update database of post/pre conditions.
    for(MinTermIterC it(step.PreCondition());it;it++) {
      ONDEBUG(std::cerr << "Adding open condition '" << *it << "'\n");
      if(!PostOpenCond(CurNode,MinTermC(it.Data(),it.IsNegated()))) {
	// This occurs if there's no way to resolve an Open Condition created
	// by new "Step".  There's no point in considering this plan further.
	return NLPStepNodeT();
      }
      PreConds().Insert(it.Data(),CurNode);
#if VDEBUG && 0
      // Check precondition list is valid (DEBUG)
      for(BListIterC<NLPStepNodeT> PCIt(PreConds(it.Data()));PCIt.IsElm();PCIt.Next())
	RavlAssert(IsValid(PCIt.Data()));
#endif
      //printf("  %s PreConds: %d \n",It.Data().Name().chars(),PreConds(It.Data()).Size());
    }
    
    for(MinTermIterC itP(step.PostCondition());itP.IsElm();itP.Next()) {
      // Look for threats. 
      //    Look for preconditions that conflict with new step's post conditions.
      MinTermC Inv(itP.Data(),itP.IsNegated());
      for(BMinTermIndexIterC<BListC<NLPStepNodeT> > mit(PreConds(),Inv);mit;mit++) {
	// FIXME :- Make this hanle variables etc...
	for(BListIterC<NLPStepNodeT> it(mit.Data());it.IsElm();it.Next()) {
	  // Look for Causal links that could be threatened.
	  ONDEBUG(cerr << "  PreConds : " << it.Data().Data().Name() << " \n");
	  for(NLPAdjLinkT Adj(it.Data().In(plan));Adj.IsElm();Adj.Next()) {
	    if(!Adj.Data().IsStepOrder()) {
	      ONDEBUG(cerr << "    Step : " << Adj.Data().Name() << " \n");
	      PostThreat(CurNode,Adj.Edge());
	    }
	  }
	}
      }
      PostConds().Insert(itP.Data(),CurNode);
    }
    ONDEBUG(cerr << "\n");
    return CurNode;
  }

  ///////////////////////////
  // Insert a causal link into the plan.
  //  GoalCond must be a condition in the TARGET of the
  // causal link.
  
  NLPCausalLinkT NonLinearPlanBodyC::InsCausalLink(NLPStepNodeT from,
						   NLPStepNodeT to,
						   MinTermC &goalCond) {
    //RavlAssert(IsNotConst());
    if(from == GoalNode())
      return NLPCausalLinkT(); // Can't have steps after goal.
    if(to == StartNode())
      return NLPCausalLinkT(); // Can't have steps before start.
    
#if 0
    const MinTermC &stepPreC = to.Data().PreCondition();
    if(!stepPreC.Unify(goalCond,bnds)) {
      ONDEBUG(cerr << "NonLinearPlanBodyC::InsCausalLink(), Failed to unify '" << stepPreC.Name() << "' and '" << goalCond.Name() << "' Bnds:'" << bnds.Name() << "' \n");
      return NLPCausalLinkT(); // Binding failed.
    }
#endif    
    NLPCausalLinkT CL = plan.InsEdge(from,to,NLPCausalLinkC(goalCond));
    if(!CL.IsValid())
      return CL;
    
    ONDEBUG(cerr << "NonLinearPlan::InsCasualLink() " << PlanID() << "  CL:" << CL.Hash() << "  " << from.Hash() << "->" << to.Hash() << " Cond:" << goalCond.Name() << ". \n");
    
    // Check for possible threats from other nodes.
    
    for(MinTermIterC itP(goalCond);itP.IsElm();itP.Next()) {
    // Look for threats. 
    // Look for preconditions that conflict with post conditions of 
      // new step.
      MinTermC Inv(itP.Data(),itP.IsNegated());
      for(BMinTermIndexIterC<BListC<NLPStepNodeT> > mit(PostConds(),Inv);mit;mit++) {
	for(BListIterC<NLPStepNodeT> It(mit.Data());It;It++) {
	  ONDEBUG(cerr << "  PostConds : " << It.Data().Data().Name() << "\n");
	  if(plan.TryEdge(It.Data().ID(),CL.Target())) { // Must be able to be placed before target.
	    if(plan.TryEdge(CL.Source(),It.Data().ID())) // Must be able to be placed after source.
	      PostThreat(It.Data(),CL);
	  }
	}
      }
    }
    ONDEBUG(cerr << "\n");
    return CL;
  }
  
  ///////////////////////////
  // Insert an ordering constriant into the plan.
  
  NLPOrderLinkT NonLinearPlanBodyC::InsOrderLink(NLPStepNodeT From,
						 NLPStepNodeT To) {
    //RavlAssert(IsNotConst());
    if(From == GoalNode())
      return NLPOrderLinkT(); // Can't have steps after goal.
    if(To == StartNode())
      return NLPOrderLinkT(); // Can't have steps before start.
    RavlAssert(!(From == To));
    NLPOrderLinkT Ret = plan.InsEdge(From,To,NLPCausalLinkC());
    if(Ret.IsValid()) {
      ONDEBUG(cerr << "NonLinearPlanBodyC::InsOrderLink() " << PlanID() << "  OL:" << Ret.Hash() << "   " << From.Hash() << "->" << To.Hash() << "  \n");
    }
    return Ret;
  }
  
  //: Check the addition of a new post-condition would be usefull.

  // This is a fairly simple check for loops of steps which are
  // identical that achieve nothing.  If it finds a linear chain
  // of steps with no side effects (Which may be usefull), it which
  // a step identical to the new one exists it tells the planner to discard
  // the new step.
  
  bool NonLinearPlanBodyC::IsUsefullNewStep(const NLPStepC &step,const NLPStepNodeT &ss) const {
    NLPStepNodeT at(ss);
    ONDEBUG(cerr << "NonLinearPlanBodyC::IsUsefullNewStep(), Checking:" << step.Name() << " \n");
    while(at.IsValid()) {
      ONDEBUG(cerr << "NonLinearPlanBodyC::IsUsefullNewStep()," << at.Data().Name() << " \n");
      if(at.Data().PostCondition() == step.PostCondition() &&
	 at.Data().PreCondition() == step.PreCondition()) {
	ONDEBUG(cerr << "NonLinearPlanBodyC::IsUsefullNewStep(), Is useless! \n");
	return false; // Exact duplicate found in history.
      }
      NLPAdjLinkT place(at.Out(const_cast<BDAGraphC<NLPStepC,NLPCausalLinkC>&>(plan)));
      at.Invalidate();
      for(;place.IsElm();place.Next()) {
	if(place.Data().IsStepOrder())
	  continue; // Ignore order links.
	if(at.IsValid()) { // Have we got a causal link already ?
	  ONDEBUG(cerr << "NonLinearPlanBodyC::IsUsefullNewStep(), Multipal causal links \n");
	  return false; // More than one cause, can't tell.
	}
	at = const_cast<BDAGraphC<NLPStepC,NLPCausalLinkC>&>(plan).NodeObjH(place.FarNodeH());
      }
    }
    ONDEBUG(cerr << "NonLinearPlanBodyC::IsUsefullNewStep(), Doesn't duplicate. \n");
    return true;
  }

  ///////////////////////////
  // Insert an open condition into the agenda.

  bool NonLinearPlanBodyC::PostOpenCond(NLPStepNodeT &aStep,
					const MinTermC &openCond) {
    ONDEBUG(cerr << "NonLinearPlanBodyC::PostOpenCond() " << PlanID() << "  AGENDA+ Cond " << openCond.Name() << " for step " << aStep.Hash() << " \n");
    agenda.InsFirst(NLPAgendaOpenGoalC(aStep,openCond,*this));
    // FIXME :- Could check if Condition is resolvable ??
    //          if not return false.
    return true;
  }

  ///////////////////////////
  // Insert an threat into the agenda.

  bool NonLinearPlanBodyC::PostThreat(NLPStepNodeT &aStep,
				      NLPCausalLinkT aLink) {
    ONDEBUG(cerr << "NonLinearPlanBodyC::PostThreat() " << PlanID() << "  AGENDA+ Step:" << aStep.Data().Name() << "  -> " << (int) aLink.Source().V() << "-" << aLink.Data().Name() << "->" << (int) aLink.Target().V() << "  \n");
    if(aStep == aLink.Target())
      return false; // A node can't threaten itself.
    agenda.InsFirst(NLPAgendaThreatC(aStep,aLink));
    return true;
  }
  
  ////////////////////////////
  // Get a linear version of the plan.
  DListC<NLPStepC> NonLinearPlanBodyC::GetLinear(void) const {
    DListC<NLPStepC> ret;
#if 1
    BGraphLinearIterC<NLPStepC,NLPCausalLinkC> it(const_cast<BDAGraphC<NLPStepC,NLPCausalLinkC>&>(plan));
    IndexC StartNodeID = StartNode().ID();
    IndexC GoalNodeID = GoalNode().ID();
    for(;it.IsElm();it.Next()) {
      if(it.NodeH() == StartNodeID)
	continue; // Ignore Start.
      if(it.NodeH() == GoalNodeID) {
	//ret.SetGoalCondition(It.Data().PreCondition());
	continue; 
      }
      ret.InsLast(it.Data());
    }
    //printf("NonLinearPlanBodyC::GetLinear() %d, Plan dump: \n",PlanID());
    //Plan.DebugPrint(cout);
    //printf("NonLinearPlanBodyC::GetLinear(), Dump finished. \n");
    Dump();
#endif
    return ret;
  }
  
  void NonLinearPlanBodyC::DoDBCheck() { 
    // planner.DoDBCheck(*this); 
  }

  ////////////////////////////
  // Dump current plan to stdout.
  
  void NonLinearPlanBodyC::Dump() const {
    cerr << "NonLinearPlanBodyC::Dump() : \n";
    BGraphLinearIterC<NLPStepC,NLPCausalLinkC> it(const_cast<BDAGraphC<NLPStepC,NLPCausalLinkC> &>(plan));
    for(;it.IsElm();it.Next()) {
      cerr << "Step " << it.Node().Hash() << ":" << it.Data().Name() << " \n";
      for(NLPAdjLinkT Adj(it.Node().Out(const_cast<BDAGraphC<NLPStepC,NLPCausalLinkC>&>(plan)))
	    ;Adj.IsElm();Adj.Next())
	cerr << "  " << Adj.Data().Name() << " -> " << Adj.FarNodeH().V() <<" \n";
    }
  }
}

