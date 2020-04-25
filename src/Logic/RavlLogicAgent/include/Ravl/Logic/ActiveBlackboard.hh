// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_ACTIVEBLACKBOARD_HEADER
#define RAVL_ACTIVEBLACKBOARD_HEADER 1
//! author="Charles Galambos"
//! userlevel=Normal
//! docentry="Ravl.API.Logic.Agent"
//! rcsid="$Id$"
//! lib=RavlLogicAgent
//! file="Ravl/Logic/Agent/ActiveBlackboard.hh"

#include "Ravl/Logic/Literal.hh"
#include "Ravl/Logic/LiteralIndex.hh"
#include "Ravl/Logic/LiteralIndexFilter.hh"
#include "Ravl/RCWrap.hh"
#include "Ravl/Threads/RWLock.hh"
#include "Ravl/DList.hh"

namespace RavlLogicN {
  
  //! userlevel=Develop
  //: Active blackboard
  
  class ActiveBlackboardBodyC
    : public RCBodyVC
  {
  public:
    ActiveBlackboardBodyC();
    //: Default constructor.
    
    bool LoadRules(const StringC &filename);
    //: Load rules from a file.
    
    UIntT AddTrigger(const LiteralC &condition,TriggerC trigger);
    //: Register a trigger.
    
    virtual bool Tell(const LiteralC &key,const RCWrapAbstractC &data);
    //: Tell blackboard something.
    
    template<class DataT>
    bool Tell(const LiteralC &key,const DataT &data) {
      RCWrapC<DataT> wrap(data);
      return Tell(key,(const RCWrapAbstractC &) wrap);
    }
    //: Tell blackboard something.
    
    virtual bool Tell(const LiteralC &key);
    //: Tell blackboard something.
    
    virtual bool Retract(const LiteralC &key);
    //: Remove a fact from the blackboard.
    
    virtual bool Retract(const LiteralC &key,const RCWrapAbstractC &data);
    //: Remove a fact from the blackboard.
    
    virtual bool Ask(const LiteralC &key,RCWrapAbstractC &data);
    //: Ask about a first match.
    
    virtual bool Ask(const LiteralC &key);
    //: Ask about a first match.
    
    template<class DataT>
    bool Ask(const LiteralC &key,DataT &data) {
      RCWrapAbstractC wrap;
      if(!Ask(key,wrap))
	return false;
      RCWrapC<DataT> rcw(wrap,true);
      if(!rcw.IsValid())
	return false;
      data = rcw.Data();
      return true;
    }
    //: Ask about a first match.
    
    template<class DataT>
    UIntT AskAll(const LiteralC &key,DListC<Tuple2C<LiteralC,DataT> > &list) {
      RWLockHoldC lock(rwlock,RWLOCK_READONLY);
      for(LiteralIndexFilterC<RCWrapAbstractC> it(index,key);it;it++) {
	RCWrapC<DataT> rcw(it.MappedData(),true);
	if(!rcw.IsValid())
	  return false;
	list.InsLast(Tuple2C<LiteralC,DataT>(it.Data(),rcw.Data()));
      }
      return true;
    }
    //: Ask about a all matchs.
    
    UIntT AddRule(const LiteralC &pre,const LiteralC &post);
    //: Add a rule to the blackboard.
    // When 'pre' condition it met, assert 'post' condition.
    // Returns an id for the rule.
    
    void Dump(ostream &out) const;
    //: Dump state of blackboard.
    
  protected:
    bool ProcessDelta(const LiteralC &lit,RCWrapAbstractC &data,bool tell = true);
    //: Process add signal.
    
    bool AssertPost(LiteralC event,const LiteralC &preCond,const LiteralC &postCond);
    //: Handle assertion of a post condition

    RWLockC rwlock;
    
    LiteralIndexC<RCWrapAbstractC> index;
    LiteralIndexC<DListC<TriggerC> > ptriggers;
    LiteralIndexC<DListC<TriggerC> > ntriggers;
  };
  
  //! userlevel=Normal
  //: Active blackboard
  
  class ActiveBlackboardC
    : public RCHandleVC<ActiveBlackboardBodyC>
  {
  public:
    ActiveBlackboardC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    ActiveBlackboardC(bool)
      : RCHandleVC<ActiveBlackboardBodyC>(*new ActiveBlackboardBodyC())
    {}
    //: Constructor.
    
  protected:
    ActiveBlackboardC(ActiveBlackboardBodyC &bod)
      : RCHandleVC<ActiveBlackboardBodyC>(bod)
    {}
    //: Body constructor.
    
    ActiveBlackboardBodyC& Body()
    { return RCHandleVC<ActiveBlackboardBodyC>::Body(); }
    //: Access body.

    const ActiveBlackboardBodyC& Body() const
    { return RCHandleVC<ActiveBlackboardBodyC>::Body(); }
    //: Access body.
    
  public:
    
    bool LoadRules(const StringC &filename)
    { return Body().LoadRules(filename); }
    //: Load rules from a file.
    
    UIntT AddTrigger(const LiteralC &condition,TriggerC trigger)
    { return Body().AddTrigger(condition,trigger); }
    //: Register a trigger.
    
    template<class DataT>
    bool Tell(const LiteralC &key,const DataT &data) 
    { return Body().Tell(key,data); }
    //: Tell blackboard something.
    
    bool Tell(const LiteralC &key,const RCWrapAbstractC &data)
    { return Body().Tell(key,data); }
    //: Tell blackboard something.
    
    bool Tell(const LiteralC &key) 
    { return Body().Tell(key); }
    //: Tell blackboard something.
    
    bool Retract(const LiteralC &key)
    { return Body().Retract(key); }
    //: Remove a fact from the blackboard.
    
    bool Ask(const LiteralC &key,RCWrapAbstractC &data) 
    { return Body().Ask(key,data); }
    //: Ask about a first match.
    
    bool Ask(const LiteralC &key)
    { return Body().Ask(key); }
    //: Ask about a first match.
    
    template<class DataT>
    bool Ask(const LiteralC &key,DataT &data) 
    { return Body().Ask(key,data); }
    //: Ask about a first match.

    template<class DataT>
    UIntT AskAll(const LiteralC &key,DListC<Tuple2C<LiteralC,DataT> > &list) 
    { return Body().AskAll(key,list);  }
    //: Ask about a all matchs.
    
    UIntT AddRule(const LiteralC &pre,const LiteralC &post)
    { return Body().AddRule(pre,post); }
    //: Add a rule to the blackboard.
    // When 'pre' condition it met, assert 'post' condition.
    
    void Dump(ostream &out) const
    { return Body().Dump(out); }
    //: Dump state of blackboard.
  };
  
  
}


#endif 
