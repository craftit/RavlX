// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlLogicAgent
//! file="Ravl/Logic/Agent/ActiveBlackboard.cc"

#include "Ravl/Logic/ActiveBlackboard.hh"
#include "Ravl/Logic/LiteralIndexIter.hh"
#include "Ravl/Logic/State.hh"
#include "Ravl/Logic/LiteralIO.hh"
#include "Ravl/Logic/Unify.hh"
#include "Ravl/Calls.hh"
#include "Ravl/CallMethods.hh"
#include "Ravl/Logic/Not.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlLogicN {
  
  //: Default constructor.
  
  ActiveBlackboardBodyC::ActiveBlackboardBodyC() 
    : index(true),
      ptriggers(true),
      ntriggers(true)
  {}
  
  
  //: Tell blackboard something.
  
  bool ActiveBlackboardBodyC::Tell(const LiteralC &key) 
  {
    static EmptyC anEmpty;
    static RCWrapC<EmptyC> emptyWrap(anEmpty);
    
    if(key.IsCondition()) {
      NotC aNot(key);
      if(aNot.IsValid())
	return Retract(aNot.Term());
      AndC aAnd(key);
      if(aAnd.IsValid()) {
	bool ret = true;
	SArray1dIterC<LiteralC> it(aAnd.Terms());
	for(it++;it;it++)
	  ret &= Tell(*it);
	return ret;
      }
      RavlAssertMsg(0,"ActiveBlackboardBodyC::Tell(), Unhandled conditional.");
      return false;
    }
    return Tell(key,(RCWrapAbstractC &)emptyWrap); 
  }
  
  //: Tell blackboard something.
  
  bool ActiveBlackboardBodyC::Tell(const LiteralC &key,const RCWrapAbstractC &data) {
    if(key.IsCondition()) {
      NotC aNot(key);
      if(aNot.IsValid())
	return Retract(aNot.Term(),data);
      AndC aAnd(key);
      if(aAnd.IsValid()) {
	bool ret = true;
	SArray1dIterC<LiteralC> it(aAnd.Terms());
	for(it++;it;it++)
	  ret &= Tell(*it,data);
	return ret;
      }
      RavlAssertMsg(0,"ActiveBlackboardBodyC::Tell(), Unhandled conditional.");
      return false;
    }
    RWLockHoldC lock(rwlock,RWLOCK_WRITE);
    index[key] = data;
    lock.Unlock();
    ProcessDelta(key,const_cast<RCWrapAbstractC &>(data),true);
    return true;
  }
  
  //: Remove a fact from the blackboard.
  
  bool ActiveBlackboardBodyC::Retract(const LiteralC &key) {
    if(key.IsCondition()) {
      NotC aNot(key);
      if(aNot.IsValid())
	return Tell(aNot.Term());
      OrC aOr(key);
      if(aOr.IsValid()) {
	bool ret = true;
	SArray1dIterC<LiteralC> it(aOr.Terms());
	for(it++;it;it++)
	  ret &= Retract(*it);
	return ret;
      }
      RavlAssertMsg(0,"ActiveBlackboardBodyC::Retract(), Unhandled conditional.");
      return false;
    }
    RWLockHoldC lock(rwlock,RWLOCK_WRITE);
    RCWrapAbstractC data;
    if(!index.Lookup(key,data))
      return false;
    if(!index.Del(key))
      return false;
    lock.Unlock();
    ProcessDelta(key,data,false);
    return true;
  }
  
  //: Remove a fact from the blackboard.
  
  bool ActiveBlackboardBodyC::Retract(const LiteralC &key,const RCWrapAbstractC &data) {
    if(key.IsCondition()) {
      NotC aNot(key);
      if(aNot.IsValid())
	return Tell(aNot.Term(),data);
      OrC aOr(key);
      if(aOr.IsValid()) {
	bool ret = true;
	SArray1dIterC<LiteralC> it(aOr.Terms());
	for(it++;it;it++)
	  ret &= Retract(*it,data);
	return ret;
      }
      RavlAssertMsg(0,"ActiveBlackboardBodyC::Retract(), Unhandled conditional.");
      return false;
    }
    RWLockHoldC lock(rwlock,RWLOCK_WRITE);
    RCWrapAbstractC idata;
    if(!index.Lookup(key,idata))
      return false;
    if(!index.Del(key))
      return false;
    lock.Unlock();
    ProcessDelta(key,idata,false);
    return true;    
  }

  //: Ask about a first match.
  
  bool ActiveBlackboardBodyC::Ask(const LiteralC &key,RCWrapAbstractC &data) {
    RWLockHoldC lock(rwlock,RWLOCK_READONLY);
    return index.Lookup(key,data);
  }
  
  //: Ask about a first match.
  
  bool ActiveBlackboardBodyC::Ask(const LiteralC &key) {
    RCWrapAbstractC data;
    RWLockHoldC lock(rwlock,RWLOCK_READONLY);
    return index.Lookup(key,data);
  }

  
  //: Load rules from a file.
  
  bool ActiveBlackboardBodyC::LoadRules(const StringC &filename) {
    StateC state(true);
    ContextC context;
    if(!LoadState(filename,state,context))
      return false;
    LiteralC pre = Var();
    LiteralC post = Var();
    LiteralC rules = Tuple(LiteralC("rule"),pre,post);
    LiteralC asserts = Tuple(LiteralC("tell"),post);
    BindSetC bnd(true);
    for(LiteralIterC it(state.Filter(rules,bnd));it;it++) {
      TupleC tup = *it;
      ONDEBUG(cerr << "ActiveBlackboardBodyC::LoadRules(), Rule: " << tup.Name() << "\n");
      AddRule(tup[1],tup[2]);
    }
    for(LiteralIterC it(state.Filter(asserts,bnd));it;it++) {
      TupleC tup = *it;
      ONDEBUG(cerr << "ActiveBlackboardBodyC::LoadRules(), Assert: " << tup.Name() << "\n");
      Tell(tup[1]);
    }
    
    return true;
  }
  
  //: Register a trigger.
  
  UIntT ActiveBlackboardBodyC::AddTrigger(const LiteralC &key,TriggerC trigger) {
    if(key.IsCondition()) {
      NotC aNot(key);
      if(aNot.IsValid()) {
	DListC<TriggerC> &list = ntriggers.Access(aNot.Term());
	list.InsLast(trigger);
	return 0;
      }
      OrC aOr(key);
      if(aOr.IsValid()) {
	SArray1dIterC<LiteralC> it(aOr.Terms());
	for(it++;it;it++)
	  AddTrigger(*it,trigger);
	return 0;
      }
      RavlAssertMsg(0,"ActiveBlackboardBodyC::AddTrigger(), Unhandled conditional.");      
      return false;
    }
    DListC<TriggerC> &list = ptriggers.Access(key);
    list.InsLast(trigger);
    return 0;
  }
  
  //: Process add signal.
  
  bool ActiveBlackboardBodyC::ProcessDelta(const LiteralC &key,RCWrapAbstractC &data,bool tell) {
    ONDEBUG(cerr << "ActiveBlackboardBodyC::ProcessDelta(), Called. Key=" << key.Name() << "\n");
    RWLockHoldC lock(rwlock,RWLOCK_READONLY);
    LiteralC literal(key);
    DListC<DListC<TriggerC> > todo;
    if(tell) {
      for(LiteralIndexFilterC<DListC<TriggerC> > it(ptriggers,key);it;it++)
	todo.InsLast(it.MappedData());
    } else {
      for(LiteralIndexFilterC<DListC<TriggerC> > it(ntriggers,key);it;it++)
	todo.InsLast(it.MappedData());
    }
    lock.Unlock();
    LiteralC x(key);
    for(DLIterC<DListC<TriggerC> > it(todo);it;it++) {
      ONDEBUG(cerr << " Got match " << key.Name() << "\n");
      for(DLIterC<TriggerC> lit(*it);lit;lit++) {
	CallFunc2C<LiteralC,RCWrapAbstractC> call2(*lit,true);
	if(call2.IsValid()) {
	  call2.Call(x,data);
	  continue;
	}
	CallFunc1C<LiteralC> call1(*lit,true);
	if(call1.IsValid()) {
	  call1.Call(x);
	  continue;
	}
	lit->Invoke();
      }
    }
    return true;
  }
  
  //: Handle assertion of a post condition
  
  bool ActiveBlackboardBodyC::AssertPost(LiteralC event,
					 const LiteralC &preCond,
					 const LiteralC &postCond) {
    ONDEBUG(cerr << "ActiveBlackboardBodyC::AssertPost(), Event=" << event.Name() << " Pre=" << preCond.Name() << " Post=" << postCond.Name() << "\n");
    BindSetC binds(true);
    if(!Unify(preCond,event,binds)) {
      cerr << "ActiveBlackboardBodyC::AssertPost(), ERROR: Unify failed. \n";
      return true;
    }
    LiteralC newLit;
    postCond.Substitute(binds,newLit);
    ONDEBUG(cerr << "ActiveBlackboardBodyC::AssertPost(), Tell=" << newLit.Name() << " binds=" << binds << "\n");
    Tell(newLit);
    return true;
  }
  
  //: Add a rule to the blackboard.
  // When 'pre' condition it met, assert 'post' condition.
  
  UIntT ActiveBlackboardBodyC::AddRule(const LiteralC &pre,const LiteralC &post) {
    LiteralC tmp;
    return AddTrigger(pre,TriggerR(*this,&ActiveBlackboardBodyC::AssertPost,tmp,pre,post));
  }
  
  //: Dump state of blackboard.
  
  void ActiveBlackboardBodyC::Dump(ostream &out) const {
    RWLockHoldC lock(rwlock,RWLOCK_READONLY);
    for(LiteralIndexIterC<RCWrapAbstractC> it(index);it;it++)
      out << it.Data().Name() << "\n";
  }
  
}
