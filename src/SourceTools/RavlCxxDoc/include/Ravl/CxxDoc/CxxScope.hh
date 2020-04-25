// This file is part of CxxDoc, The RAVL C++ Documentation tool 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU General 
// Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
#ifndef RAVLCXXDOC_SCOPE_HEADER
#define RAVLCXXDOC_SCOPE_HEADER 1
//////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! userlevel=Normal
//! docentry="Ravl.API.Source Tools.CxxDoc.Internal"
//! lib=RavlCxxDoc
//! author="Charles Galambos"
//! date="08/12/1999"
//! file="Ravl/SourceTools/CxxDoc/CxxScope.hh"

#include "Ravl/CxxDoc/Object.hh"

namespace RavlCxxDocN {
  using namespace RavlN;
  
  class ScopeC ;
  
  /////////////////////////////////////////////////
  
  //! userlevel=Develop
  //: Scoped object.  like a class, struct, or namespace.
  
  class ScopeBodyC 
    : public ObjectListBodyC
  {
  public:
    ScopeBodyC()
    {}
    //: Default constructor.
    
    ScopeBodyC(const StringC &nname)
      : ObjectListBodyC(nname)
    {}
    //: Constructor.
    
    ScopeBodyC(const StringC &nname,const ObjectC &contents);
    //: Constructor.
    
    ScopeBodyC(const StringC &nname,DListC<ObjectC> &contents)
      : ObjectListBodyC(nname,contents)
    {}
    //: Constructor.

    virtual const char *TypeName() const 
    { return "namespace"; }
    //: Get name of object type.
    
    virtual void Append(ObjectC &obj);
    //: Append to scope.
   
    ObjectC &Lookup(const StringC &name)
    { return tab[name]; }
    //: Lookup object.
    
    bool LookupI(const StringC &name,ObjectC &nxt,bool useInherit,HSetC<ScopeC> &done);
    //: Lookup name allowing for inheritance.
    // returns true if object has been found.
    
    bool Merge(const ScopeC &oth);
    //: Merge this scope with another.

    bool Merge(const ObjectC &oth);
    //: Merge this scope with another.
    
    ObjectC ResolveName(const StringC &str,bool useInherit = true);
    //: Resolve a path
    
    ObjectC ResolveName(DListC<ObjectC> path,RCHashC<StringC,ObjectC> &tempTab,bool useInherit = true);
    //: Resolve a path
    // templSub contains any template paramiter subsitutions found when 
    // resolving the path. 
    
    static bool MatchTemplateArgs(ClassTemplateC &tc,ObjectC &pathObj,RCHashC<StringC,ObjectC> &templTab);
    //: Match template arguments.
    // puts the results into templTab.
    
    ScopeC RootScope();
    //: Get the root scope.
    
    DListC<ObjectC> &Uses() 
    { return uses; }
    //: Access inherit from list.

    void Resolve();
    //: Resolve links in input data.
    
  protected:
    DListC<ObjectC> uses; 
    // Actually set of other scopes, used by this one.
    // its the inherit list for classes.
    
    HashC<StringC,ObjectC> tab; // Mapping of names to object definitions.
  };
  
  //! userlevel=Normal
  //: Scoped object.  like a class, struct, or namespace.
  
