// This file is part of CxxDoc, The RAVL C++ Documentation tool 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU General 
// Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
#ifndef RAVLCXXDOC_CXXELEMENTS_HEADER
#define RAVLCXXDOC_CXXELEMENTS_HEADER 1
//////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! userlevel=Normal
//! docentry="Ravl.API.Source Tools.CxxDoc.Internal"
//! lib=RavlCxxDoc
//! author="Charles Galambos"
//! date="08/12/1999"
//! file="Ravl/SourceTools/CxxDoc/CxxElements.hh"

#include "Ravl/CxxDoc/Object.hh"
#include "Ravl/CxxDoc/CxxScope.hh"

//: C++ parser's namespace.
// This namespace contains the classes that make up the parser, the parse tree 
// and the C++ documenation system.

namespace RavlCxxDocN {
  
  ////////////////////////////////////////////////
  //! userlevel=Develop
  //: Object with template arg specification.
  
  class ObjectTemplateBodyC
    : public ObjectBodyC
  {
  public:
    ObjectTemplateBodyC()
    {}
    //: Default constructor.

    ObjectTemplateBodyC(const StringC &name,const ObjectC &argLst);
    //: Constructor.
    
    ObjectListC &TemplArgList()
    { return argList; }
    //: Access arg list.
    
    virtual bool IsTemplate() const
    { return true; }
    //: Is this a templated object ?
    
    virtual StringC FullName(RCHashC<StringC,ObjectC> &templSub,DesciptionGeneratorC &dg = defaultDescGen,int maxDepth = 60) const;
    //: Get full name of object
    // template args and all.
    
    virtual ObjectC Subst(RCHashC<StringC,ObjectC> &subst) const;
    //: Create a new object with subsituted args.
    
  protected:
    ObjectListC argList;
  };

  /////////////////////////////////////////////////
  //! userlevel=Develop
  //: Object with template arg specification.
  
  class ObjectTemplateC
    : public ObjectC
  {
  public:
    ObjectTemplateC()
    {}
    //: Default constructor.
    // creates an invalid handle.
    
    ObjectTemplateC(const StringC &nname,const ObjectC &argLst)
      : ObjectC(*new ObjectTemplateBodyC(nname,argLst))
    {}
    //: Constructor.
    
    static bool IsA(const ObjectC &obj)
    { return dynamic_cast<const ObjectTemplateBodyC *>(&obj.Body()) != 0; }
    //: Is this a Object Template.
    
    ObjectTemplateC(const ObjectC &obj)
      : ObjectC(dynamic_cast<const ObjectTemplateBodyC *>(BodyPtr(obj)))
    {}
    //: Base class constructor.
    
  protected:
    ObjectTemplateC(ObjectTemplateBodyC &bod)
      : ObjectC(bod)
    {}
    //: Body constructor.
    
    ObjectTemplateC(const ObjectTemplateBodyC *bod)
      : ObjectC(bod)
    {}
    //: Body constructor.
    
    ObjectTemplateBodyC &Body() 
    { return dynamic_cast<ObjectTemplateBodyC &>(ObjectC::Body()); }
    //: Access body.
    
    const ObjectTemplateBodyC &Body() const
    { return dynamic_cast<const ObjectTemplateBodyC &>(ObjectC::Body()); }
    //: Access body.
    
  public:
    
    friend class ObjectTemplateBodyC;
  };
  
  
  class DataTypeC;
  
  /////////////////////////////////////////////////
  //! userlevel=Develop
  //: DataType object.
  // Data type.
  
  class DataTypeBodyC 
    : public ObjectBodyC
  {
  public:
    DataTypeBodyC()
    {}
    //: Default constructor.
    
    DataTypeBodyC(const StringC &nname,const StringC &nalias = StringC(""),const ObjectC &nscopePath = ObjectC());
    //: Constructor.
    
    DataTypeBodyC(const StringC &preQual,const ObjectC &nscopePath,const StringC &postQual);
    //: Constructor.
    
    DataTypeBodyC(const DataTypeC &dt,const StringC &nalias);
    //: Constructor.
    
    DataTypeBodyC(const StringC &nname,const ObjectC &nalias);
    //: Constructor.

    virtual const char *TypeName() const 
    { return "datatype"; }
    //: Get name of object type.
    
    StringC &Alias() { return alias; }
    //: Get alias for type.

    const StringC &Alias() const { return alias; }
    //: Get alias for type.
    
    void SetAlias(const StringC &str) 
    { alias = str; }
    //: Get alias for type.
    
    virtual StringC ActualPath() const;
    //: Resolve data references correctly.
    
    void SetScopePath(ObjectListC &sp)
    { scopePath = sp; } 
    //: Set the path to the scope.
    
    ObjectListC &ScopePath()
    { return scopePath; }
    //: Access scope path.
    
    const ObjectListC &ScopePath() const
    { return scopePath; }
    //: Access scope path.

    virtual ObjectC GetScope() 
    { return scopePath; }
    //: Set scope for object.
    
    virtual StringC FullName(RCHashC<StringC,ObjectC> &templSub,DesciptionGeneratorC &dg = defaultDescGen,int maxDepth = 60) const;
    //: Get full name of object
    // template args and all.
    
    virtual ObjectC Subst(RCHashC<StringC,ObjectC> &subst) const;
    //: Create a new object with subsituted args.
    
    void SetPreQual(const StringC &pq) { 
      if(preQual != "")
	preQual = pq + ' ' + preQual;
      else
	preQual = pq;
      if(pq != "")
	Name() = pq + ' ' + Name();
    }
    //: Set pre qualifiers (const, virtual etc..)

    bool IsTemplated() const {
      if(!scopePath.IsValid())
	return false;
      if(scopePath.List().IsEmpty())
	return false;
      return scopePath.List().Last().IsTemplate();
    }
    //: Is type templated ?
    
    
  protected:
    StringC alias;   // Name associated with type (as in function args.)
    
    StringC preQual;
    ObjectListC scopePath; // If scope resolved path, its stored here.
    StringC postQual;
  };
  
