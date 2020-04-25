// This file is part of CxxDoc, The RAVL C++ Documentation tool 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU General 
// Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlCxxDoc
//! file="Ravl/SourceTools/CxxDoc/CxxElements.cc"

#include "Ravl/CxxDoc/CxxElements.hh"
#include "Ravl/DLIter.hh"
#include "Ravl/CDLIter.hh"
#include "Ravl/HashIter.hh"
#include "Ravl/StringList.hh"
#include "Ravl/HSet.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlCxxDocN
{
  
  //: Generate a full path name, with template paramiter subs.

  StringC PathName(const DListC<ObjectC> &path,RCHashC<StringC,ObjectC> &templSub,DesciptionGeneratorC &dg = defaultDescGen,int maxDepth = 20) {
    StringC ret;
    bool first = true;
    //cerr << "Making path....\n";
    for(ConstDLIterC<ObjectC> it(path);it.IsElm();it.Next()) {
      if(!it->IsValid()) {
	ret += "(NULL)";
	continue;
      }
      if(first)
	first = false;
      else
	ret += "::";
      if(maxDepth < 1) {
	ONDEBUG(std::cerr << "PathName(DListC<ObjectC>) ");
	cerr << "ERROR: Maximum template depth exceeded in arg '" << it->Name() <<  "' Type:" << it->TypeName() <<" \n";
	cerr << "Subs:";
	for(HashIterC<StringC,ObjectC> sit((HashC<StringC,ObjectC> &)templSub);sit;sit++)
	  std::cerr << " " << sit.Key() << " -> " << sit.Data().Name() << "\n";
	cerr << "\nSubs end.\n" << flush;
	RavlAssert(0);
	ret += it->Name();
      } else {
	if(maxDepth < 9)
	  std::cerr << "PathName(DListC<ObjectC>), WARNING: Maximum template depth nearly exceeded (" << maxDepth <<") in arg '" << it->Name() <<  "' Type:" << it->TypeName() <<"\n";
	ret += it->FullName(templSub,dg,maxDepth-1);
      }
    }
    return ret;
  }
  
  //: Generate a full path name.
  
  StringC PathName(DListC<ObjectC> path,DesciptionGeneratorC &dg,int maxDepth) {
    return PathName(path,emptyTemplSubst,dg,maxDepth);
  }

  using namespace RavlN;

  //// ObjectTemplateC ////////////////////////////////////////////////////////

  //: Constructor.
  
  ObjectTemplateBodyC::ObjectTemplateBodyC(const StringC &name,const ObjectC &argLst)
    : ObjectBodyC(name),
      argList(argLst)
  {
    ONDEBUG(std::cerr << "ObjectTemplateBodyC::ObjectTemplateBodyC() Name:'" << name << "'\n");
  }
  
  //: Get full name of object
  // template args and all.
  
  StringC ObjectTemplateBodyC::FullName(RCHashC<StringC,ObjectC> &templSub,DesciptionGeneratorC &dg,int maxDepth) const {
    ONDEBUG(std::cerr << "ObjectTemplateBodyC::FullName() for '" << Name() << "' called. \n");
    StringC ret = dg.TextFor(ObjectC(const_cast<ObjectBodyC &>(((ObjectBodyC &)*this)))).Copy(); // Make sure its a copy.
    ret += dg.TextFor('<');
    bool first = true;
    // Go through template args.
    for(ConstDLIterC<ObjectC> it(argList.List());it;it++) {
      // std::cerr << "ObjectTemplateBodyC::FullName() arg:'" << it->Name() << "' Type:" << it->TypeName() << "\n";
      if(first)
	first = false;
      else
	ret += dg.TextFor(',');
      // Is template arg a datatype in itself ? or a constant.
      if(maxDepth < 1) {
	ONDEBUG(std::cerr << "ObjectTemplateBodyC::FullName() ");
	cerr << "ERROR: Maximum template depth exceeded in '" << Name() << "' arg '" << it->Name() <<  "'\n";
	ret += it->Name();
      } else {
	if(maxDepth < 9)
	  std::cerr << "ObjectTemplateBodyC::FullName()  WARNING: Maximum template depth nearly exceeded (" << maxDepth <<") in '" << Name() << "' arg '" << it->Name() <<  "'\n";
	ret += it->FullName(templSub,dg,maxDepth-1);
      }
    }
    ret += dg.TextFor('>');
    return ret;
  }
  
  //: Create a new object with subsituted args.
  
  ObjectC ObjectTemplateBodyC::Subst(RCHashC<StringC,ObjectC> &subst) const {
    ONDEBUG(std::cerr << "ObjectTemplateBodyC::Subst(), Called '" << Name() << "' \n");
    if(!subst.IsEmpty() && !argList.List().IsEmpty()) {
      bool doneSub = false;
      ObjectListC ol("TemplateArgList");
      for(ConstDLIterC<ObjectC> it(argList.List());it;it++) {
	ObjectC obj = it->Subst(subst);
	if(obj != *it)
	  doneSub = true;
	ol.Append(obj);
      }
      if(doneSub)
	return ObjectTemplateC(Name(),ol);
    }
    return ObjectTemplateC(const_cast<ObjectTemplateBodyC &>(*this)); // No change.
  }
  
  //// DataTypeC ////////////////////////////////////////////////////////

  //: Constructor.
  
  DataTypeBodyC::DataTypeBodyC(const StringC &nPreQual,const ObjectC &nscopePath,const StringC &nPostQual) 
    : preQual(nPreQual),
      postQual(nPostQual)
  {
    if(!ObjectListC::IsA(nscopePath)) {
      ObjectC ob(nscopePath);
      scopePath = ObjectListC("ScopePath");
      scopePath.Append(ob);
    } else
      scopePath = ObjectListC(nscopePath);
    //cerr << "Got scope path : " << scopePath.Name() << "\n";
    StringC nName;
    if(preQual != ""){
      nName += preQual; 
      nName += ' ';
    }
    nName += scopePath.Name();
    if(postQual != "") {
      nName += ' ';
      nName += postQual;
    }
    Name() = nName;
  }
  
  //: Constructor.
  
  DataTypeBodyC::DataTypeBodyC(const DataTypeC &dt,const StringC &nalias)
    : alias(nalias)
  {
    if(dt.IsValid()) {
      Name() = dt.Name();
      scopePath = dt.ScopePath();
    }
  }
  
  //: Constructor.
  DataTypeBodyC::DataTypeBodyC(const StringC &nname,const ObjectC &nalias) 
    : ObjectBodyC(nname)
  {
    if(nalias.IsValid())
      alias = nalias.Name();
  }

  //: Constructor.
  
  DataTypeBodyC::DataTypeBodyC(const StringC &nname,const StringC &nalias,const ObjectC &nscopePath)
    : ObjectBodyC(nname),
      alias(nalias)
  {
    if(nscopePath.IsValid()) {
      if(ObjectListC::IsA(nscopePath)) {
	scopePath = ObjectListC(nscopePath);
	//cerr << "Got scope path : " << scopePath.Name() << "\n";
      } else
	cerr << "DataTypeBodyC::DataTypeBodyC(), ERROR: not an object list given as scopepath. \n";
    }
  }
  
  //: Resolve data references correctly.
  
  StringC DataTypeBodyC::ActualPath() const {
    //ONDEBUG(std::cerr << "DataTypeBodyC::ActualPath(), Called. for :" << Name() << " \n");
    if(!HasParentScope()) {
      //cerr << "DataTypeBodyC::ActualPath(), No parent scope, can't resolve. \n";
      //return scopePath.Name();
      return StringC("std");
    }
    if(!scopePath.IsValid()) {
      std::cerr << "DataTypeBodyC::ActualPath(), No scopePath, can't resolve. \n";
      return Name();
    }
    RCHashC<StringC,ObjectC> templSub;
    ObjectC ao = const_cast<ScopeBodyC &>(ParentScope()).ResolveName(const_cast<ObjectListC &>(scopePath).List(),templSub);
    if(!ao.IsValid()) {
      std::cerr << "DataTypeBodyC::ActualPath(), Can't resolve : " << PathName(const_cast<ObjectListC &>(scopePath).List()) << "\n";
      return scopePath.Name();
    }
    //ONDEBUG(std::cerr << "Full path found : " << ao.FullPath() << "\n");
    return ao.FullPath();
  }

  //: Get full name of object
  // template args and all.
  
  StringC DataTypeBodyC::FullName(RCHashC<StringC,ObjectC> &templSub,DesciptionGeneratorC &dg,int maxDepth) const {
    //cerr << "Got scopepath for '" << Name() << "' Larg:'" << larg.Name() << "'  ==" << larg.IsVar("templateArgs") <<  "\n";
    StringC ret;
    if(preQual != "") {
      ret += preQual;
      ret += ' ';
    }
    if(scopePath.IsValid()) {
      if(maxDepth < 9) 
	cerr << "DataTypeBodyC::FullName(), WARNING: Maximum template depth nearly exceeded (" << maxDepth <<") in arg '" << Name() <<  "' Type:" << TypeName() <<" \n";
      ret += PathName(const_cast<ObjectListC &>(scopePath).List(),templSub,dg,maxDepth-1);
    } else {
      if(Name() != "..." && Name() != "class") //  && Name() != "void"
	cerr << "DataTypeBodyC::FullName(), WARNING: Invalid scope path for '" << Name() << "' \n";
      ret += Name();
    }
    if(postQual != "") {
      ret += ' ';
      ret += postQual;
    }
    return ret;
  }

  //: Create a new object with subsituted args.
  
  ObjectC DataTypeBodyC::Subst(RCHashC<StringC,ObjectC> &subst) const {
    ONDEBUG(std::cerr << "DataTypeBodyC::Subst(), Called '" << Name() << "' \n");
    if(scopePath.IsValid() && !subst.IsEmpty()) {
      if(scopePath.List().Size() == 1) {
	// Check for a possible short cut....
	if(const_cast<DListC<ObjectC> & > (scopePath.List()).First().Name() == name) {
	  ObjectC *lu = subst.Lookup(Name());
	  if(lu != 0)
	    return *lu;
	  return DataTypeC(const_cast<DataTypeBodyC &>(*this));
	}
      }
      if(!scopePath.List().IsEmpty()) {
	bool doneSub = false;
	ObjectListC ol(StringC("DTtemplateArgs:") + Name());
	for(DLIterC<ObjectC> it(const_cast<ObjectListC &>(scopePath).List());it;it++) {
	  ObjectC obj = it->Subst(subst);
	  ol.Append(obj);
	  if(obj != *it)
	    doneSub = true;
	}
	if(doneSub) // Actually done anything ?
	  return DataTypeC(preQual,ol,postQual);
      }
    }
    return DataTypeC(const_cast<DataTypeBodyC &>(*this));
  }
  
  // ---------------------------------------------------------------
  
  //: Default constructor.
  ClassBodyC::ClassBodyC()
    : curAccess("private")
    {}
  
  //: Constructor.
  ClassBodyC::ClassBodyC(const StringC &nname)
    : ScopeBodyC(nname),
      curAccess("private")
    {
      SetVar("ClassName",nname);
    }
  
  //: Constructor.
  ClassBodyC::ClassBodyC(const StringC &nname,DListC<ObjectC> &contents)
    : ScopeBodyC(nname),
      curAccess("private")
    { 
      SetVar("ClassName",nname);
      AppendList(contents);
    }
  
  //: Constructor.
  
  ClassBodyC::ClassBodyC(const StringC &nname,const ObjectC &contents,const ObjectC &inherit)
    : ScopeBodyC(nname),
      curAccess("private")
  {
    SetVar("ClassName",nname);
    AppendList(contents);
    AppendList(inherit);
  }

  //: Append to list.
  
  void ClassBodyC::Append(ObjectC &obj) { 
    if(!obj.IsValid())
      return ;
    MarkerC aMark(obj);
    if(aMark.IsValid()) {
      curAccess = aMark.Name();
      list.InsLast(obj);
      //ONDEBUG(std::cerr << "ClassBodyC::Append(), Changing access '" << curAccess << "' \n");
      return;
    }
    if(!InheritC::IsA(obj) && !DerivedC::IsA(obj))
      obj.SetVar("access",curAccess);
    ScopeBodyC::Append(obj); 
  }

  //////// ClassTemplateBodyC ///////////////////////////////////////////////////////////

  //: Get full name of object
  // template args and all.
  
  StringC ClassTemplateBodyC::FullName(RCHashC<StringC,ObjectC> &templSub,DesciptionGeneratorC &dg,int maxDepth) const {
    ONDEBUG(std::cerr << "ClassTemplateBodyC::FullName(), Called for " << Name() <<  "\n");
    StringC ret = dg.TextFor(ObjectC(const_cast<ObjectBodyC &>((ObjectBodyC &)*this))).Copy();
    ret += dg.TextFor('<');
    bool first = true;
    for(ConstDLIterC<ObjectC> it(templArgs.List());it;it++) {
      ONDEBUG(std::cerr << "ClassTemplateBodyC::FullName(), Arg='" << it->Name() <<  "' \n");
      if(first)
	first = false;
      else
	ret += dg.TextFor(',');
      // Is template arg a datatype in itself ? or a constant.

      StringC alias;
      if(DataTypeC::IsA(*it)) { // This should be true.
	DataTypeC dt(const_cast<ObjectC &>(*it));
	alias = dt.Alias(); 
      }
      
      if(maxDepth < 1) {
	ONDEBUG(std::cerr << "ClassTemplateBodyC::FullName() ");
	cerr << "ERROR: Maximum template depth exceeded template class arg '" << it->Name() <<  "'\n";
	RavlAssert(0);
	ret += it->Name();
      } else {
	if(maxDepth < 9)
	  std::cerr << "ClassTemplateBodyC::FullName(),  WARNING: Maximum template depth nearly exceeded (" << maxDepth <<") in '" << Name() << "' arg '" << it->Name() <<  "'\n";
	ObjectC obj;
	if(!templSub.Lookup(alias,obj)) {
	  ret += it->FullName(templSub,dg,maxDepth-1);
	} else {
	  ret += obj.FullName(templSub,dg,maxDepth-1);
	  alias = StringC(); // Don't display alias.
	}
      }
      if(!alias.IsEmpty()) {
	ret += ' ';
	ret += alias;
      }
    }
    ret += dg.TextFor('>');
    return ret;
  }

  //: Create a new object with subsituted args.
  
  ObjectC ClassTemplateBodyC::Subst(RCHashC<StringC,ObjectC> &subst) const {
    ONDEBUG(std::cerr << "ClassTemplateBodyC::Subst(), Called '" << Name() << "' \n");
    if(!subst.IsEmpty() && !templArgs.List().IsEmpty()) {
      ObjectListC ol("TemplateArgList");
      bool doneSub = false;
      for(ConstDLIterC<ObjectC> it(templArgs.List());it;it++) {
	ObjectC obj = it->Subst(subst);
	ol.Append(obj);
	if(obj != *it)
	  doneSub = true;
      }
      if(doneSub)
	return ObjectTemplateC(Name(),ol);
    }
    return ClassTemplateC(const_cast<ClassTemplateBodyC &>(*this));
  }
  
  
  //////// MethodBodyC ///////////////////////////////////////////////////////////
  //: Constructor.
  
  MethodBodyC::MethodBodyC(const StringC &nname,const DataTypeC &rt,const ObjectListC &nargs,ObjectC &nquals,bool isConv,bool isPtr)
    : ObjectBodyC(nname),
      retType(rt),
      isConstructor(false),
      isConversion(isConv),
      isPointer(isPtr),
      quals(nquals)
  {
    CopyLineNo(rt);
    IncludeLineNo(nquals);
    // Build full name....
    ObjectListC tl(nargs);
    StringC baseName = Name().Copy();
    SetVar("BaseName",baseName);
    for(DLIterC<ObjectC> it(nargs.List());it;it++) {
      if(!DataTypeC::IsA(it.Data())) {
	cerr << "MethodBodyC::MethodBodyC(), WARNING: Arg not a DataType.  Name:'" << Name() << "' \n";
	continue;
      }
      DataTypeC dt(it.Data());
      args += dt;
    }
    Name() = IndexName();
  }
  
  MethodBodyC::MethodBodyC(const MethodC &meth)
    : ObjectBodyC(meth.Name()),
      retType(meth.ReturnType()),
      args(meth.Args()),
      scopeInfo(meth.ScopeInfo()),
      isConstructor(false),
      isConversion(meth.IsConversion()),
      isPointer(meth.IsPointer()),
      quals(meth.Quals()),
      definition(meth.Definition())
  {
    CopyLineNo(meth);
    SetVar("BaseName",meth.Var("BaseName"));
    SetConstructor(meth.IsConstructor());
  }
  
  //: Rename method.
  
  void MethodBodyC::Rename(const StringC &nname) {
    SetVar("BaseName",nname);
    Name() = IndexName();
  }
  
  //: Generate a name which is suitable for indexing.
  // This is the name without the return type, and paramiters names.
  
  StringC MethodBodyC::IndexName() const {
    bool isFirst = true;
    StringC ret = Var("BaseName");
    if(isConversion) {
      ret += ' ';
      ret += retType.FullName();
    }
    ret += '(';
    if(args.IsEmpty())
      ret += "void"; // Just to normallise everything.
    for(DLIterC<DataTypeC> it(args);it;it++) {
      if(isFirst) {
	isFirst = false;
      } else
	ret += ',';
      ret += it->FullName();;
    }
    ret += ')';
    if(quals.IsValid() && !quals.Name().IsEmpty())
      ret += StringC(' ') + quals.Name(); // Constant stuff.        
    return ret;
  }
  
  //: Get full name of object
  // template args and all.
  
  StringC MethodBodyC::FullName(RCHashC<StringC,ObjectC> &templSub,DesciptionGeneratorC &dg,int maxDepth) const {    
    StringC ret;
    if(IsConversion()) {
      ret += dg.MethodNameText("operator ");
      ret += ReturnType().FullName(templSub,dg,maxDepth);
    } else {
      if(!IsConstructor()) {
	ret += ReturnType().FullName(templSub,dg,maxDepth);
	ret += ' ';
      }
      if(isPointer)
	ret += "(*" ;
      ret += dg.MethodNameText(Var("BaseName"));
      if(isPointer)
	ret += ')';
    }
    ret += dg.TextFor('(');
    bool first = true;
    for(ConstDLIterC<DataTypeC> it(Args());it.IsElm();it.Next()) {
      if(first) 
	first = false;
      else
	ret += dg.TextFor(',');
      ret += it.Data().FullName(templSub,dg,maxDepth);
      if(it.Data().Alias() != "") {
	ret += ' ';
	ret += dg.TextFor(it.Data().Alias());
      }
    }
    ret += dg.TextFor(')');
    if(Quals().IsValid()) {
      ret += ' ';
      ret += Quals().Name();
    }
    
    return ret;
  }

  //: Compose the name as definition
  
  StringC MethodBodyC::DefinitionName(const StringC &aScopeName) {
    StringC ret;
    RCHashC<StringC,ObjectC> templSub;
    DesciptionGeneratorC dg;
    ObjectC scope = GetScope();
    int maxDepth = 20;
    StringC scopeName = aScopeName;
    if(scopeName.IsEmpty()) {
      if(scope.IsValid())
	scopeName = scope.Name() + "::";
    }
    if(IsConversion()) {
      ret += scopeName + "operator ";
      ret += ReturnType().FullName(templSub,dg,maxDepth);
    } else {
      if(!IsConstructor()) {
	ret += ReturnType().FullName(templSub,dg,maxDepth);
	ret += ' ';
      }
      if(isPointer)
	ret += "(*" ;
      ret += scopeName + Var("BaseName");
      if(isPointer)
	ret += ')';
    }
    ret += dg.TextFor('(');
    bool first = true;
    for(ConstDLIterC<DataTypeC> it(Args());it;it++) {
      if(first) 
	first = false;
      else
	ret += dg.TextFor(',');
      ret += it.Data().FullName(templSub,dg,maxDepth);
      StringC alias = it.Data().Alias();
      if(alias != "") {
	ret += ' ';
	if(alias.contains("="))
	  ret += alias.before("=").TopAndTail();
	else
	  ret += alias;
      }
    }
    ret += dg.TextFor(')');
    if(Quals().IsValid()) {
      ret += ' ';
      ret += Quals().Name();
    }
    
    return ret;    
  }

  //: Set constructor flag.
  
  void MethodBodyC::SetConstructor(bool val) {
    isConstructor = val;
    if(val) {
      SetVar("constructor","true");
    } else 
      comment.Vars().Del("constructor");
  }
  
  //: Dump to 'out' in human readable form.
  
  void MethodBodyC::Dump(std::ostream &out,int indent) {
    if(definition.IsValid()) {
      Pad(out,indent+1) << "Def:\n"; 
      definition.Dump(out,indent+2);
    }
    ObjectBodyC::Dump(out,indent);
  }
  
  //////// TypedefBodyC ///////////////////////////////////////////////////////////
  //: Constructor.
  
  TypedefBodyC::TypedefBodyC(const StringC &nname,const DataTypeC &ntype)
    : ObjectBodyC(nname),
      dataType(ntype)
  {
    if(dataType.IsValid()) {
      SetVar("datatype",dataType.FullName());
    } else
      SetVar("datatype","*unknown*");
  }
  
  //: Get full name of object
  // template args and all.
  
  StringC TypedefBodyC::FullName(RCHashC<StringC,ObjectC> &templSub,DesciptionGeneratorC &dg,int maxDepth) const {
    return Name();
  }
  

  //////// VariableBodyC ///////////////////////////////////////////////////////////
  
  //: Constructor.
  
  VariableBodyC::VariableBodyC(const StringC &naname)
    : ObjectBodyC(naname)
  {
    SetVar("datatype","*unknown*");
  }

  //: Constructor.
  
  VariableBodyC::VariableBodyC(ObjectC &ndt,const StringC &nname)
    : ObjectBodyC(nname),
      dt(ndt)
  {
    //cerr << "VariableBodyC::VariableBodyC() Called :'" << Name() <<"'\n";
    if(dt.IsValid()) {
      SetVar("datatype",dt.FullName());
    } else
      SetVar("datatype","*unknown*");
  }
  
  //////// InheritBodyC ///////////////////////////////////////////////////////////
  
  //: Constructor.
  
  InheritBodyC::InheritBodyC(ScopeAccessT nscopeAccess,bool nuseNamespace)
    : ObjectBodyC("Inherit:"),
      scopeAccess(nscopeAccess),
      virt(false),
      resolveFailed(false),
      useNamespace(nuseNamespace)
  {
    switch(nscopeAccess) {
    case SAPrivate:   SetVar("access","private");   break;
    case SAPublic:    SetVar("access","public");    break;
    case SAProtected: SetVar("access","protected"); break;
    }
  }
  
  //: Set the scope definition.
  // Used in parser.
  
  void InheritBodyC::SetScopeDef(const ObjectC &obj) { 
    inheritDef = obj; 
    if(obj.IsValid())
      Name() += obj.Name();
    SetVar("classname",obj.Name());
    //Name() = obj.Name();
  }
  
  //: Scope definition as a string.
  
  StringC InheritBodyC::ScopeDef() {
    if(!inheritDef.IsValid())
      return "(NULL)";
    if(!ObjectListC::IsA(inheritDef))
      return inheritDef.Name();
    ObjectListC ol(inheritDef);
    return PathName(ol.List());
  }

  //: Scope definition as a string.
  
  StringC InheritBodyC::ScopeDef(RCHashC<StringC,ObjectC> &ts) const {
    if(!inheritDef.IsValid())
      return "(NULL)";
    if(!ObjectListC::IsA(inheritDef))
      return inheritDef.Name();
    ObjectListC ol(inheritDef);
    return PathName(ol.List(),ts);
  }


  //: Object we inherit from;
  
  ScopeC &InheritBodyC::From() {
    if(resolveFailed)
      return inheritFrom;
    if(!inheritFrom.IsValid()) 
      Resolve();
    return inheritFrom;
  }

  //: Attempt to resolve parent class.
  
  bool InheritBodyC::Resolve() {
    ONDEBUG(std::cerr << "InheritBodyC::Resolve(), " << Name() << " templSub=" << templSub << "\n");
    if(inheritFrom.IsValid()) 
      return true;
    if(!HasParentScope()) {
      std::cerr << "InheritBodyC::Resolve(), Failed, no parent scope. \n";
      SetVar("resolveFailed","1");
      return false;
    }
    DListC<ObjectC> path;
    if(ObjectListC::IsA(inheritDef)) {
      ObjectListC ol(inheritDef);
      path = ol.List();
    } else
      path.InsFirst(inheritDef);
    
    ScopeC ps = ParentScope(); // Get scope we're for.
    if(ps.IsValid()) {
      if(ps.HasParentScope())
	ps = ps.ParentScope(); // Then get the scope of the object to inherit into.
      else
	ps.Invalidate();
    }
    if(!ps.IsValid()) {
      std::cerr << "InheritBodyC::Resolve(), WARNING: Out of parent scopes can't resolve '" << PathName(path) << "' \n";
      SetVar("resolveFailed","1");
      resolveFailed = true;
      return false; 
    }
    ObjectC rs=ps.ResolveName(path,templSub,
			      !useNamespace); // Don't use inheritance to resolve namespaces.
    if(!rs.IsValid()) {
      std::cerr << "Can't resolve name '" << PathName(path) << "' \n";
      ONDEBUG(std::cerr << " Parent scope '" << ParentScope().Name() << "'. \n");
      SetVar("resolveFailed","1");
      resolveFailed = true;
      return false;
    }
    if(!ScopeC::IsA(rs)) {
      std::cerr << "Inherited Object is not a scope. '" << rs.Name() << "'  \n";
      SetVar("resolveFailed","1");
      resolveFailed = true;
      return false;
    }
    ScopeC as(rs);
    inheritFrom = as;
    DerivedC deriveMark(scopeAccess,ParentScope().PathList());
    inheritFrom.Append(deriveMark);
    ONDEBUG(std::cerr << "InheritBodyC::Resolve(), '" << Name() << "' found '" << inheritFrom.Name() <<  "'\n");
    return true;
  }

  //: Get full name of object
  // template args and all.
  
  StringC InheritBodyC::FullName(RCHashC<StringC,ObjectC> &allTemplSub,DesciptionGeneratorC &dg,int maxDepth) const {
    ScopeC fromScope = const_cast<InheritBodyC &>(*this).From();
    RCHashC<StringC,ObjectC> lsub = templSub;
    ONDEBUG(std::cerr << "InheritBodyC::FullName(), Subst=" << templSub << " ScopeDef='" <<const_cast<InheritBodyC &>(*this).ScopeDef(lsub) << "' \n");
    if(!fromScope.IsValid())
      return const_cast<InheritBodyC &>(*this).ScopeDef(lsub);
    ONDEBUG(std::cerr << "InheritBodyC::FullName(), Path='" << dg.TextFor(fromScope) << "' \n");
    return fromScope.FullName(lsub,dg,maxDepth);
  }
  
  
  //////// DerivedBodyC ///////////////////////////////////////////////////////////
  
  //: Constructor.
  
  DerivedBodyC::DerivedBodyC(ScopeAccessT nscopeAccess,const ObjectListC &npath)
    : scopeAccess(nscopeAccess),
      path(npath)  
  {
    Name() = StringC("Derived:") + PathName(path.List());
    switch(scopeAccess) {
    case SAPrivate:   SetVar("access","private"); break;
    case SAPublic:    SetVar("access","public"); break;
    case SAProtected: SetVar("access","protected"); break;
    }
  }

  //: Get handle to derived object.
  // (Slowish)
  
  ObjectC DerivedBodyC::DerivedObj() {
    ObjectC ret;
    if(!HasParentScope()) {
      std::cerr << "DerivedBodyC::DerivedObj(), Failed, no parent scope. \n";
      return ret;
    }
    RCHashC<StringC,ObjectC> templSub;
    ret = ParentScope().ResolveName(path.List(),templSub);
    if(!ret.IsValid()) {
      ONDEBUG(std::cerr << "DerivedBodyC::DerivedObj() ");
      std::cerr << "ERROR: Failed to find parent object '" << PathName(path.List()) <<"' (" << path.List().Last().BaseName() << ") from " << Name() <<" \n";
    }
    return ret;
  }

  //////// EnumBodyC ///////////////////////////////////////////////////////////
  
  //: Constructor.
  
  EnumBodyC::EnumBodyC(const StringC &nname,const ObjectListC &nvalues) 
    : ObjectListBodyC(nname)
  {
    //ONDEBUG(std::cerr << "Got enum :" << nname << "\n");
    if(nvalues.IsValid()) 
      list = nvalues.List();
  }
  
  EnumBodyC::EnumBodyC(const StringC &nname) 
    : ObjectListBodyC(nname)
  {
    //ONDEBUG(std::cerr << "Got enum :" << nname << "\n");
  }

}
