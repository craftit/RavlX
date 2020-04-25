// This file is part of CxxDoc, The RAVL C++ Documentation tool 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU General 
// Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlCxxDoc
//! file="Ravl/SourceTools/CxxDoc/Object.cc"

#include "Ravl/CxxDoc/Object.hh"
#include "Ravl/CxxDoc/CxxScope.hh"
#include "Ravl/CxxDoc/Strings.hh"
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

#define USE_STRING_CACHE 1

namespace RavlCxxDocN
{
  using namespace RavlN;

#if RAVL_VISUALCPP_NAMESPACE_BUG  
  using RavlN::StringC;
  using RavlN::RCHashC;
  using RavlN::HashIterC;
  using RavlN::DLIterC;
#endif
  
  static int anonObjectCount = 1;
  RCHashC<StringC,ObjectC> emptyTemplSubst; // Empty template subsitution.
  DesciptionGeneratorC defaultDescGen; // Default do nothing description generator.

  //: Set a global variable.

  static HashC<StringC,StringC> strCache;
  
  void CommentInfoC::SetVar(const StringC &nm,const StringC &value) { 
#if USE_STRING_CACHE
    StringC vName,vValue;
    if(!strCache.Lookup(nm,vName)) {
      strCache[nm] = nm; 
      vName = nm;
    }
    if(value.length() < 10) {
      if(!strCache.Lookup(value,vValue)) {
	strCache[value] = value;
	vValue = value;
      }
      vars.Insert(vName,vValue);
    } else
      vars.Insert(vName,value); 
#else
    vars.Insert(nm,value); 
#endif
  }
  
  //: Render an object name.
  
  StringC DesciptionGeneratorC::TextFor(const ObjectC &obj) {
    return obj.Name();
  }
  
  //: Render a string 'as is'.
  
  StringC DesciptionGeneratorC::TextFor(const StringC &obj) {
    return obj;
  }
  
  //: Render a character.
  // Typical used for '<' '>' and the like..
  StringC DesciptionGeneratorC::TextFor(char let) {
    char arr[2];
    arr[0] = let;
    arr[1] = 0;
    return StringC(arr);
  }

  //: Render a method name appropriatly. (e.g. bold.)
  
  StringC DesciptionGeneratorC::MethodNameText(const StringC &obj) {
    return obj;
  }
  
  //: Constructor.
  
  ObjectBodyC::ObjectBodyC()
    : name(StringC("anon2?") + StringC(anonObjectCount++)),
      parent(0),
      startLineno(-1),
      endLineno(-1)
  {}
  
  //: Constructor.
  
  ObjectBodyC::ObjectBodyC(const StringC &nname)
    : name(nname),
      parent(0),
      startLineno(-1),
      endLineno(-1)
  {}
  
  //: Dump to 'out' in human readable form.
  
  void ObjectBodyC::Dump(std::ostream &out,int indent) {
    Pad(out,indent) << "Object: '" << name << "'  Type:" << TypeName() << "\n";
    for(HashIterC<StringC,StringC> it(comment.Vars());it.IsElm();it.Next())
      Pad(out,indent+1) << "(" << it.Key() << "=" << it.Data() <<")\n";
    for(HashIterC<StringC,StringC> it(comment.Locals());it.IsElm();it.Next())
      Pad(out,indent+1) << "[" << it.Key() << "=" << it.Data() <<"]\n";
  }
  
  //: Get full path to object.
  
  StringC ObjectBodyC::FullPath() const  { 
    if(!HasParentScope())
      return StringC("::");
    if(ParentScope().HasParentScope()) {
      //if(const_cast<CommentInfoC &>(comment).Vars().IsElm("templateArgs"))
      //	return ParentScope().FullPath() + "::" + Name()  + '<' + const_cast<CommentInfoC &>(comment).Vars()["templateArgs"] + '>';
      return ParentScope().FullPath() + "::" + Name();
    }
    //    if(const_cast<CommentInfoC &>(comment).Vars().IsElm("templateArgs"))
    //      return Name() + '<' + const_cast<CommentInfoC &>(comment).Vars()["templateArgs"] + '>';
    return Name();
  }
  