  //! userlevel=Normal
  //: DataTyped object.  like a class, struct, or namespace.
  
  class DataTypeC 
    : public ObjectC 
  {
  public:
    DataTypeC()
    {}
    //: Default Constructor.
    // creates an invalid object.
    
    DataTypeC(const StringC &nname,const StringC &nalias = StringC(""),const ObjectC &nscopePath = ObjectC())
      : ObjectC(*new DataTypeBodyC(nname,nalias,nscopePath))
    {}
    //: Constructor.
    
    DataTypeC(const StringC &preQual,const ObjectC &nscopePath,const StringC &postQual)
      : ObjectC(*new DataTypeBodyC(preQual,nscopePath,postQual))
    {}
    //: Constructor.
    
    DataTypeC(const DataTypeC &dt,const StringC &nalias)
      : ObjectC(*new DataTypeBodyC(dt,nalias))
    {}
    //: Constructor.
    
    DataTypeC(const StringC &dt,const ObjectC &nalias)
      : ObjectC(*new DataTypeBodyC(dt,nalias))
    {}
    //: Constructor.
    
    DataTypeC(ObjectC &obj)
      : ObjectC(dynamic_cast<DataTypeBodyC *>(BodyPtr(obj)))
    {}
    //: Base class constructor.
    
    static bool IsA(const ObjectC &obj)
    { return dynamic_cast<const DataTypeBodyC *>(&obj.Body()) != 0; }
    //: Test if 'obj' is a ScopeC...
    
  protected:
    DataTypeC(ObjectBodyC &bod)
      : ObjectC(bod)
    {}
    //: Body constructor.
    
    DataTypeC(const ObjectBodyC *bod)
      : ObjectC(bod)
    {}
    //: Body constructor.
    
    DataTypeBodyC &Body() 
    { return dynamic_cast<DataTypeBodyC &>(ObjectC::Body()); }
    //: Access body.
    
    const DataTypeBodyC &Body() const
    { return dynamic_cast<const DataTypeBodyC &>(ObjectC::Body()); }
    //: Access body.
    
  public:
    StringC &Alias() 
    { return Body().Alias(); }
    //: Get alias for type.

    const StringC &Alias() const 
    { return Body().Alias(); }
    //: Get alias for type.
    
    void SetAlias(const StringC &str) 
    { Body().SetAlias(str); }
    //: Get alias for type.
    
    void SetScopePath(ObjectListC &sp)
    { Body().SetScopePath(sp); }
    //: Set the path to the scope.
    
    ObjectListC &ScopePath()
    { return Body().ScopePath(); }
    //: Access scope path.

    const ObjectListC &ScopePath() const
    { return Body().ScopePath(); }
    //: Access scope path.
    
    void SetPreQual(const StringC &pq)
    { Body().SetPreQual(pq); }
    //: Set pre qualifiers (const, virtual etc..)

    bool IsTemplated() const
    { return Body().IsTemplated(); }
    //: Is type templated ?
    
    friend class DataTypeBodyC;
  };
  
  /////////////////////////////////////////////////
  // Object list

  class ClassC;
  
  //! userlevel=Develop  
  //: Class storage object.
  // This includes structs and unions. 
  
  class ClassBodyC 
    : public ScopeBodyC
  {
  public:
    ClassBodyC();
    //: Default constructor.
    
    ClassBodyC(const StringC &nname);
    //: Constructor.
    
    ClassBodyC(const StringC &nname,DListC<ObjectC> &contents);
    //: Constructor.
    
    ClassBodyC(const StringC &nname,const ObjectC &contents,const ObjectC &inherit);
    //: Constructor.
    
    virtual const char *TypeName() const 
    { return "class"; }
    //: Get name of object type.
    
    virtual void Append(ObjectC &obj);
    //: Append to list.
    
    DListC<ScopeC> &Inherit()
    { return inherit; }
    //: List of classes inherited from.
    
  protected:
    DListC<ScopeC> inherit;    
    StringC curAccess;  // Used in building class description to hold current access type.
  };
  
  //! userlevel=Normal
  //: Class storage object.
  // This includes structs and unions. 
  
