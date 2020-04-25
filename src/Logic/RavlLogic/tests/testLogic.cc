// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlLogic
//! file="Ravl/Logic/Base/testLogic.cc"
//! docentry="Ravl.API.Logic"
//! author="Charles Galambos"
//! userlevel=Develop

#include "Ravl/Logic/Tuple.hh"
#include "Ravl/Logic/Unify.hh"
#include "Ravl/Logic/Var.hh"
#include "Ravl/Logic/StateSet.hh"
#include "Ravl/Logic/Or.hh"
#include "Ravl/Logic/And.hh"
#include "Ravl/Logic/Not.hh"
#include "Ravl/Logic/LiteralIO.hh"
#include "Ravl/Logic/Value.hh"
#include "Ravl/Logic/Context.hh"
#include "Ravl/StrStream.hh"
#include "Ravl/BinStream.hh"

using namespace RavlLogicN;

#define DODEBUG	0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

int testBasic();
int testCompose();
int testBind();
int testStateSet();
int testStateOr();
int testStateAnd();
int testStateNot();
int testLiteralIO();
int testBinaryIO();

int main() {
  int err;
#if 1
  if((err = testBasic()) != 0) {
    std::cerr << "testBasic failed at : " << err << "\n";
    return 1;
  }
  if((err = testBind()) != 0) {
    std::cerr << "testBind failed at : " << err << "\n";
    return 1;
  }
  if((err = testCompose()) != 0) {
    std::cerr << "testBasic failed at : " << err << "\n";
    return 1;
  }
  if((err = testStateSet()) != 0) {
    std::cerr << "testStateSet failed at : " << err << "\n";
    return 1;
  }
  if((err = testStateOr()) != 0) {
    std::cerr << "testStateOr failed at : " << err << "\n";
    return 1;
  }
  if((err = testStateAnd()) != 0) {
    std::cerr << "testStateAnd failed at : " << err << "\n";
    return 1;
  }
  if((err = testStateNot()) != 0) {
    std::cerr << "testStateAnd failed at : " << err << "\n";
    return 1;
  }
  if((err = testLiteralIO()) != 0) {
    std::cerr << "testLiteralIO failed at : " << err << "\n";
    return 1;
  }
#endif
  if((err = testBinaryIO()) != 0) {
    std::cerr << "testBinaryIO failed at : " << err << "\n";
    return 1;
  }
  std::cerr << "Test passed. \n";
  return 0;
}

//: Test basic literal operations and unification.

int testBasic() {
  std::cout << "testBasic() Started. \n";  
  
  if(!Literal().IsValid()) return __LINE__;
  if(!Var().IsValid()) return __LINE__;
  
  LiteralC s1(Literal());
  LiteralC s2(Literal());
  if(s1 == s2) return __LINE__;
  if(s1 != s1) return __LINE__;
  VarC v1(true);
  if(s1 == v1) return __LINE__;
  LiteralC test;
  
  LiteralC nl1("Hello");
  if(nl1 != LiteralC("Hello")) return __LINE__;
  
  // Check unification of plain literals.
  BindSetC bs1(true);
  if(!Unify(v1,s1,bs1)) return __LINE__;
  if(!bs1.IsBound(v1)) return __LINE__;
  if(!bs1.Resolve(v1,test)) return __LINE__;
  if(test != s1) return __LINE__;
  
  bs1.Empty();
  if(!Unify(s1,v1,bs1)) return __LINE__;
  if(!bs1.IsBound(v1)) return __LINE__;
  if(!bs1.Resolve(v1,test)) return __LINE__;
  if(test != s1) return __LINE__;
  
  // Check the most basic unify operation.
  TupleC e1 = Tuple(s1,s2);
  //e1.Dump(std::cerr);   std::cerr <<"\n";
  TupleC e2 = Tuple(s1,s2);
  if(e1.Hash() != e2.Hash()) return __LINE__;
  if(e1 != e2) return __LINE__;
  BindSetC bs(true);
  if(!Unify(e1,e2,bs)) return __LINE__;
  TupleC e3 = Tuple(s1,Var());
  //e3.Dump(std::cerr); std::cerr << "\n";
  if(!Unify(e1,e3,bs)) return __LINE__;
  if(bs.Size() != 1) return __LINE__;

  return 0;
}