  //: Put padding spaces in.
  
  std::ostream & ObjectBodyC::Pad(std::ostream &out,int indent) {
    for(int i= 0;i <indent;i++)
      out << ' ';
    return out;
  }

  //: Set comment information.
  
  void ObjectBodyC::SetComment(const CommentInfoC &cinf) {
    //cerr << "Setting comment '" << Name() << "' from '" << comment.Header() << "'\n";
    //cerr << "Setting comment '" << Name() << "' to '" << cinf.Header() << "'\n";
    comment = cinf;
    comment.Header() = comment.Header().Copy();
    comment.Text() = comment.Text().Copy();
    SetVar(STR(brief),comment.Header());
    SetVar(STR(detail),comment.Text());
  }
  
  //: Update the comments for the object.
  
  void ObjectBodyC::UpdateComment(CommentInfoC &cinf) {    
    //cerr << "Updating comment '" << Name() << "' from '" << comment.Header() << "'\n";
    //cerr << "Updating comment '" << Name() << "' to '" << cinf.Header() << "'\n";
    comment.SetStartLine(cinf.StartLine());
    comment.SetEndLine(cinf.EndLine());
    comment.Vars().Add(cinf.Vars());
    comment.Header() = cinf.Header().Copy();
    comment.Text() = cinf.Text().Copy();
    comment.Locals().Data().Add(cinf.Locals().Data());
    SetVar(STR(brief),comment.Header());
    SetVar(STR(detail),comment.Text());
  }

  //: Set scope for object.
  
  ObjectC ObjectBodyC::GetScope() 
  { return ObjectC(); }

  //: Setup parent scope.
  
  void ObjectBodyC::SetParentScope(ScopeBodyC *ns) { 
    RavlAssert((dynamic_cast<ScopeBodyC *>((ObjectBodyC*) ns) != 0) || (ns == 0));
    parent = ns; 
  }

  // : Generate path list to this object.
  
  ObjectListC ObjectBodyC::PathList() const {
    ObjectListC ret;
    if(!HasParentScope()) {
      ret = ObjectListC(STR(PathList));
      //ObjectC tmp("::");
      //      ret.Append(tmp);
    } else {
      ret = ParentScope().PathList();
      ObjectC tmp(Name());
      ret.Append(tmp);
    }
    return ret;
  }
  
  //: Output to stream.
  std::ostream &operator<<(std::ostream &s,const ObjectC &out) { 
    if(!out.IsValid())
      s << "(NULL)";
    else
      s << out.Name();
    return s; 
  }

  //: Output to stream.
  // Not implemented.
  
  BinOStreamC &operator<<(BinOStreamC &strm,const ObjectC &out) {
    RavlAssertMsg(0,"not implemented");
    return strm;
  }
  
  //: Input from stream.
  // Not implemented.
  
  BinIStreamC &operator>>(BinIStreamC &strm,ObjectC &out) {
    RavlAssertMsg(0,"not implemented");
    return strm;
  }
  
  //: Copy line no from object.
  void ObjectBodyC::CopyLineNo(const ObjectC &ob) 
  { CopyLineNo(ob.Body()); }
  
  //: Include lines into object.
  void ObjectBodyC::IncludeLineNo(const ObjectC &ob)
  { IncludeLineNo(ob.Body()); }
  
  //: Get full name of object
  // template args and all.
  
  StringC ObjectBodyC::FullName(RCHashC<StringC,ObjectC> &templSub,DesciptionGeneratorC &dg,int maxDepth) const {
    ObjectC *lu = templSub.Lookup(Name());
    if(lu == 0) {
      return dg.TextFor(ObjectC(const_cast<ObjectBodyC &>(*this)));
    }
    if(maxDepth < 1) {
      ONDEBUG(std::cerr << "ObjectBodyC::FullName() ");
      std::cerr << "ERROR: Maximum template depth exceeded in '" << Name() <<"' -> '" << lu->Name() << "'\n";
      return lu->Name();
    }
    return lu->FullName(emptyTemplSubst,dg,maxDepth-1); // Only do substition once!
  }