  class ClassC 
    : public ScopeC 
  {
  public:
    ClassC()
    {}
    //: Default Constructor.
    // creates an invalid object.
    
    ClassC(const StringC &nname)
      : ScopeC(*new ClassBodyC(nname))
    {}
    //: Constructor.

    ClassC(const StringC &nname,DListC<ObjectC> &contents)
      : ScopeC(*new ClassBodyC(nname,contents))
    {}
    //: Constructor.

    ClassC(const StringC &nname,const ObjectC &contents,const ObjectC &inherit)
      : ScopeC(*new ClassBodyC(nname,contents,inherit))
    {}
    //: Constructor.
    
    static bool IsA(const ObjectC &obj)
    { return dynamic_cast<const ClassBodyC *>(&obj.Body()) != 0; }
    //: Is object a class ?
    
    ClassC(const ObjectC &obj)
      : ScopeC(dynamic_cast<const ClassBodyC *>(BodyPtr(obj)))
    {}
    //: Base class constructor.
    
  protected:
    ClassC(ClassBodyC &bod)
      : ScopeC(bod)
    {}
    //: Body constructor.
    
    ClassBodyC &Body() 
    { return dynamic_cast<ClassBodyC &>(ObjectC::Body()); }
    //: Access body.
    
    const ClassBodyC &Body() const
    { return dynamic_cast<const ClassBodyC &>(ObjectC::Body()); }
    //: Access body.
    
  public:
    
  };
  
  //! userlevel=Develop
  //: Templated class body.
  
  class ClassTemplateBodyC
    : public ClassBodyC 
  {
  public:
    ClassTemplateBodyC()
    {}
    //: Default constructor.

    ClassTemplateBodyC(const StringC &nname,const ObjectC &ntemplArgs)
      : ClassBodyC(nname),
	templArgs(ntemplArgs)
    {}
    //: Constructor.
    
    ClassTemplateBodyC(const StringC &nname,DListC<ObjectC> &contents)
      : ClassBodyC(nname,contents)
    {}
    //: Constructor.
    
    ClassTemplateBodyC(const StringC &nname,const ObjectC &contents,const ObjectC &inherit,const ObjectC &ntemplArgs)
      : ClassBodyC(nname,contents,inherit),
	templArgs(ntemplArgs)
    {}
    //: Constructor.
    
    ObjectListC &TemplArgList()
    { return templArgs; }
    //: Access arg list.
    
    virtual bool IsTemplate() const
    { return true; }
    //: Is this a templated object ?
    
    virtual StringC FullName(RCHashC<StringC,ObjectC> &templSub,DesciptionGeneratorC &dg = defaultDescGen,int maxDepth = 60) const;
    //: Get full name of object
    // template args and all.

    virtual ObjectC Subst(RCHashC<StringC,ObjectC> &subst) const;
    //: Create a new object with subsituted args.

  protected:
    ObjectListC templArgs; // Template args.
  };

  //! userlevel=Normal
  //: Templated class.
  
  class ClassTemplateC
    : public ClassC 
  {
  public:
    ClassTemplateC()
    {}
    //: Default constructor.
    // Creates an invalid handle.

    ClassTemplateC(const StringC &nname,const ObjectC &ntemplArgs)
      : ClassC(*new ClassTemplateBodyC(nname,ntemplArgs))
    {}
    //: Constructor.

    ClassTemplateC(const StringC &nname,DListC<ObjectC> &contents)
      : ClassC(*new ClassTemplateBodyC(nname,contents))
    {}
    //: Constructor.

    ClassTemplateC(const StringC &nname,const ObjectC &contents,const ObjectC &inherit,const ObjectC &ntemplArgs)
      : ClassC(*new ClassTemplateBodyC(nname,contents,inherit,ntemplArgs))
    {}
    //: Constructor.
    
    static bool IsA(const ObjectC &obj)
    { return dynamic_cast<const ClassTemplateBodyC *>(&obj.Body()) != 0; }
    //: Is object a class ?
    
    ClassTemplateC(ObjectC &obj)
      : ClassC(dynamic_cast<ClassTemplateBodyC *>(BodyPtr(obj)))
    {}
    //: Base class constructor.
  protected:
    ClassTemplateC(ClassTemplateBodyC &bod)
      : ClassC(bod)
    {}
    //: Body constructor.
    
    ClassTemplateC(const ClassTemplateBodyC *bod)
      : ClassC(bod)
    {}
    //: Body constructor.
    
    ClassTemplateBodyC &Body() 
    { return dynamic_cast<ClassTemplateBodyC &>(ObjectC::Body()); }
    //: Access body.
    
    const ClassTemplateBodyC &Body() const
    { return dynamic_cast<const ClassTemplateBodyC &>(ObjectC::Body()); }
    //: Access body.
    
  public:
    
    friend class ClassTemplateBodyC;
  };
  
  /////////////////////////////////////////////////
  // Object list

  class MethodC;
  
  //! userlevel=Develop
  //: Method object. 
  
