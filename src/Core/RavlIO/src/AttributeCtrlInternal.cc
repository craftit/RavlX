// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlIO

#include "Ravl/DP/AttributeCtrlInternal.hh"
#include "Ravl/CallMethods.hh"
#include "Ravl/HashIter.hh"
#include "Ravl/SysLog.hh"

namespace RavlN {
  //: Default constructor.
  
  AttributeCtrlInternalC::AttributeCtrlInternalC()
    : access(true),
      trigIdAlloc(1)
  {}
  

  //: Destructor.
  
  AttributeCtrlInternalC::~AttributeCtrlInternalC() {
    RemoveChangedSignals();
  }

  //: Remove all currently registered changed signals from parent.
  
  bool AttributeCtrlInternalC::RemoveChangedSignals() {
    AMutexLockC lock(access);
    // Remove any existing changed signals.
    for(HashIterC<StringC,IntT> it(parentSignalMap);it;it++) {
      if(it.Data() == -1)
        continue;
      parentAttrCtrl.RemoveChangedSignal(it.Data());
    }
    parentSignalMap.Empty();
    return true;
  }
  
  //: Remapall currently registered changed signals to the new parent.
  
  bool AttributeCtrlInternalC::RemapChangedSignals(const AttributeCtrlC &parent) {
    AMutexLockC lock(access);    
    // Remove any existing changed signals.
    for(HashIterC<StringC,IntT> it(parentSignalMap);it;it++) {
      if(it.Data() == -1)
        continue;
      RavlAssert(parentAttrCtrl.IsValid());
      parentAttrCtrl.RemoveChangedSignal(it.Data());
    }
    
    // Setup new parent.
    parentAttrCtrl = parent;
    
    if(parentAttrCtrl.IsValid()) {
      // Reconstruct signals with new parent.
      
      for(HashIterC<StringC,IntT> it(parentSignalMap);it;it++) {
        it.Data() = parentAttrCtrl.RegisterChangedSignal(it.Key(),TriggerR(*this,&AttributeCtrlInternalC::CBHandleChangeSignal,it.Key()));
        // It may be the new parent doesn't support a signal, in which case RegisterChangedSignal will return -1.
        // We'll just ignore this for now.
      }
    }
    return true;
  }
  
  //: Issue attribute changed signal.
  
  void AttributeCtrlInternalC::IssueChangedSignal(const StringC &attrName) {
    AMutexLockC lock(access);
    DListC<TriggerC> lst;
    if(!name2trigList.Lookup(attrName,lst))
      return ; // Nothing to invoke.
    DListC<TriggerC> sendLst = lst.Copy();
    lock.Unlock();
    for(DLIterC<TriggerC> it(sendLst);it;it++)
      (*it).Invoke();
  }
  
  //: Register a changed signal.
  
  IntT AttributeCtrlInternalC::RegisterChangedSignal(const StringC &attrName,const TriggerC &trig,const AttributeCtrlC &parent) {
    AMutexLockC lock(access);
    if(parent != parentAttrCtrl) {
      if(parentAttrCtrl.IsValid())
        RavlWarning("AttributeCtrlInternalC::RegisterChangedSignal, WARNING: Late remapping of attribute changed signal. ");
      RemapChangedSignals(parent);
    }
    // Do we need to map a parent changed signal into this one ?
    if((!attribTypes.IsElm(attrName) || mapSignalBack[attrName]) && !parentSignalMap.IsElm(attrName)) {
      if(!parentAttrCtrl.IsValid() && !mapSignalBack[attrName]) {
        RavlWarning("AttributeCtrlInternalC::RegisterChangedSignal, Unknown attribute '%s' (No parent) ",attrName.c_str());
        //RavlAssert(0);
        return -1;
      }
      if(parentAttrCtrl.IsValid()) {
        IntT value = parentAttrCtrl.RegisterChangedSignal(attrName,TriggerR(*this,&AttributeCtrlInternalC::CBHandleChangeSignal,attrName));
        if(value < 0 && !mapSignalBack[attrName]) {
          RavlWarning("AttributeCtrlInternalC::RegisterChangedSignal, Unknown attribute '%s' ",attrName.c_str());
          return -1;
        }
        parentSignalMap[attrName] = value;
      }
    }
    // Register signal in local context.
    IntT id = trigIdAlloc++;
    DListC<TriggerC> &list = name2trigList[attrName];
    list.InsLast(trig);
    trigId2trig[id] = list;
    trigId2trig[id].Last();
    return id;
  }
  
  //: Remove changed signal.
  
  bool AttributeCtrlInternalC::RemoveChangedSignal(IntT id,const AttributeCtrlC &parent) {
    AMutexLockC lock(access);
    if(parent != parentAttrCtrl) {
      if(parentAttrCtrl.IsValid())
        RavlWarning("AttributeCtrlInternalC::RemoveChangedSignal, WARNING: Late remapping of attribute changed signal. ");
      RemapChangedSignals(parent);
    }
    
    DLIterC<TriggerC> *trig = trigId2trig.Lookup(id);
    if(trig == 0) 
      return false;
    trig->Del();
    trigId2trig.Del(id);
    
    // We could unregister unneeded signals from the parent attribute here. Might save 
    // a little run time.
    return true;
  }
  
  //: Signal updates on all registered callbacks
  
  bool AttributeCtrlInternalC::IssueRefreshSignal() {
    DListC<TriggerC> lst;
    // Make a list of triggers to call, with a lock on.
    AMutexLockC lock(access);
    for(HashIterC<StringC,DListC<TriggerC> > it(name2trigList);it;it++) {
      DListC<TriggerC> tmp = it.Data().Copy();
      lst.MoveLast(tmp);
    }
    lock.Unlock();
    // Call everything with the lock disabled.
    for(DLIterC<TriggerC> it(lst);it;it++)
      (*it).Invoke();
    return true;
  }
  
  //: Map attribute changed signal to parent even if its an attribute at this level.
  
  bool AttributeCtrlInternalC::MapBackChangedSignal(const StringC &sigName) {
    mapSignalBack += sigName;
    return true;
  }
  
  //: Handle an update signal from the parent object.
  
  bool AttributeCtrlInternalC::CBHandleChangeSignal(StringC &sigName) {
    IssueChangedSignal(sigName);
    return true;
  }

}