  //: Create a new object with subsituted args.
  
  ObjectC ObjectBodyC::Subst(RCHashC<StringC,ObjectC> &subst) const {
    ONDEBUG(std::cerr << "ObjectBodyC::Subst(), Called '" << Name() << "' \n");
    ObjectC *lu = subst.Lookup(Name());
    if(lu != 0)
      return *lu;
    return ObjectC(const_cast<ObjectBodyC &>(*this));
  }

  //: Access arg list.
  ObjectListC &ObjectBodyC::TemplArgList() { 
    RavlAssert(0); // Abstract function.
    static ObjectListC dummy;
    return dummy; // None, so return an empty dummy.
  }
  
  ///// ObjectC //////////////////////////////////////////////////

  // : Generate path list to this object.
  
  ObjectListC ObjectC::PathList() const
  { return Body().PathList(); }
  
  ///// ObjectListBodyC //////////////////////////////////////////////////
  
  //: Dump to 'out' in human readable form.
  
  void ObjectListBodyC::Dump(std::ostream &out,int indent) {
    Pad(out,indent) << "List: '" << name << "'  Type:" << TypeName() << "\n";
    {
      for(HashIterC<StringC,StringC> it(comment.Vars());it.IsElm();it.Next())
	Pad(out,indent+1) << "(" << it.Key() << "=" << it.Data() <<")\n";
      for(HashIterC<StringC,StringC> it(comment.Locals().Data());it.IsElm();it.Next())
	Pad(out,indent+1) << "[" << it.Key() << "=" << it.Data() <<"]\n";
    }
    for(DLIterC<ObjectC> it(list);it;it++) {
      if(!it.Data().IsValid()) {
	Pad(out,indent+2) << "Invalid ref.\n";
	continue;
      }
      it.Data().Dump(out,indent+2);
    }
  }
  
  //: Append to list.
  
  void ObjectListBodyC::Append(ObjectC &obj) { 
    if(!obj.IsValid())
      return ; // Can't append an invalid handle.
    if(ObjectListC::IsA(obj)) {
      ObjectListC ol(obj);
      if(ol.IsWrapper()) {
	for(DLIterC<ObjectC> it(ol.List());it.IsElm();it.Next())
	  List().InsLast(it.Data());
      } else
	List().InsLast(obj); 
    } else
      List().InsLast(obj); 
  }
  
  //: Append list to list one.
  
  void ObjectListBodyC::AppendList(DListC<ObjectC> &contents) {
    for(DLIterC<ObjectC> it(contents);it.IsElm();it.Next()) 
      Append(it.Data());
  }
  
  //: Append list to list one.
  
  void ObjectListBodyC::AppendList(const ObjectC &contents) {
    ObjectListC ol(contents);
    AppendList(ol.List());
  }
  
  //: Set variable in all immediate children.
  
  void ObjectListBodyC::SetAll(const StringC &var,const StringC &dat) {
    for(DLIterC<ObjectC> it(list);it;it++) {
      // Scopes can be merged from all over the place so set
      // the variable in the members of the scope too.
      if(ScopeC::IsA(it.Data()) &&  StringC("class") != it.Data().TypeName()) {
	ObjectListC sl(it.Data());
	sl.SetAll(var,dat);
      }
      it.Data().SetVar(var,dat);
    }
  }
  
  //: Find named child.
  // returns an invalid handle if not found.

  ObjectC ObjectListBodyC::Find(const StringC &aname) {
    for(DLIterC<ObjectC> it(list);it;it++) {
      if(it->Name() == aname)
	return *it;
    }
    return ObjectC();
  }
  
}