  class MethodBodyC 
    : public ObjectBodyC
  {
  public:
    MethodBodyC()
      : isConstructor(false),
	isConversion(false),
	isPointer(false)
    {}
    //: Default constructor.
    
    MethodBodyC(const StringC &nname)
      : ObjectBodyC(nname),
	isConstructor(false),
	isConversion(false),
	isPointer(false)
    {}
    //: Constructor.
    
    MethodBodyC(const StringC &nname,const DataTypeC &rt,const ObjectListC &args,ObjectC &quals,bool isConv = false,bool isPtr = true);
    //: Constructor.

    MethodBodyC(const MethodC &om);
    //: Constructor.
    
    RCBodyVC &Copy() const
    { return *new MethodBodyC(*this); }
    //: Make copy of body.
    
    virtual const char *TypeName() const 
    { return "method"; }
    //: Get name of object type.
    
    void Rename(const StringC &name);
    //: Rename method.
    
    StringC IndexName() const;
    //: Generate a name which is suitable for indexing.
    // This is the name without the return type, and paramiters names.
    
    virtual void SetScope(ObjectC &obj) 
    { scopeInfo = obj; }
    //: Set scope for object.
    
    ObjectC &ScopeInfo() 
    { return scopeInfo; }
    //: Access scope info.
    
    const ObjectC &ScopeInfo() const
    { return scopeInfo; }
    //: Access scope info.
    
    virtual ObjectC GetScope() 
    { return scopeInfo; }
    //: Set scope for object.
    
    bool IsConstructor() const
    { return isConstructor; }
    //: Test if method is a constructor.

    bool IsConversion() const
    { return isConversion; }
    //: Test if method is a constructor.
    
    bool IsPointer() const
    { return isPointer; }
    //: Is this actually a function pointer.
    
    void SetConstructor(bool val);
    //: Set constructor flag.
    
    DataTypeC &ReturnType() 
    { return retType; }
    //: Access return type.
    
    DListC<DataTypeC> &Args()
    { return args; }
    //: Access arg list.
    
    ObjectC &Quals()
    { return quals; }
    //: Access method qualifiers.

    const DataTypeC &ReturnType() const
    { return retType; }
    //: Access return type.
    
    const DListC<DataTypeC> &Args() const
    { return args; }
    //: Access arg list.
    
    const ObjectC &Quals() const
    { return quals; }
    //: Access method qualifiers.
    
    virtual StringC FullName(RCHashC<StringC,ObjectC> &templSub,DesciptionGeneratorC &dg = defaultDescGen,int maxDepth = 60) const;
    //: Get full name of object
    // template args and all.
    
    const ObjectC &Definition() const
    { return definition; }
    //: Access definition of object.
    
    void SetDefinition(const ObjectC &obj)
    { definition = obj; }
    //: Set object definition.
    
    virtual void Dump(std::ostream &out,int indent = 0);
    //: Dump to 'out' in human readable form.
    
    StringC DefinitionName(const StringC &scopeName = StringC());
    //: Compose the name as definition
    
  protected:
    DataTypeC retType;
    DListC<DataTypeC> args;
    ObjectC scopeInfo;
    bool isConstructor;
    bool isConversion;
    bool isPointer;
    ObjectC quals;
    ObjectC definition;
  };
  
  //! userlevel=Normal
  //: Methodd object.
  
  class MethodC 
    : public ObjectC 
  {
  public:
    MethodC()
    {}
    //: Default Constructor.
    // creates an invalid object.
    
    MethodC(const StringC &nname)
      : ObjectC(*new MethodBodyC(nname))
    {}
    //: Constructor.
    
    static bool IsA(const ObjectC &obj)
    { return dynamic_cast<const MethodBodyC *>(&obj.Body()) != 0; }
    //: Is object a method ?
    
    MethodC(ObjectC &obj)
      : ObjectC(dynamic_cast<const MethodBodyC *>(BodyPtr(obj)))
    {}
    //: Base class constructor.
    
    MethodC(const MethodC &om,bool copy)
      : ObjectC(*new MethodBodyC(om))
    {}
    //: Constructor.
    
    MethodC(const StringC &nname,const DataTypeC &rt,const ObjectListC &args,ObjectC &quals,bool isConv = false,bool isPtr = false)
      : ObjectC(*new MethodBodyC(nname,rt,args,quals,isConv,isPtr))
    {}
    //: Constructor.
    
  protected:
    MethodC(MethodBodyC &bod)
      : ObjectC(bod)
    {}
    //: Body constructor.

    MethodC(const MethodBodyC *bod)
      : ObjectC(bod)
    {}
    //: Body constructor.
    
    MethodBodyC &Body() 
    { return dynamic_cast<MethodBodyC &>(ObjectC::Body()); }
    //: Access body.
    
    const MethodBodyC &Body() const
    { return dynamic_cast<const MethodBodyC &>(ObjectC::Body()); }
    //: Access body.
    
  public:
    MethodC Copy() const
    { return MethodC(static_cast<MethodBodyC &>(Body().Copy())); }
    //: Copy class.
    
    StringC IndexName() const
    { return Body().IndexName(); }
    //: Generate a name which is suitable for indexing.
    // This is the name without the return type, and paramiters names.
    
    void Rename(const StringC &name)
    { Body().Rename(name); }
    //: Rename method.
    
    ObjectC &ScopeInfo() 
    { return Body().ScopeInfo(); }
    //: Access scope info.
    
    const ObjectC &ScopeInfo() const
    { return Body().ScopeInfo(); }
    //: Access scope info.
    
    bool IsConstructor() const
    { return Body().IsConstructor(); }
    //: Test if method is a constructor.
    
    bool IsConversion() const
    { return Body().IsConversion(); }
    //: Is this method a conversion operator ?
    
    bool IsPointer() const
    { return Body().IsPointer(); }
    //: Is this actually a function pointer.
    
    void SetConstructor(bool val)
    { Body().SetConstructor(val); }
    //: Set constructor flag.
    
    DataTypeC &ReturnType() 
    { return Body().ReturnType(); }
    //: Access return type.
    
    DListC<DataTypeC> &Args()
    { return Body().Args(); }
    //: Access arg list.
    
    ObjectC &Quals()
    { return Body().Quals(); }
    //: Access method qualifiers.
    
    const DataTypeC &ReturnType() const
    { return Body().ReturnType(); }
    //: Access return type.
    
    const DListC<DataTypeC> &Args() const
    { return Body().Args(); }
    //: Access arg list.
    
    const ObjectC &Quals() const
    { return Body().Quals(); }
    //: Access method qualifiers.
    
    const ObjectC &Definition() const
    { return Body().Definition(); }
    //: Access definition of object.
    
    void SetDefinition(const ObjectC &obj)
    { Body().SetDefinition(obj); }
    //: Set object definition.
    
    StringC DefinitionName(const StringC &scopeName)
    { return Body().DefinitionName(scopeName); }
    //: Compose the name as definition
    
    friend class MethodBodyC;
  };
  
