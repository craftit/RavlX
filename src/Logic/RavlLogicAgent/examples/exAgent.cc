// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! author="Charles Galambos"
//! docentry="Ravl.API.Logic.Agent"
//! rcsid="$Id$"
//! lib=RavlLogicAgent
//! file="Ravl/Logic/Agent/exAgent.cc"

#include "Ravl/Option.hh"
#include "Ravl/Logic/ActiveBlackboard.hh"

using namespace RavlLogicN;
using namespace RavlN;

int main(int nargs,char **argv) {
  OptionC opt(nargs,argv);
  StringC file = opt.String("","rules.rl","Input file. ");
  bool dump = opt.Boolean("d",false,"Dump blackboard. ");
  opt.Check();
  
  ActiveBlackboardC abb(true);
  if(!abb.LoadRules(file)) {
    cerr << "Failed to load '" << file << "'\n";
    return 1;
  }
  if(dump) {
    cout << "State:\n";
    abb.Dump(cout);
  }
  return 0;
}