int testCompose() {
  std::cerr << "testCompose() Started. \n";
  // Check and...
  AndC land(true);
  LiteralC l1("l1");;
  LiteralC l2("l2");
  LiteralC l3("l3");
  if(land.Arity() != 1) return __LINE__;
  land *= l1;
  if(land.Arity() != 2) return __LINE__;
  if(land.Terms()[1] != l1) return __LINE__;
  land *= l2;
  if(land.Arity() != 3) return __LINE__;  
  if(land.Terms()[1] != l1) return __LINE__;
  if(land.Terms()[2] != l2) return __LINE__;
  std::cerr << "And=" << land << "\n";
  // Check minterm.
  MinTermC mt((const LiteralC &)land,false);
  std::cerr << "MinTerm=" << mt << "\n";
  NotC nt(true,l3);
  mt *= nt;
  std::cerr << "MinTerm=" << mt << "\n";
  
  LiteralC goalSymb = l1 * !l2 * !l3;
  std::cerr << "Term=" << goalSymb << "\n";
  return 0;
}

// Test BindSetC.

int testBind() {
  std::cout << "testBind() Started. \n";  
  BindSetC test(true);
  if(!test.IsValid()) return __LINE__;
  VarC v1(true);
  VarC v2(true);
  VarC v3(true);
  LiteralC l1(true);
  LiteralC l2(true);
  BindMarkT bm1 = test.Mark();  
  if(!test.Bind(v1,l1)) return __LINE__;
  if(test.Bind(v1,l2)) return __LINE__;
  if(!test.Bind(v2,l2)) return __LINE__;  
  BindMarkT bm2 = test.Mark(); 
  if(!test.Bind(v3,l1)) return __LINE__;
  if(test[v1] != l1) return __LINE__;
  if(test[v2] != l2) return __LINE__;
  if(test[v3] != l1) return __LINE__;
  if(test.Size() != 3) return __LINE__;
  test.Undo(bm2);
  if(test.Size() != 2) return __LINE__;
  if(!test.IsBound(v1)) return __LINE__;
  if(!test.IsBound(v2)) return __LINE__;
  if(test.IsBound(v3)) return __LINE__;
  test.Undo(bm1);
  if(test.Size() != 0) return __LINE__;
  if(test.IsBound(v1)) return __LINE__;
  if(test.IsBound(v2)) return __LINE__;
  if(test.IsBound(v3)) return __LINE__;
  return 0;
}


// Test BindSetC.

int testStateSet() {
  std::cout << "testStateSet() Started. \n";
  StateSetC state(true);
  if(!state.IsValid()) return __LINE__;
  LiteralC l1(true);
  LiteralC l2(true);
  if(state.Ask(l1)) return __LINE__;
  if(!state.Tell(l1)) return __LINE__;
  if(state.Ask(l2)) return __LINE__;
  if(!state.Tell(l2)) return __LINE__;
  if(!state.Ask(l1)) return __LINE__;
  if(!state.Ask(l2)) return __LINE__;
  // Check filter on unbound var.
  VarC v1(true);
  BindSetC binds(true);
  LiteralIterC it = state.Filter(v1,binds);
  int count = 0;
  for(;it;it++) {
    if(!binds.IsBound(v1)) return __LINE__;
    count++;
  }
  if(count != 2) return __LINE__;
  // Check filter on bound var.
  count = 0;
  binds.Bind(v1,l1);
  it = state.Filter(v1,binds);
  for(;it;it++) {
    count++;
    if(it.Data() != l1) return __LINE__;
  }
  if(count != 1) return __LINE__;
  // Dump state.
  std::cout << "Test dump of state: \n";
  std::cout << state;
  return 0;
}


int testStateOr() {
  std::cout << "testStateOr() Started. \n";
  //cerr << "testStateOr() \n";
  StateSetC state(true);
  LiteralC l1(true);
  LiteralC l2(true);
  LiteralC l3(true);
  VarC v1(true);
  
  TupleC t1(l1,l2);
  TupleC t2(l2,l3);
  TupleC t3(l2,l1);
  state.Tell(t1);
  state.Tell(t2);
  state.Tell(t3);
  
  LiteralC orTest(Tuple(v1,l3) + Tuple(l1,v1));
  //cerr << "testStateOr() Creating iter.\n";
  BindSetC binds(true);
  int count = 0;
  for(LiteralIterC it(orTest.Solutions(state,binds));it;it++) {
    if(it.Data() == t1) {
      if(!binds.IsBound(v1)) return __LINE__;
    }
    if(it.Data() == t2) {
      if(!binds.IsBound(v1)) return __LINE__;
    }
    if(it.Data() == t3) return __LINE__;
    count++;
  }
  if(count != 2) return __LINE__;
  return 0;
}