  //! userlevel=Develop
  //: Templated function body.
  
  class MethodTemplateBodyC
    : public MethodBodyC
  {
  public:
    MethodTemplateBodyC()
    {}
    //: Default constructor.
    
    MethodTemplateBodyC(const StringC &nname,const ObjectC &ntemplArgs)
      : MethodBodyC(nname),
	templArgs(ntemplArgs)
    {}
    //: Constructor.
    
    MethodTemplateBodyC(const StringC &nname,const DataTypeC &rt,const ObjectListC &args,ObjectC &quals,const ObjectC &ntemplArgs)
      : MethodBodyC(nname,rt,args,quals),
	templArgs(ntemplArgs)
    {}
    //: Constructor.

    MethodTemplateBodyC(const MethodC &meth,const ObjectC &ntemplArgs)
      : MethodBodyC(meth),
	templArgs(ntemplArgs)
    {}
    //: Constructor.
    
    ObjectListC &TemplArgList()
    { return templArgs; }
    //: Access arg list.
    
    bool IsTemplate() const
    { return true; }
    //: Access arg list.
    
    virtual StringC FullName(RCHashC<StringC,ObjectC> &templSub,DesciptionGeneratorC &dg = defaultDescGen,int maxDepth = 60) const;
    //: Get full name of object
    // template args and all.
    
  protected:
    ObjectListC templArgs; // Template args.

  };
  
  //! userlevel=Normal
  //: Templated function.
  
  class MethodTemplateC 
    : public MethodC
  {
  public:
    MethodTemplateC()
    {}
    //: Default constructor.
    // creates an invalid object.
        
    MethodTemplateC(const StringC &nname,const ObjectC &ntemplArgs)
      : MethodC(*new MethodTemplateBodyC(nname,ntemplArgs))
    {}
    //: Constructor.
    
    static bool IsA(const ObjectC &obj)
    { return dynamic_cast<const MethodTemplateBodyC *>(&obj.Body()) != 0; }
    //: Is object a method ?
    
    MethodTemplateC(ObjectC &obj)
      : MethodC(dynamic_cast<const MethodTemplateBodyC *>(BodyPtr(obj)))
    {}
    //: Base class constructor.
    
    MethodTemplateC(const StringC &nname,const DataTypeC &rt,const ObjectListC &args,ObjectC &quals,const ObjectC &ntemplArgs)
      : MethodC(*new MethodTemplateBodyC(nname,rt,args,quals,ntemplArgs))
    {}
    //: Constructor.
    
    MethodTemplateC(const MethodC &meth,const ObjectC &ntemplArgs) 
      : MethodC(*new MethodTemplateBodyC(meth,ntemplArgs))
    {}
    //: Turn a normal func into a templated one.
  protected:
    MethodTemplateC(MethodTemplateBodyC &bod)
      : MethodC(bod)
    {}
    //: Body constructor.
    
    MethodTemplateBodyC &Body() 
    { return dynamic_cast<MethodTemplateBodyC &>(ObjectC::Body()); }
    //: Access body.
    
    const MethodTemplateBodyC &Body() const
    { return dynamic_cast<const MethodTemplateBodyC &>(ObjectC::Body()); }
    //: Access body.
    
  public:
    
  };
  
  /////////////////////////////////////////////////
  // Object list

  class TypedefC;
  
  //! userlevel=Develop
  //: Typedef object.  like a class, struct, or namespace.
  