  class ScopeC 
    : public ObjectListC 
  {
  public:
    ScopeC()
    {}
    //: Default Constructor.
    // creates an invalid object.
    
    ScopeC(const StringC &nname)
      : ObjectListC(*new ScopeBodyC(nname))
    {}
    //: Constructor.

    ScopeC(const StringC &nname,DListC<ObjectC> &contents)
      : ObjectListC(*new ScopeBodyC(nname,contents))
    {}
    //: Constructor.

    ScopeC(const StringC &nname,ObjectC &contents)
      : ObjectListC(*new ScopeBodyC(nname,contents))
    {}
    //: Constructor.
    
    static bool IsA(const ObjectC &obj)
    { return dynamic_cast<const ScopeBodyC *>(&obj.Body()) != 0; }
    //: Test if 'obj' is a ScopeC...
    
    ScopeC(ObjectC &obj)
      : ObjectListC(dynamic_cast<const ScopeBodyC *>(BodyPtr(obj)))
    {}
    //: Base class constructor.
    
    ScopeC(ScopeBodyC &bod)
      : ObjectListC(bod)
    {}
    //: Body constructor.
    // Really should be protected...
  protected:
    ScopeC(const ScopeBodyC *bod)
      : ObjectListC(bod)
    {}
    //: Body constructor.
    // Really should be protected...
    
    ScopeBodyC &Body() 
    { return dynamic_cast<ScopeBodyC &>(ObjectC::Body()); }
    //: Access body.

    const ScopeBodyC &Body() const
    { return dynamic_cast<const ScopeBodyC &>(ObjectC::Body()); }
    //: Access body.

    
  public:
    ObjectC &Lookup(const StringC &name) 
    { return Body().Lookup(name); }
    //: Append to list.
    
    bool LookupI(const StringC &name,ObjectC &nxt,bool useInherit,HSetC<ScopeC> &done)
    { return Body().LookupI(name,nxt,useInherit,done); }
    //: Lookup name allowing for inheritance.
    //!param: name - Name of object to search for.
    //!param: nxt - Variable in which to store results.
    //!param: useInherit - Use inhertance ?
    //!param: done - Hash set of scopes that have been visited.
    //!return: True if named object is found.
    
    bool LookupI(const StringC &name,ObjectC &nxt,bool useInherit = true) { 
      HSetC<ScopeC> done; // Create table of checked scopes.
      return Body().LookupI(name,nxt,useInherit,done); 
    }
    //: Lookup name allowing for inheritance.
    // returns true if object has been found.
    //!param: name - Name of object to search for.
    //!param: nxt - Variable in which to store results.
    //!param: useInherit - Use inhertance ?
    //!return: True if named object is found.
    
    DListC<ObjectC> &Uses() 
    { return Body().Uses(); }
    //: Access inherit from list.
    
    bool Merge(const ScopeC &oth)
    { return Body().Merge(oth); }
    //: Merge this scope with another.
    
    bool Merge(const ObjectC &oth)
    { return Body().Merge(oth); }
    //: Merge this scope with another.
    
    ObjectC ResolveName(const StringC &str,bool useInherit = true)
    { return Body().ResolveName(str,useInherit); }
    //: Resolve a name.
    
    ObjectC ResolveName(DListC<ObjectC> path,RCHashC<StringC,ObjectC> &templSub,bool useInherit = true)
    { return Body().ResolveName(path,templSub,useInherit); }
    //: Resolve a name.
    // templSub contains any template paramiter subsitutions found when 
    // resolving the path. 
    
    void Resolve()
    { Body().Resolve(); }
    //: Resolve links in input data.
    
    friend class ScopeBodyC;
  };
  
  //////////////////////////////////////////////////
  
  //! userlevel=Normal
  //: Iterator through all objects in the inheritance hirachy.
  
  class InheritIterC {
  public:
    InheritIterC(ObjectC &start,ScopeAccessT nIncAccess =  SAProtected,bool localOnly = false);
    //: Constructor.
    // start = inital context. <br>
    // nIncAccess = Access level to use. i.e. SAPublic means only look at the public interface. <br>
    // if localOnly is true don't look at inherited scopes.
    
    bool Next();
    //: Goto next.
    // Returns true if there is a next element.
    
    void operator++(int)
    { Next(); }
    //: Goto next object.
    
    ObjectC &Data() 
    { return at.Data(); }
    //: Get current object.

    const ObjectC &Data() const
    { return at.Data(); }
    //: Get current object.
    
    ObjectC &operator*()
    { return at.Data(); }
    //: Get current object.
    
    const ObjectC &operator*() const
    { return at.Data(); }
    //: Get current object.

    ObjectC *operator->()
    { return &at.Data(); }
    //: Get current object.
    
    const ObjectC *operator->() const
    { return &at.Data(); }
    //: Get current object.
    
    bool IsElm() const 
    { return at.IsElm(); }
    //: Is element in list ?
    
    operator bool() const
    { return at.IsElm(); }
    //: Is element in list ?
    
    ScopeC &CurrentScope()
    { return currentScope; }
    //: Get current scope.

    RCHashC<StringC,ObjectC> &TemplateSubs()
    { return currentTSubs; }
    //: Access current template subsitutions.
    
  protected:
    typedef Tuple2C<ObjectC,RCHashC<StringC,ObjectC> > InstanceDefT;
    
    bool NextValid();
    //: Goto next valid element.
    
    bool ListNext(InstanceDefT &start);
    //: Goto next valid element.

    bool localOnly;               // Restrict search to local scope.
    DLIterC<ObjectC> at;          // Iterate objects in current scope.
    DListC<InstanceDefT> toDo;    // Scopes yet to be processed.
    HSetC<InstanceDefT> done;     // Scopes that have been handled already.
    ScopeC currentScope;    // Info about current scope.
    RCHashC<StringC,ObjectC> currentTSubs; // current template substitutions.
    ScopeAccessT incAccess;
  };

}

#endif
