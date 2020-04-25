// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////
//! rcsid="$Id$"
//! date="1/9/1997"
//! author="Charles Galambos"
//! lib=RavlLogicNLP
//! file="Ravl/Logic/NonLinearPlanner/exNonlinearPlanner.cc"
//! docentry="Ravl.API.Logic.Planning"
//! userlevel=Normal

//: Example program for non-linear planner.

#include "Ravl/Logic/NonLinearPlan.hh"
#include "Ravl/Logic/NLPlanner.hh"
#include "Ravl/Logic/Not.hh"
#include "Ravl/Logic/And.hh"
#include "Ravl/Logic/Or.hh"
#include "Ravl/Logic/Var.hh"
#include "Ravl/Logic/State.hh"
#include "Ravl/Option.hh"

int SimpleTest();
int LoopTest();

using namespace RavlLogicN;
using namespace RavlN;

int main(int argc,char **argv) 
{
  OptionC  option(argc,argv,true);
  option.Check();
  
  LiteralC goals;
  LiteralC startCond;
  
  // Setup some named literals.
  
  LiteralC see("See");
  LiteralC inv("Inv");
  LiteralC ball("Ball");
  LiteralC bat("Bat");
  LiteralC igloo("Igloo");
  
  VarC var1("var1");
  
  // A start condition.
  
  startCond = TupleC(see,ball) * TupleC(inv,bat) * TupleC(see,igloo); 
  
  // A goal condition.
  
  goals = TupleC(inv,ball) * !TupleC(inv,bat) * TupleC(see,bat) *  !TupleC(see,igloo) * !TupleC(inv,igloo);

  
  // Now for some actions...
  
  
  DListC<NLPStepC> rules;
  
  rules.InsLast(NLPStepC(TupleC(LiteralC("Remove"),var1),
			 TupleC(see,var1),
			 !TupleC(see,var1)
			 )
		);
  rules.InsLast(NLPStepC(TupleC(LiteralC("get"),var1),
			 TupleC(see,var1),
			 TupleC(inv,var1) *!TupleC(see,var1)
			 )
		);
  
  rules.InsLast(NLPStepC(TupleC(LiteralC("put"),var1),
			 TupleC(inv,var1),
			 TupleC(see,var1)*!TupleC(inv,var1)
			 )
		);
  
  // Construct a planner with a rule set.
  
  NLPlannerC planner(rules);
  
  StateC init(true);
  init.Tell(startCond);
  
  // Run the planner.
  
  DListC<NLPStepC> plan = planner.Apply(init,MinTermC(goals,true));
  
  // Print a list of results.
  
  if(!plan.IsValid()) {
    std::cerr << "Unable to complete plan. \n";
    return 1;
  }
  std::cerr << "Plan=" << plan << "\n";
  return 0;
}