  class TypedefBodyC 
    : public ObjectBodyC
  {
  public:
    TypedefBodyC()
    {}
    //: Default constructor.
    
    TypedefBodyC(const StringC &nname,const DataTypeC &ntype);
    //: Constructor.
    
    void SetDefinition(const ObjectC &obj)
    { definition = obj; }
    //: Set type definition.

    virtual const char *TypeName() const 
    { return "typedef"; }
    //: Get name of object type.
    
    DataTypeC &DataType()
    { return dataType; }
    //: Access data type.

    virtual StringC FullName(RCHashC<StringC,ObjectC> &templSub,DesciptionGeneratorC &dg = defaultDescGen,int maxDepth = 60) const;
    //: Get full name of object
    // template args and all.
    
  protected:
    DataTypeC dataType;
    ObjectC definition; // Definition of type, if anonymous.
  };
  
  //! userlevel=Normal
  //: Typedefd object.  like a class, struct, or namespace.
  
  class TypedefC 
    : public ObjectC 
  {
  public:
    TypedefC()
    {}
    //: Default Constructor.
    // creates an invalid object.
    
    TypedefC(const StringC &nname,const DataTypeC &atype)
      : ObjectC(*new TypedefBodyC(nname,atype))
    {}
    //: Constructor.

    static bool IsA(const ObjectC &obj)
    { return dynamic_cast<const TypedefBodyC *>(&obj.Body()) != 0; }
    //: Is object a method ?
    
    TypedefC(ObjectC &obj)
      : ObjectC(dynamic_cast<const TypedefBodyC *>(BodyPtr(obj)))
    {}
    //: Base class constructor.
    
  protected:
    TypedefC(ObjectBodyC &bod)
      : ObjectC(bod)
    {}
    //: Body constructor.
    
    TypedefBodyC &Body() 
    { return dynamic_cast<TypedefBodyC &>(ObjectC::Body()); }
    //: Access body.
    
    const TypedefBodyC &Body() const
    { return dynamic_cast<const TypedefBodyC &>(ObjectC::Body()); }
    //: Access body.
    
  public:
    void SetDefinition(const ObjectC &obj)
    { Body().SetDefinition(obj); }
    //: Set type definition.
    
    DataTypeC &DataType()
    { return Body().DataType(); }
    //: Access data type.
    
  };
  
  /////////////////////////////////////////////////
  // Object list
  
  class VariableC;
  
  //! userlevel=Develop
  //: Variable object.  like a class, struct, or namespace.
  
  class VariableBodyC 
    : public ObjectBodyC
  {
  public:
    VariableBodyC()
    {}
    //: Default constructor.
    
    VariableBodyC(const StringC &naname);
    //: Constructor.
    
    VariableBodyC(ObjectC &ndt,const StringC &nname);
    //: Constructor.

    virtual const char *TypeName() const 
    { return "variable"; }
    //: Get name of object type.
    
    DataTypeC &DataType()
    { return dt; }
    //: Access data type.
    
  protected:
    DataTypeC dt;
  };

  //! userlevel=Normal
  //: Variabled object.  like a class, struct, or namespace.
  
  class VariableC 
    : public ObjectC 
  {
  public:
    VariableC()
    {}
    //: Default Constructor.
    // creates an invalid object.
    
    VariableC(const StringC &nname)
      : ObjectC(*new VariableBodyC(nname))
    {}
    //: Constructor.
    
    VariableC(ObjectC &dt,const StringC &nalias)
      : ObjectC(*new VariableBodyC(dt,nalias))
    {}
    //: Constructor.
    
    static bool IsA(const ObjectC &obj)
    { return dynamic_cast<const VariableBodyC *>(&obj.Body()) != 0; }
    //: Is object a method ?
    
    VariableC(ObjectC &obj)
      : ObjectC(dynamic_cast<const VariableBodyC *>(BodyPtr(obj)))
    {}
    //: Base class constructor.
    
  protected:
    VariableC(ObjectBodyC &bod)
      : ObjectC(bod)
    {}
    //: Body constructor.
    
    VariableC(const ObjectBodyC *bod)
      : ObjectC(bod)
    {}
    //: Body constructor.
    
    VariableBodyC &Body() 
    { return dynamic_cast<VariableBodyC &>(ObjectC::Body()); }
    //: Access body.
    
    const VariableBodyC &Body() const
    { return dynamic_cast<const VariableBodyC &>(ObjectC::Body()); }
    //: Access body.
    
  public:
    DataTypeC &DataType()
    { return Body().DataType(); }
    //: Access data type.
    
  };

  /////////////////////////////////////////////////
  // Marker.
  
  class MarkerC;
  
  //! userlevel=Develop
  //: Marker object.  like a class, struct, or namespace.
  
  class MarkerBodyC 
    : public ObjectBodyC
  {
  public:
    MarkerBodyC()
    {}
    //: Default constructor.
    
    MarkerBodyC(const StringC &nname)
      : ObjectBodyC(nname)
    {}
    //: Constructor.
    
  protected:
    
  };

  //! userlevel=Normal
  //: Markerd object.  like a class, struct, or namespace.
  