int testStateAnd() {
  std::cout << "testStateAnd() Started. \n";
  //cerr << "testStateAnd() \n";
  StateSetC state(true);
  LiteralC l1(true);
  LiteralC l2(true);
  LiteralC l3(true);
  VarC v1(true);
  VarC v2(true);
  
  TupleC t1(l1,l2);
  TupleC t2(l2,l3);
  TupleC t3(l2,l1);
  TupleC t4(l1,l3);
  state.Tell(t1);
  state.Tell(t2);
  state.Tell(t3);
  state.Tell(t4);
  
  LiteralC andTest(Tuple(v1,l3) * Tuple(l1,v2));
  //cerr << "testStateAnd() Creating iter.\n";
  BindSetC binds(true);
  int count = 0;
  for(LiteralIterC it(andTest.Solutions(state,binds));it;it++) {
    //ONDEBUG(std::cerr << "Got solution, Binds :" << binds << "\n");
    if(!binds.IsBound(v1)) return __LINE__;
    if(!binds.IsBound(v2)) return __LINE__;
    count++;
  }
  if(count != 4) return __LINE__;
  return 0;
}

int testStateNot() {
  std::cout << "testStateNot() Started. \n";
  //cerr << "testStateAnd() \n";
  StateSetC state(true);
  LiteralC l1(true);
  LiteralC l2(true);
  LiteralC l3(true);
  VarC v1(true);
  
  TupleC t1(l1,l2);
  TupleC t2(l2,l3);
  TupleC t3(l2,l1);
  TupleC t4(l1,l3);
  state.Tell(t1);
  state.Tell(t2);
  state.Tell(t3);
  state.Tell(t4);
  
  LiteralC notTest = !(Tuple(l1,l3) + Tuple(l2,v1));
  //cerr << "testStateAnd() Creating iter.\n";
  BindSetC binds(true);
  int count = 0;
  for(LiteralIterC it(notTest.Solutions(state,binds));it;it++) {
    //ONDEBUG(std::cerr << "Got solution, Binds :" << binds << "\n");
    //    if(!binds.IsEmpty()) return __LINE__;
    if(it.Data() != t1) return __LINE__;
    count++;
  }
  if(count != 1) return __LINE__;
  return 0;
}

int testLiteralIO() {
  StrIStreamC is("(hello) (fred one (three ?four)) fred");
  StateC state(true);
  ContextC context;
  if(!LoadState(is,state,context)) return __LINE__;
  if(state.Size() != 3) return __LINE__;
  state.Dump(std::cerr);
  return 0;
}

int testBinaryIO() {
  std::cerr << "Testing binary IO. \n";
#if 1
  ContextC context;
  LiteralC l1(true);
  LiteralC l2(true);
  LiteralC l3(true);
  VarC v1(true);
  VarC v2(true);
  StateC state1(true);
  OrC anor = l2 + v1;
  TupleC t1(l1,l2,v1,l1);
  LiteralC lt1 = l1 * l2 + v1 + !l3;
  MinTermC mt1(l1 * l3,anor);
  BindSetC bs1(true);
  bs1.Bind(v1,l2);
  bs1.Bind(v2,l3);
  state1.Tell(l1);
  state1.Tell(l2);
  if(!state1.Ask(t1[0])) return __LINE__;
  ValueC<IntT> value1(7);
  
  StrOStreamC os;
  BinOStreamC bos(os);
  bos << context << t1 << lt1 << bs1 << state1 << mt1 << value1;
  
  //cerr << "Reloading.\n";
  
  StringC data = os.String();
  StrIStreamC is(data);
  BinIStreamC bis(is);
  
#if 0
  OStreamC fileOut("test.dat");
  fileOut.write(data.chars(),data.Size());
  fileOut.os() << flush;
#endif
  MinTermC mt2;
  TupleC t2;
  LiteralC lt2;
  BindSetC bs2;
  StateC state2;
  ValueC<IntT> value2;
  
  bis >> context >> t2 >> lt2 >> bs2 >> state2 >> mt2 >> value2;
  
  // std::cout << "Binds=" << bs2 <<"\n";
  //cout << "l1=" << t2[0].Name() <<"\n";
  //cout << mt1.Name() << "\n";
  //cout << mt2.Name() << "\n";
  
  if(!t2[2].IsVariable()) return __LINE__;
  if(t2[0] != t2[3]) return __LINE__;
  if(t2[0] == t2[1]) return __LINE__;
  if(!state2.Ask(t2[0])) return __LINE__;
  if(value2.Data() != value1.Data()) return __LINE__;
#endif
  std::cerr << "Done.\n";
  return 0;
}
