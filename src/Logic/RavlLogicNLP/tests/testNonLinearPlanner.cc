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
//! file="Ravl/Logic/NonLinearPlanner/testNonLinearPlanner.cc"
//! userlevel=Develop
//! docentry="Ravl.API.Logic.Planning"

#include <stdio.h>
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
  //BooleanT verbose  = option.Boolean("v",FALSE,"Verbose mode.");
  option.Check();
  int ret;
#if 1
  if((ret = SimpleTest()) != 0) {
    cerr << "Test failed. :" << ret << "\n";
    return 1;
  }
#endif
#if 1
  if((ret = LoopTest()) != 0) {
    cerr << "Test failed. :" << ret << "\n";
    return 1;
  }
#endif
  cout << "Test passed. \n";
  return 0;
}

int SimpleTest()
{
  LiteralC goals;
  LiteralC startCond;
  
  LiteralC see("See");
  LiteralC inv("Inv");
  LiteralC ball("Ball");
  LiteralC bat("Bat");
  LiteralC igloo("Igloo");
  startCond = TupleC(see,ball) * TupleC(inv,bat) * TupleC(see,igloo); 
  cerr << "Start=" << startCond << "\n"; 
  cerr << "Setting up goals.  \n";
  goals = TupleC(inv,ball) * !TupleC(inv,bat) * TupleC(see,bat) * 
    !TupleC(see,igloo) * !TupleC(inv,igloo);
  cerr << "Goal=" << goals << "\n";
  cerr << "Setting up rules.  \n";
  VarC var1("var1");
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
  
  //theory.Dump();
  cerr << "Constructing planner.  \n";
  
  NLPlannerC planner(rules);
  StateC init(true);
  init.Tell(startCond);
  DListC<NLPStepC> plan = planner.Apply(init,MinTermC(goals,true));
  if(!plan.IsValid()) {
    cerr << "Unable to complete plan. \n";
    return 1;
  }
  cerr << "Plan=" << plan << "\n";
  return 0;
}

#if 1
int LoopTest() 
{
  LiteralC goals;
  LiteralC startCond;
  
  cerr << "Setting up start condition for LoopTest  \n";
  LiteralC see("see");
  LiteralC bat("bat ");
  LiteralC inv("inv");
  LiteralC ball("ball");
  
  startCond = TupleC(see,bat);
  cerr << "Setting up goals.  \n";
  goals = TupleC(inv,ball);
  
  cerr << "Setting up rules.  \n";
  VarC var1("var1");
  DListC<NLPStepC> rules;
  rules.InsLast(NLPStepC(TupleC(LiteralC("get"),var1),
			 TupleC(see,var1),
			 TupleC(inv,var1) *!TupleC(see,var1)
			 )
		);
  
  rules.InsLast(NLPStepC(TupleC(LiteralC("put"),var1),
			 TupleC(inv,var1),
			 TupleC(see,var1)*!TupleC(inv,var1))
		);
  
  //theory.Dump();
  cerr << "Constructing planner.  \n";
  
  NLPlannerC planner(rules);
  
  StateC init(true);
  init.Tell(startCond);
  DListC<NLPStepC> plan = planner.Apply(init,MinTermC(goals,true));
  return 0;
}
#endif
