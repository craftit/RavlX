// This file is part of CxxDoc, The RAVL C++ Documentation tool 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU General 
// Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlCxxDoc
//! file="Ravl/SourceTools/CxxDoc/CxxScope.cc"

#include "Ravl/CxxDoc/CxxScope.hh"
#include "Ravl/CxxDoc/CxxElements.hh"
#include "Ravl/CxxDoc/Strings.hh"
#include "Ravl/DLIter.hh"
#include "Ravl/CDLIter.hh"
#include "Ravl/HashIter.hh"
#include "Ravl/StringList.hh"
#include "Ravl/HSet.hh"
#include "Ravl/BlkQueue.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlCxxDocN
{

  //// ScopeBodyC ////////////////////////////////////////////////////////
  
  //: Constructor.
  
  ScopeBodyC::ScopeBodyC(const StringC &nname,const ObjectC &contents)
    : ObjectListBodyC(nname)
  {
    AppendList(contents);
  }

  //: Match template arguments.
  // puts the results into templTab.
  bool ScopeBodyC::MatchTemplateArgs(ClassTemplateC &tc,ObjectC &pathObj,RCHashC<StringC,ObjectC> &templTab) {
    if(!pathObj.IsTemplate()) {
      // Check for default template args ?
      ONDEBUG(std::cerr << "Inheriting for templated object without args from " << pathObj.Name() << " \n");
      return false;
    }
    // Match the template args.
    DLIterC<ObjectC> it1(tc.TemplArgList().List()); // Iterate class args.
    DLIterC<ObjectC> it2(pathObj.TemplArgList().List());
    for(;it1 && it2;it1++,it2++) {
      StringC nm1;
      if(DataTypeC::IsA(*it1)) {
	DataTypeC dt(*it1);
	nm1 = dt.Alias();
      } else
	nm1=it1->Name();
      ONDEBUG(std::cerr << "Template Substitute: " << nm1  << " for " << it2->Name() << " (Templ:" << it2->IsTemplate() << " Type:" << it2->TypeName() << " Fn:" << it2->FullName(templTab) << ")\n");
      templTab[nm1] = *it2;
    }
    if(it1 || it2) {
      std::cerr << "Mismatch in number of template args for " << tc.Name() << " from " << pathObj.Name() << "\n";
      return false;
    }
    return true;
  }
  
  //: Lookup name allowing for inheritance.
  // returns true if object has been found.
  
  bool ScopeBodyC::LookupI(const StringC &name,ObjectC &ret,bool useInherit,HSetC<ScopeC> &done) {
    ONDEBUG(std::cerr << "ScopeBodyC::LookupI(), In " << Name() << " of '" << name << "' Inherit:"  << useInherit << "\n");
    if(tab.Lookup(name,ret)) {
      ONDEBUG(std::cerr << "ScopeBodyC::LookupI(), Found " << name << " in '" << Name() << "' \n");	
      return true;
    }
    if(!useInherit)
      return false;
    done += ScopeC(*this); // Done this one!
    // Check up inheritance hirachy for name.
    for(DLIterC<ObjectC> it(uses);it;it++) {
      InheritC inhrt(*it);
      if(!inhrt.IsValid())
	continue;
      ONDEBUG(std::cerr << "Inherit : '" << inhrt.Name() << "'\n");
      ScopeC &scope = inhrt.From();
      if(!scope.IsValid())
	continue;
      if(done[scope])
	continue;
      done += scope;
      if(scope.LookupI(name,ret,true,done)) {
	ONDEBUG(std::cerr << "ScopeBodyC::LookupI(), Found " << name << " in '" << Name() << "' \n");	
	return true;
      }
    }
    return false;
  }
  
  //: Resolve a name.
  
  ObjectC ScopeBodyC::ResolveName(const StringC &str,bool useInherit) {
    ObjectC start(*this);
    if(str == "::") // Start at root ?
      return RootScope();
    if(str == start.BaseName()) // Are we in it already ?
      return start;
    do {
      ONDEBUG(std::cerr << "ScopeBodyC::ResolveName(StringC) Start Scope: '" << start.Name() << "'  Looking for:'" << str << "'\n"); 
      if(!ScopeC::IsA(start))
	break;
      ScopeC tmpScope(start);
      ObjectC place;
      if(tmpScope.LookupI(str,place,useInherit)) {
	if(place.IsValid())
	  return place;
      }
      // Goto parent scope and try to resolve the path again.
      if(!start.HasParentScope()) 
	break; // No more parents.
      start = start.ParentScope();
    } while(1) ;
    return ObjectC();
  }
  
  //: Resolve a name.
  
  ObjectC ScopeBodyC::ResolveName(DListC<ObjectC> path,RCHashC<StringC,ObjectC> &templSub,bool useInherit) {
    ObjectC start(*this);
    DLIterC<ObjectC> it(path);
    // Go back through parent scopes looking for an object that matches 'path'.
    do {
      it.First();
      if(!it.IsElm()) {
	cerr << "ScopeBodyC::ResolveName() WARNING: Path is empty. \n";
	return ObjectC(); // Failed...
      }
      if(it->Name() == "::") { // Start at root ?
	start = RootScope();
	it.Next();
      }
      ONDEBUG(std::cerr << "ScopeBodyC::ResolveName() Start Scope: '" << start.Name() << "'  Looking for:'" << PathName(path) << "'\n"); 
      templSub.Empty(); // Remove any existing subsitutions.
      ObjectC place(start);
      bool failed = false;
      for(;it;it++) {
	if(!ScopeC::IsA(place)) {
	  failed = true;
	  break;
	}
	ScopeC tmpScope(place);
	if(!tmpScope.LookupI(it->BaseName(),place,useInherit)) {
	  failed = true;
	  break;
	}
	
	// Specification for a templated object ?
	if(ClassTemplateC::IsA(place)) {
	  // Look for subtitutions for template parameters.
	  ClassTemplateC ct(place);
	  MatchTemplateArgs(ct,*it,templSub);
	}
      }
      if(!failed && !it.IsElm() && place.IsValid() && !DataTypeC::IsA(place)) {
	//cerr << "Found '" << place.Name() << "'\n";	
	return place;
      }
      // Goto parent scope and try to resolve the path again.
      if(!start.HasParentScope()) {
	//cerr << "No parent. \n";
	break; // No more parents.
      }
      start = start.ParentScope();
    } while(1) ;
    return ObjectC();
  }
    
  //: Get the root scope.
  
  ScopeC ScopeBodyC::RootScope() {
    ScopeC place(*this);
    while(place.Body().parent != 0) 
      place = ScopeC(*place.Body().parent);
    return place;
  }

  void ScopeBodyC::Append(ObjectC &obj)  {
    //cerr << "ScopeBodyC::Append() Object '" << obj.Name() << "' dectype:" << obj.Var("dectype") << "\n";
    if(!obj.IsValid())
      return ;
    if(ObjectListC::IsA(obj)) { // Is it a object list wrapper.
      ObjectListC ol(obj);
      if(ol.IsWrapper()) {
	for(DLIterC<ObjectC> it(ol.List());it.IsElm();it.Next())
	  Append(it.Data());
	return ;
      }
    }
    
    ObjectC subScope = obj.GetScope();
    if(subScope.IsValid()) {
      if(tab.IsElm(subScope.Name())) {
	//cerr  << "Putting into sub scope: '" << obj.Name() <<"' SubScope: '" << subScope.Name() << "'\n";
	ObjectC empty;
	obj.SetScope(empty); // Clear scope slot.
	ObjectC &sub = tab[subScope.BaseName()];
	if(ScopeC::IsA(sub))
	  ScopeC(sub).Append(obj); // Append into to proper place.
      }
      return ;
    }
    if(InheritC::IsA(obj))
      uses.InsLast(obj);
    
    obj.SetParentScope(this);
    
    if(!tab.IsElm(obj.Name())) {
      // Just add new entity.
      tab.Insert(obj.BaseName(),obj);
      List().InsLast(obj); 
      return ;
    }
    
    ObjectC &oldobj = tab[obj.BaseName()];
    
    // Handle forward declarations.
    StringC objDecType = obj.Var("dectype");
    StringC oldDecType = oldobj.Var("dectype");
    if(objDecType == "forward")
      return ; // Ignore forwards after any other declaration.
    if(oldDecType == "prototype") {
      if(objDecType == "definition") {
	if(MethodC::IsA(oldobj)) {
	  MethodC method(oldobj);
	  method.SetDefinition(obj);
	}
	return ; // 
      }
      if(objDecType == "prototype")  // Nothing wrong with having two prototypes...
	return ; // 
    }
    
    if(oldDecType == "friend" && objDecType == "friend") 
      return ; // Ignore it.
    
    if(objDecType == "extern") 
      return ; // Ignore it.
    
    if(oldDecType == "forward" || oldDecType == "extern" || (oldDecType == "friend" && objDecType != "friend")) {
      //ONDEBUG(std::cerr << "Replacing " << oldobj.Name() << "\n");
      for(DLIterC<ObjectC> it(List());it.IsElm();it.Next()) {
	//ONDEBUG(std::cerr << "Checking '" << it.Data().Name() << "' \n");
	ObjectC &listobj = it.Data();
	if(listobj == oldobj) {
	  listobj = obj;
	  oldobj = obj; // Replace in table as well.
	  //ONDEBUG(std::cerr << "Replaced " << oldobj.Name() << "\n");
	  break;
	}
      }
      return ;
    }
    
    // Merge namespaces....
    
    if(ScopeC::IsA(oldobj)) {
      if(ScopeC::IsA(obj)) {
	if(ClassC::IsA(obj) || ClassC::IsA(oldobj)) {
	  std::cerr << "ERROR: Class/Namespace or Class/Class clash for '" << obj.Name() << "' Old:" << oldDecType << " New:" << objDecType << "\n";
	  return ;
	}
	ScopeC newScope(obj);
	ScopeC oldScope(oldobj);
	oldScope.Merge(newScope);
	return ;
      }
      return ;
    }
    if(oldDecType == "definition") {
      if(objDecType == "prototype") {
	cerr << "WARNING: Replacing definition with prototype for " << oldobj.Name() <<"\n";
	ObjectC wasOld = oldobj;
	oldobj = obj; // Use prototype for documentation
	if(MethodC::IsA(obj)) {
	  MethodC aMethod(obj);
	  aMethod.SetDefinition(wasOld);
	  if(aMethod.ScopeInfo().IsValid())
	    return ; // Its just cause we haven't resolved scopes properly !
	}
      }
      return ; // 
    }
    
    std::cerr << "ERROR: Duplicate name for object '" << obj.Name() << "' Old:" << oldDecType << " New:" << objDecType << "\n";
    return ;
  }
  
 

  //: Merge this scope with another.
  
  bool ScopeBodyC::Merge(const ScopeC &oth) {
    AppendList(oth);
    int master = 0;
    if(Comment().Locals().IsElm("maindoc") || Comment().Vars().IsElm("maindoc"))
      master = 2;
    if(oth.Comment().Locals().IsElm("maindoc") || oth.Comment().Vars().IsElm("maindoc"))
      master = 1;
    if(master != 0)
      Comment().Locals()["maindoc"] = "1"; // Either was this is the main doc now.
    
    while(1) { // Just so we can break out.
      StringC de2 = oth.Var("docentry");
      if(de2 == "") // Any docentry to merge ?
	break;
      if(!IsVar("docentry")) {// Got a local docentry ?
	SetVar("docentry",de2);
	break;
      }
      StringC de1 = Var("docentry");
      if(de1 == de2)
	break;
      HSetC<StringC> set;
      StringListC ent1(de1,";");
      StringListC ent2(de2,";");
      {
	for(DLIterC<StringC> it(ent1);it;it++)
	  set += (*it).TopAndTail() ;
      }
      {
	for(DLIterC<StringC> it(ent2);it;it++)
	  set += (*it).TopAndTail();
      }
      HSetIterC<StringC> it(set);
      if(!it.IsElm())
	break;
      StringC newun = (*it).Copy();
      it++;
      for(;it;it++)
	newun += StringC(";") + *it;
      SetVar("docentry",newun);
      break;
    }
    
    if(master == 0) {
      // Sort out comment and userlevel.
      // Take the longest comment and its userlevel.
      int desc1 = Comment().Text().length() + Comment().Header().length();
      int desc2 = oth.Comment().Text().length() + oth.Comment().Header().length();
      if(desc2 > desc1) {
	Comment().Text() = oth.Comment().Text();
	Comment().Header() = oth.Comment().Header();
      }
    } else {
      // Use the marked one.
      if(master == 1) {
	Comment().Text() = oth.Comment().Text();
	Comment().Header() = oth.Comment().Header();
	SetVar(STR(brief),comment.Header());
	SetVar(STR(detail),comment.Text());
      }
    }
    return true;
  }
  
  //: Merge this scope with another.
  
  bool ScopeBodyC::Merge(const ObjectC &oth) {
    if(!ScopeC::IsA(oth))
      return false;
    ScopeC ascope(const_cast<ObjectC &>(oth));
    return Merge(ascope);
  }

  //: Resolve links in input data.
  
  void ScopeBodyC::Resolve() {
    ONDEBUG(std::cerr << "ScopeBodyC::Resolve() '" << Name() << "' \n");
    BlkQueueC<ScopeC> todo;
    todo.InsLast(ScopeC(*this));
    // Resolve width first.
    while(!todo.IsEmpty()) {
      ScopeC here(todo.Pop());
      for(DLIterC<ObjectC> it(here.List());it.IsElm();it.Next()) {
	if(ScopeC::IsA(it.Data())) {
	  ScopeC scope(it.Data());
	  todo.InsLast(scope);
	  continue;
	}
	if(InheritC::IsA(it.Data())) {
	  InheritC ihr(it.Data());
	  ihr.Resolve();
	  continue;
	}
      }
    }
  }


  // InheritIterC ////////////////////////////////////////////////////
  
  //: Goto next valid element.
  
  bool InheritIterC::ListNext(InheritIterC::InstanceDefT &start) {
    done += start;
    currentTSubs = start.Data2();
    if(!ObjectListC::IsA(start.Data1()))
      return true;
    ObjectListC ol(start.Data1());
    at = ol.List();
    if(!ScopeC::IsA(start.Data1())) 
      return true;
    ScopeC ss(start.Data1());
    currentScope = ss;
    
    for(DLIterC<ObjectC> it(ss.Uses());it;it++) {
      ObjectC newObj;
      RCHashC<StringC,ObjectC> newSubs;
      if(InheritC::IsA(*it)) {
	if(localOnly)
	  continue;
	InheritC inh(*it);
	// Check access.
	switch(inh.ScopeAccess())
	  {
	  case SAPrivate:
	    if(incAccess != SAPrivate)
	      continue;
	      /* no break */
	  case SAProtected:
	    if(incAccess == SAPublic)
	      continue;
	      /* no break */
	  case SAPublic: 
	    break; // Always.
	  }
	newObj = inh.From();
#if DODEBUG
	if(newObj.IsValid())
	  std::cerr << "InheritIterC::ListNext(),  Current:" << ss.Name() << " New scope:" << newObj.Name() << " \n"; 
	else 
	  std::cerr << "InheritIterC::ListNext(),  Current:" << ss.Name() << " New scope: *Unknown* \n"; 
#endif
	// Sort out template sub's.
	if(!currentTSubs.IsEmpty()) { // Have we got some to merge ??
	  ObjectC *chk;
	  for(HashIterC<StringC,ObjectC> it((HashC<StringC,ObjectC> &)inh.TemplateSubs());it;it++) {
	    if((chk = currentTSubs.Lookup(it.Data().BaseName())) == 0)
	      chk = &it.Data();
	    newSubs[it.Key()] = chk->Subst(currentTSubs);
	    //newSubs[it.Key()] = ObjectC(chk->FullName(currentTSubs)); // Hack!
	    ONDEBUG(std::cerr << "InheritIterC::ListNext(), Sub '" << it.Key() << "' = '" << chk->Name() << "' Was:'" << it.Data() << "'\n");
	  }
	} else  // No templates subs to merge so just use inherit ones.
	  newSubs = inh.TemplateSubs();
	
      } else {
	newObj = *it;
	if(newObj.IsValid())
	  std::cerr << "WARNING: Non inherit in uses '" << newObj.Name() << "' Ignoring.\n";
	newObj.Invalidate();
      }
      if(newObj.IsValid())
	toDo.InsLast(InstanceDefT (newObj,newSubs));
    }
    return true;
  }
  
  //: Goto next valid element.
  
  bool InheritIterC::NextValid() {
    if(at.IsElm())
      return true;
    do {
      InstanceDefT next;
      do {
	if(toDo.IsEmpty())
	  return false;
	next = toDo.PopFirst();
	if(!next.Data1().IsValid())
	  continue;
      } while(done.IsMember(next) || !ObjectListC::IsA(next.Data1())) ;
      ListNext(next);
    } while(!at.IsElm()); // In case of empty list....
    return true;    
  }

  
  //: Default constructor.
  
  InheritIterC::InheritIterC(ObjectC &start,ScopeAccessT nIncAccess,bool nLocalOnly)
    : localOnly(nLocalOnly),
      incAccess(nIncAccess)
  {
    RCHashC<StringC,ObjectC> subs;
    InstanceDefT inital(start,subs);
    ListNext(inital);
    NextValid();
  }
  
  //: Goto next.
  
  bool InheritIterC::Next() {
    at.Next();
    return NextValid();
  }
  

}