  class MarkerC 
    : public ObjectC 
  {
  public:
    MarkerC()
    {}
    //: Default Constructor.
    // creates an invalid object.
    
    MarkerC(const StringC &nname)
      : ObjectC(*new MarkerBodyC(nname))
    {}
    //: Constructor.

    MarkerC(ObjectC &obj)
      : ObjectC(dynamic_cast<const MarkerBodyC *>(BodyPtr(obj)))
    {}
    //: Base class constructor.

    static bool IsA(const ObjectC &obj)
    { return dynamic_cast<const MarkerBodyC *>(&obj.Body()) != 0; }
    //: Is object a method ?
    
  protected:
    MarkerC(ObjectBodyC &bod)
      : ObjectC(bod)
    {}
    //: Body constructor.
    
    MarkerBodyC &Body() 
    { return dynamic_cast<MarkerBodyC &>(ObjectC::Body()); }
    //: Access body.
    
    const MarkerBodyC &Body() const
    { return dynamic_cast<const MarkerBodyC &>(ObjectC::Body()); }
    //: Access body.
    
  public:
    
  };

  /////////////////////////////////////////////////
  // Inherit.
  
  class InheritC;
  
  //! userlevel=Develop
  //: Inherit object.  like a class, struct, or namespace.
  
  class InheritBodyC 
    : public ObjectBodyC
  {
  public:
    
    InheritBodyC()
      : scopeAccess(SAPublic),
	virt(false),
	resolveFailed(false)
    {}
    //: Default constructor.
    
    InheritBodyC(ScopeAccessT nscopeAccess,bool useNamespace = false);
    //: Constructor.

    virtual const char *TypeName() const 
    { return "inherit"; }
    //: Get name of object type.
    
    ScopeC &From();
    //: Object we inherit from;
    
    ScopeAccessT ScopeAccess() 
    { return scopeAccess; }
    //: Access to object.
    
    void SetScopeDef(const ObjectC &obj);
    //: Set the scope definition.
    // Used in parser.
    
    bool IsVirtual() const
    { return virt; }
    //: Test if object is virtual.
    
    void SetVirtual(bool val)
    { virt = val; }
    //: Set virtual inheritance flag.
    
    StringC ScopeDef();
    //: Scope definition as a string.

    StringC ScopeDef(RCHashC<StringC,ObjectC> &str) const;
    //: Scope definition as a string.

    bool Resolve();
    //: Attempt to resolve parent class.
    
    RCHashC<StringC,ObjectC> &TemplateSubs()
    { return templSub; }
    //: Access template subsitutions.
    
    virtual StringC FullName(RCHashC<StringC,ObjectC> &templSub,DesciptionGeneratorC &dg = defaultDescGen,int maxDepth = 60) const;
    //: Get full name of object
    // template args and all.
    
  protected:
    ScopeAccessT scopeAccess; // Access permissions to parent scope.
    ObjectC inheritDef;  // Definition for object we inherit from in the form of a path.
    ScopeC inheritFrom;  // Scope we inherit from.
    bool virt;           // Virtual inheritance ?
    bool resolveFailed;  // Did we succeed in resolving it ?
    bool useNamespace;   // Is this a using namespace ?
    RCHashC<StringC,ObjectC> templSub; // Template substitutions, where needed.
  };

  //! userlevel=Normal
  //: Inheritd object.  like a class, struct, or namespace.
  
  class InheritC 
    : public ObjectC 
  {
  public:
    InheritC()
    {}
    //: Default Constructor.
    // creates an invalid object.
    
    InheritC(ScopeAccessT nscopeAccess,bool useNamespace = false)
      : ObjectC(*new InheritBodyC(nscopeAccess,useNamespace))
    {}
    //: Constructor.
    
    InheritC(ObjectC &obj)
      : ObjectC(dynamic_cast<const InheritBodyC *>(BodyPtr(obj)))
    {}
    //: Base class constructor.

    static bool IsA(const ObjectC &obj)
    { return dynamic_cast<const InheritBodyC *>(&obj.Body()) != 0; }
    //: Is object a class ?
    
  protected:
    InheritC(ObjectBodyC &bod)
      : ObjectC(bod)
    {}
    //: Body constructor.
    
    InheritBodyC &Body() 
    { return dynamic_cast<InheritBodyC &>(ObjectC::Body()); }
    //: Access body.
    
    const InheritBodyC &Body() const
    { return dynamic_cast<const InheritBodyC &>(ObjectC::Body()); }
    //: Access body.
    
  public:
    void SetScopeDef(const ObjectC &obj) 
    { Body().SetScopeDef(obj); }
    //: Set the scope definition.
    // Used in parser.
    
    StringC ScopeDef()
    { return Body().ScopeDef(); }
    //: Scope definition as a string.

    StringC ScopeDef(RCHashC<StringC,ObjectC> &ts)
    { return Body().ScopeDef(ts); }
    //: Scope definition as a string.
    // with template substitutions.
    
    ScopeC &From()
    { return Body().From(); }
    //: Object we inherit from;
    
    ScopeAccessT ScopeAccess() 
    { return Body().ScopeAccess(); }
    //: Access to object.
    
    bool IsVirtual() const
    { return Body().IsVirtual(); }
    //: Test if object is virtual.
    
    void SetVirtual(bool val)
    { Body().SetVirtual(val);; }
    //: Set virtual inheritance flag.
    
    bool Resolve()
    { return Body().Resolve(); }
    //: Attempt to resolve inherited class.
    
    RCHashC<StringC,ObjectC> &TemplateSubs()
    { return Body().TemplateSubs(); }
    //: Access template subsitutions.
    
  };

