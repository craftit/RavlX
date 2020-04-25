// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlLogicAgent
//! file="Ravl/Logic/Agent/testActiveBlackboard.cc"

#include "Ravl/Logic/ActiveBlackboard.hh"
#include "Ravl/Logic/Var.hh"
#include "Ravl/Logic/Tuple.hh"
#include "Ravl/Calls.hh"

using namespace RavlLogicN;

int testBasic();
int testRules();

int main() {
  IntT ln;
  if((ln = testBasic()) != 0) {
    cerr << "Test failed on line " << ln << "\n";
    return 1;
  }
  if((ln = testRules()) != 0) {
    cerr << "Test failed on line " << ln << "\n";
    return 1;
  }
  cerr << "Test passed. \n";
  return 0;
}

bool simpleTrigger(int &value) {
  value++;
  cerr << "simpleTigger(), Called. \n";
  return true;
}

int testBasic() {
  cerr << "testBasic(), Called \n";
  ActiveBlackboardC blackboard(true);
  
  LiteralC l1 = Literal();
  LiteralC l2 = Literal();
  TupleC t1 = TupleC(l1,l2);
  
  // Setup a trigger.
  
  LiteralC trig1 = TupleC(l1,Var());
  int value = 0;
  blackboard.AddTrigger(trig1,Trigger(&simpleTrigger,value));
  
  // Post data to the blackboard.
  
  blackboard.Tell(t1,value);
  
  return 0;
}

int testRules() {
  cerr << "testRules(), Called. \n";
  ActiveBlackboardC blackboard(true);
  LiteralC l1("l1");
  LiteralC l2("l2");
  VarC v1(true);
  blackboard.AddRule(Tuple(l1,v1),Tuple(l2,v1));
  blackboard.Tell(Tuple(l1,l1));
  if(!blackboard.Ask(Tuple(l2,l1))) return __LINE__;
  return 0;
}