  /////////////////////////////////////////////////
  // Derived.
  
  class DerivedC;
  
  //! userlevel=Develop
  //: Derived object.  like a class, struct, or namespace.
  
  class DerivedBodyC 
    : public ObjectBodyC
  {
  public:
    DerivedBodyC()
      : scopeAccess(SAPublic)
    {}
    //: Default constructor.
    
    DerivedBodyC(ScopeAccessT nscopeAccess,const ObjectListC &path);
    //: Constructor.

    virtual const char *TypeName() const 
    { return "derived"; }
    //: Get name of object type.
    
    ScopeAccessT ScopeAccess() 
    { return scopeAccess; }
    //: Access to object.
    
    ObjectListC &Parent()
    { return path; }
    //: Get path to parent object

    ObjectC DerivedObj();
    //: Get handle to derived object.
    // (Slowish)
    
  protected:
    ScopeAccessT scopeAccess;
    ObjectListC path;  // Path to object.
  };

  ////////////////////////////////////////////////////////////////////////
  
  //! userlevel=Normal
  //: Derivedd object.  like a class, struct, or namespace.

  class DerivedC 
    : public ObjectC 
  {
  public:
    DerivedC()
    {}
    //: Default Constructor.
    // creates an invalid object.
    
    DerivedC(ScopeAccessT nscopeAccess,const ObjectListC &npath)
      : ObjectC(*new DerivedBodyC(nscopeAccess,npath))
    {}
    //: Constructor.
    
    DerivedC(ObjectC &obj)
      : ObjectC(dynamic_cast<const DerivedBodyC *>(BodyPtr(obj)))
    {}
    //: Base class constructor.
    
    static bool IsA(const ObjectC &obj)
    { return dynamic_cast<const DerivedBodyC *>(&obj.Body()) != 0; }
    //: Is object a class ?
    
  protected:
    DerivedC(ObjectBodyC &bod)
      : ObjectC(bod)
    {}
    //: Body constructor.
    
    DerivedC(const ObjectBodyC *bod)
      : ObjectC(bod)
    {}
    //: Body constructor.
    
    DerivedBodyC &Body() 
    { return dynamic_cast<DerivedBodyC &>(ObjectC::Body()); }
    //: Access body.
    
    const DerivedBodyC &Body() const
    { return dynamic_cast<const DerivedBodyC &>(ObjectC::Body()); }
    //: Access body.
    
  public:
    ScopeAccessT ScopeAccess() 
    { return Body().ScopeAccess(); }
    //: Access to object.
    
    ObjectListC &Parent()
    { return Body().Parent(); }
    //: Get path to parent object

    ObjectC DerivedObj()
    { return Body().DerivedObj(); }
    //: Get handle to derived object.
    // (Slowish)
  };
  
  
  /////////////////////////////////////////////////
  // Object list

  class EnumC;
  
  //! userlevel=Develop
  //: Enum object.  like a class, struct, or namespace.
  
  class EnumBodyC 
    : public ObjectListBodyC
  {
  public:
    EnumBodyC()
    {}
    //: Default constructor.
    
    EnumBodyC(const StringC &nname,const ObjectListC &nvalues);
    //: Constructor.

    EnumBodyC(const StringC &nname);
    //: Constructor.
    
    virtual const char *TypeName() const 
    { return "enum"; }
    //: Get name of object type.
    
  protected:
  };
  
  //! userlevel=Normal
  //: Enumd object.  like a class, struct, or namespace.
  
  class EnumC 
    : public ObjectListC 
  {
  public:
    EnumC()
    {}
    //: Default Constructor.
    // creates an invalid object.
    
    EnumC(const StringC &nname,const ObjectListC &nvalues)
      : ObjectListC(*new EnumBodyC(nname,nvalues))
    {}
    //: Constructor.

    EnumC(const StringC &nname)
      : ObjectListC(*new EnumBodyC(nname))
    {}
    //: Constructor.
    
    static bool IsA(const ObjectC &obj)
    { return dynamic_cast<const EnumBodyC *>(&obj.Body()) != 0; }
    //: Is object a method ?
    
    EnumC(ObjectC &obj)
      : ObjectListC(dynamic_cast<const EnumBodyC *>(BodyPtr(obj)))
    {}
    //: Base class constructor.
    
  protected:
    EnumC(ObjectBodyC &bod)
      : ObjectListC(bod)
    {}
    //: Body constructor.
    
    EnumBodyC &Body() 
    { return dynamic_cast<EnumBodyC &>(ObjectC::Body()); }
    //: Access body.
    
    const EnumBodyC &Body() const
    { return dynamic_cast<const EnumBodyC &>(ObjectC::Body()); }
    //: Access body.
    
  public:
    
  };
  
}

#endif
