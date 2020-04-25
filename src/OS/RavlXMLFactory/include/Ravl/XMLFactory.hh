// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2009, OmniPerception Ltd
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_XMLFACTORY_HEADER
#define RAVL_XMLFACTORY_HEADER 1
//! lib=RavlXMLFactory
//! author=Charles Galambos
//! docentry=Ravl.API.Core.IO.XMLFactory

#include "Ravl/Exception.hh"
#include "Ravl/XMLTree.hh"
#include "Ravl/SmartPtr.hh"
#include "Ravl/DList.hh"
#include "Ravl/RCAbstract.hh"
#include "Ravl/SmartLayerPtr.hh"
#include "Ravl/Threads/RWLock.hh"
#include "Ravl/Threads/Mutex.hh"
#include "Ravl/SysLog.hh"
#include "Ravl/RCWrap.hh"
#include "Ravl/Collection.hh"
#include "Ravl/Traits.hh"
#include "Ravl/HSet.hh"

#include <vector>

#define rThrowBadConfigContextOnFailS(CONTEXT, METHOD, ARGS) { \
  if (!CONTEXT.METHOD) { \
    RavlN::StrOStreamC strm; \
    strm << ARGS << "\n"; \
    strm << "Factory filename: '" << CONTEXT.Factory().MasterConfigFilename() << "'  Node: " << CONTEXT.Name() << " \n"; \
    strm << "From: " << __FILE__ << " Line:" << __LINE__ ; \
    RavlN::SysLog(RavlN::SYSLOG_ERR) << strm.String(); \
    throw RavlN::ExceptionBadConfigC(strm.String().data(), true); \
  } \
}


namespace RavlN {

  class XMLFactoryC;
  class XMLFactoryHC;

  enum XMLFactorySearchScopeT
  { XMLFACTORY_SEARCH_PARENT_NODES,
    XMLFACTORY_SEARCH_LOCAL_ONLY
  };

  enum XMLFactoryCreateModeT
  { XMLFACTORY_CREATE_DEFAULT,
    XMLFACTORY_CREATE_NO_REGISTERATION
  };

  //! uselevel=Develop
  //: Node in the build tree.

  class XMLFactoryNodeC
    : public RCLayerBodyC
  {
  public:
    XMLFactoryNodeC()
      : m_createLock(true), // Create a recursive lock.
        m_xmlNode(true),
        m_createComponentCalled(false)
    {}
    //: Default constructor.

    explicit XMLFactoryNodeC(const XMLTreeC &xmlNode)
      : m_createLock(true), // Create a recursive lock.
        m_xmlNode(xmlNode),
        m_createComponentCalled(false)
    {}
    //: Constructor.

    XMLFactoryNodeC(const XMLTreeC &xmlNode,const XMLFactoryNodeC &parent)
      : m_createLock(true), // Create a recursive lock.
        m_xmlNode(xmlNode),
        m_createComponentCalled(false),
        m_parent(&parent)
    {}
    //: Constructor.

    XMLFactoryNodeC(const XMLTreeC &xmlNode,const XMLFactoryNodeC &parent,const RCWrapAbstractC &component)
      : m_createLock(true), // Create a recursive lock.
        m_xmlNode(xmlNode),
        m_component(component),
        m_createComponentCalled(false),
        m_parent(&parent)
    {}
    //: Constructor.

    ~XMLFactoryNodeC();
    //: Destructor.

    typedef SmartOwnerPtrC<XMLFactoryNodeC> RefT;
    //: Owner reference

    typedef SmartCallbackPtrC<XMLFactoryNodeC> CBRefT;
    //: Callback reference.

    const StringC &Name() const
    { return m_xmlNode.Name(); }
    //: Access node name.

    RCWrapAbstractC Component() const
    {
      RWLockHoldC hold(m_access,RWLOCK_READONLY);
      RCWrapAbstractC ret = m_component;
      hold.Unlock();
      return ret;
    }
    //: Component

    void SetComponent(const RCWrapAbstractC &comp) {
      RWLockHoldC hold(m_access,RWLOCK_WRITE);
      m_component = comp;
    }
    //: Set component at node.

    bool FollowPath(const StringC &path,RefT &node,bool verbose = false) const;
    //: Follow path to child node.

    bool FollowPath(const DListC<StringC> &path,RefT &node,bool verbose = false) const;
    //: Follow path to child node.

    bool UsePath(const StringC &path,
                 RefT &node,
                 bool restrictToXMLTree = true,
                 bool verbose = false,
                 XMLFactorySearchScopeT searchScope = XMLFACTORY_SEARCH_PARENT_NODES);
    //: Follow path to child node, create nodes where needed

    bool UsePath(const DListC<StringC> &path,
                 RefT &node,
                 bool restrictToXMLTree = true,
                 bool verbose = false,
                 XMLFactorySearchScopeT searchScope = XMLFACTORY_SEARCH_PARENT_NODES);
    //: Follow path to child node, create nodes where needed

    bool FindChild(const StringC &name,RefT &child) const;
    //: Find named child.

    bool UseChild(const StringC &name,RefT &child,bool restrictToXMLTree = true,bool verbose = false);
    //: Find child and create one if it doesn't exist.

    bool AddChild(const StringC &name,XMLFactoryNodeC &child);
    //: Add a child to this node.
    // Note: This will overwrite any existing node of the name

    bool DelChild(const StringC &name);
    //: Remove a child.

    template<class DataT>
    bool SetComponent(const StringC &name,const DataT &data) {
      RCWrapC<DataT> handle(data);
      return SetComponentInternal(name,handle);
    }
    //: Get named component.

    XMLTreeC &XMLNode()
    { return m_xmlNode; }
    //: XML Node

    const XMLTreeC &XMLNode() const
    { return m_xmlNode; }
    //: XML Node

    StringC Path() const;
    //: Get path to this node.

    bool CreateComponent(XMLFactoryC &factory);
    //: Create component in node.

    bool HasParent() const
    { return m_parent.IsValid(); }
    //: Has node got a parent ?

    bool HasChild(const StringC &name) const
    { return m_xmlNode.IsElm(name) || m_xmlNode.Data().IsElm(name); }
    //: Test if a child exists.

    bool Dump(std::ostream &strm,int level = 0);
    //: Dump node tree in human readable form.
    
    bool CheckUsedAttributes() const;
    //: Check if all attributes have been used.
    
    void UseAttribute(const StringC &name) const;
    //: Flag attribute as used.
    
    StringC AttributeString(const StringC &name,const StringC &defaultValue = StringC("")) const
    { 
      UseAttribute(name);
      return XMLNode().AttributeString(name,defaultValue); 
    }
    //: Access attribute.

    UIntT AttributeUInt(const StringC &name,UIntT defaultValue = 0) const
    { 
      UseAttribute(name);
      return XMLNode().AttributeUInt(name,defaultValue); 
    }
    //: Access attribute.

    IntT AttributeInt(const StringC &name,IntT defaultValue = 0) const
    { 
      UseAttribute(name);
      return XMLNode().AttributeInt(name,defaultValue); 
    }
    //: Access attribute.

    RealT AttributeReal(const StringC &name,RealT defaultValue = 0) const
    { 
      UseAttribute(name);
      return XMLNode().AttributeReal(name,defaultValue); 
    }
    //: Access attribute.

    UInt64T AttributeUInt64(const StringC &name,UInt64T defaultValue = 0) const
    { 
      UseAttribute(name);
      return XMLNode().AttributeUInt64(name,defaultValue); 
    }
    //: Access attribute.

    Int64T AttributeInt64(const StringC &name,Int64T defaultValue = 0) const
    { 
      UseAttribute(name);
      return XMLNode().AttributeInt64(name,defaultValue); 
    }
    //: Access attribute.

    bool AttributeBool(const StringC &name,bool defaultValue = false) const
    { 
      UseAttribute(name);
      return XMLNode().AttributeBool(name,defaultValue); 
    }
    //: Access attribute.

    template<typename DataT>
    bool Attribute(const StringC &name,DataT &value,const DataT &defaultValue = DataT()) const
    { 
      UseAttribute(name);
      return XMLNode().Attribute(name,value,defaultValue); 
    }
    //: Access generic attribute.
    // Return true if non default value has been specified.
    
    bool HasAttribute(const StringC &name) const
    {
      if(XMLNode().HasAttribute(name)) {
        UseAttribute(name);
        return true;
      }
      return false;
    }
    //: Test if attribute is set.

    const RavlN::HSetC<RavlN::StringC> &UseAttributes() const
    { return m_usedAttributes; }
    //: Access set of used attributes.
    
  protected:

    bool GetComponentInternal(const StringC &name,
                              const std::type_info &to,
                              RCWrapAbstractC &handle,
                              bool silentError=false
                              ) const;
    //: Get named component.

    bool UseComponentInternal(XMLFactoryC &factory,
                              const StringC &name,
                              const std::type_info &to,
                              RCWrapAbstractC &handle,
                              bool silentError = false,
                              const std::type_info &defaultType = typeid(void),
                              XMLFactorySearchScopeT searchScope = XMLFACTORY_SEARCH_PARENT_NODES
                              );
    //: Get named component.

    bool SetComponentInternal(const StringC &name,const RCWrapAbstractC &handle);
    //: Set named component.

    virtual void ZeroOwners();
    //: Called when owner handles drop to zero.

    RWLockC m_access;
    MutexC m_createLock;
    XMLTreeC m_xmlNode;
    RCWrapAbstractC m_component;
    bool m_createComponentCalled;
    HashC<StringC,RefT> m_children;

    CBRefT m_parent; // Parent node.
    
    mutable RavlN::HSetC<RavlN::StringC> m_usedAttributes;

  private:
    XMLFactoryNodeC(const XMLFactoryNodeC &other);
    //! Copy constructor not supported

    friend class XMLFactoryC;
  };

  //! userlevel=Normal
  //: XML Factory context.
  // This class should be passed into the construction methods.
  // It contains a handle to the factory and the current place in the parse tree.
  // To use the class you should define RAVL type converter between an instance of
  // this class and the object you wish to construct. See XMLFactoryRegisterC for
  // an example of how to do this using a constructor.

  class XMLFactoryContextC
  {
  public:
    XMLFactoryContextC()
    {}
    //: Constructor.

    explicit XMLFactoryContextC(const StringC &configFile,XMLTreeLoadC *loader = 0);
    //: Setup directly from a config file name.

    XMLFactoryContextC(const XMLFactoryNodeC &iNode)
      : m_iNode(&iNode)
    {}
    //: Constructor.

    XMLFactoryContextC(const XMLFactoryC &factory,const XMLFactoryNodeC &iNode);
    //: Constructor.

    XMLFactoryContextC(const XMLFactoryContextC &context,const StringC &childName);
    //: Got a child node.

    explicit XMLFactoryContextC(const XMLFactoryC &factory);
    //: Construct from Factory.

    XMLFactoryContextC(const XMLFactoryHC &factory);
    //: Construct from Factory.

    ~XMLFactoryContextC();
    //: Destructor
    
    const XMLTreeC &Node() const
    { return m_iNode->XMLNode(); }
    //: Access xml node.

    XMLTreeC &Node()
    { return m_iNode->XMLNode(); }
    //: Access xml node.

    const XMLFactoryNodeC &INode() const {
      RavlAssert(m_iNode.IsValid());
      return *m_iNode;
    }
    //: Access node

    XMLFactoryNodeC &INode() {
      RavlAssert(m_iNode.IsValid());
      return *m_iNode;
    }
    //: Access node
    
    DListC<XMLTreeC> &Children()
    { return Node().Children(); }
    //: Access a list of child nodes.

    const DListC<XMLTreeC> &Children() const
    { return Node().Children(); }
    //: Access a list of child nodes.

    const StringC &Name() const
    { return m_iNode->Name(); }
    //: Node name.

    void UseAttribute(const StringC &name) const
    { return INode().UseAttribute(name); }
    //: Flag attribute as used.
    
    StringC AttributeString(const StringC &name,const StringC &defaultValue = StringC("")) const
    { return INode().AttributeString(name,defaultValue);  }
    //: Access attribute.

    UIntT AttributeUInt(const StringC &name,UIntT defaultValue = 0) const
    { return INode().AttributeUInt(name,defaultValue); }
    //: Access attribute.

    IntT AttributeInt(const StringC &name,IntT defaultValue = 0) const
    { return INode().AttributeInt(name,defaultValue); }
    //: Access attribute.

    RealT AttributeReal(const StringC &name,RealT defaultValue = 0) const
    { return INode().AttributeReal(name,defaultValue); }
    //: Access attribute.

    UInt64T AttributeUInt64(const StringC &name,UInt64T defaultValue = 0) const
    { return INode().AttributeUInt64(name,defaultValue);  }
    //: Access attribute.

    Int64T AttributeInt64(const StringC &name,Int64T defaultValue = 0) const
    { return INode().AttributeInt64(name,defaultValue); }
    //: Access attribute.

    bool AttributeBool(const StringC &name,bool defaultValue = false) const
    { return INode().AttributeBool(name,defaultValue); }
    //: Access attribute.

    template<typename DataT>
    bool Attribute(const StringC &name,DataT &value,const DataT &defaultValue = DataT()) const
    { return INode().Attribute(name,value,defaultValue); }
    //: Access generic attribute.
    // Return true if non default value has been specified.

    bool HasAttribute(const StringC &name) const
    { return INode().HasAttribute(name); }
    //: Is a named attribute set ?

    XMLFactoryC &Factory() const;
    //: Access handle to associated factory.

    void Invalidate() {
      m_iNode.Invalidate();
      m_factory.Invalidate();
    }
    //: Invalidate context.

    bool IsValid() const
    { return m_iNode.IsValid() && m_factory.IsValid(); }
    //: Test if context is valid.

    template<class DataT>
    bool UseComponent(const StringC &name,
                      DataT &data,
                      bool suppressErrorMessages = false,
                      const std::type_info &defaultType=typeid(void)) const;
    //: Get named component, or create it if not found.
    // This will search parent context's successively up the tree until a
    // component with a matching name is found.

    template<class DataT>
    bool UseChildComponent(const StringC &name,
                           DataT &data,
                           bool suppressErrorMessages = false,
                           const std::type_info &defaultType=typeid(void)) const;
    //: Get named component, or create it if not found.
    // This will only check the local context for the component, creating
    // a new instance if its not found.

    bool UseComponent(const StringC &name,
                      RCWrapAbstractC &data,
                      bool suppressErrorMessages = false,
                      const std::type_info &defaultType=typeid(void)) const;
    //: Get named component, or create it if not found.
    // This will search parent context's successively up the tree until a
    // component with a matching name is found.

    bool UseChildComponent(const StringC &name,
                           RCWrapAbstractC &data,
                           bool suppressErrorMessages = false,
                           const std::type_info &defaultType=typeid(void)) const;
    //: Get named component, or create it if not found.
    // This will only check the local context for the component, creating
    // a new instance if its not found.

    
    template<class DataT>
    bool CreateComponent(const StringC &name,DataT &data,
                         bool suppressErrorMessages = false,
                         XMLFactoryCreateModeT createMode = XMLFACTORY_CREATE_DEFAULT) const;
    //: Create a new instance of the named component.

    template<class DataT>
    bool GetComponent(const StringC &name,DataT &data,bool suppressErrorMessages = false) const;
    //: Get instance of the named component.

    void SetFactory(const XMLFactoryC &factory);
    //: Set factory to use.

    template<class DataT>
    bool SetComponent(const StringC &name,const DataT &data);
    //: Set component for name.

    bool ChildContext(const StringC &key,XMLFactoryContextC &child) const;
    //: lookup child in tree.
    // Returns true and updates parameter 'child' if child is found.

    bool CreateContext(const StringC &key,XMLFactoryContextC &child) const;
    //: lookup child in tree.
    // Returns true and updates parameter 'child' if child is found.

    bool HasChild(const StringC &childName) const
    { return m_iNode->HasChild(childName); }
    //: Test if a child context exist

    StringC Path() const
    { return m_iNode->Path(); }
    //: Get path to this node.

    template<class DataT>
    bool UseComponentGroup(const StringC &group,std::vector<DataT> &list,const std::type_info &defaultType=typeid(void)) const {
      XMLFactoryContextC childContext;
      if(!ChildContext(group,childContext))
        return false;
      HSetC<StringC> used;
      for(RavlN::DLIterC<XMLTreeC> it(childContext.Children());it;it++) {
        if(used.IsMember(it->Name())) {
          RavlError("Duplicate name '%s' in group '%s' ",it->Name().c_str(),Path().c_str());
          throw RavlN::ExceptionBadConfigC("Duplicate component");
        }
        used += it->Name();
        DataT value;
        if(!childContext.UseChildComponent(it->Name(),value,false,defaultType)) {
          RavlError("Failed to load child component %s, at %s ",it->Name().data(),childContext.Path().data());
          throw RavlN::ExceptionBadConfigC("Failed to load component");
        }
        list.push_back(value);
      }
      return true;
    }
    //: Load component list.
    // Returns true if child group exists, though it still may be empty.

    template<class DataT>
    bool UseComponentGroup(const StringC &group,DListC<DataT> &list,const std::type_info &defaultType=typeid(void)) const {
      XMLFactoryContextC childContext;
      if(!ChildContext(group,childContext))
        return false;
      HSetC<StringC> used;
      for(RavlN::DLIterC<XMLTreeC> it(childContext.Children());it;it++) {
        if(used.IsMember(it->Name())) {
          RavlError("Duplicate name '%s' in group '%s' ",it->Name().c_str(),Path().c_str());
          throw RavlN::ExceptionBadConfigC("Duplicate component");
        }
        used += it->Name();
        DataT value;
        if(!childContext.UseChildComponent(it->Name(),value,false,defaultType)) {
          RavlError("Failed to load child component %s, at %s ",it->Name().data(),childContext.Path().data());
          throw RavlN::ExceptionBadConfigC("Failed to load component");
        }
        list.InsLast(value);
      }
      return true;
    }
    //: Load component list.
    // Returns true if child group exists, though it still may be empty.

    template<class DataT>
    bool UseComponentGroup(const StringC &group,CollectionC<DataT> &list,const std::type_info &defaultType=typeid(void)) const {
      XMLFactoryContextC childContext;
      if(!ChildContext(group,childContext))
        return false;
      if(!list.IsValid())
        list = CollectionC<DataT>(childContext.Children().Size());
      HSetC<StringC> used;
      for(RavlN::DLIterC<XMLTreeC> it(childContext.Children());it;it++) {
        if(used.IsMember(it->Name())) {
          RavlError("Duplicate name '%s' in group '%s' ",it->Name().c_str(),Path().c_str());
          throw RavlN::ExceptionBadConfigC("Duplicate component");
        }
        used += it->Name();
        DataT value;
        if(!childContext.UseChildComponent(it->Name(),value,false,defaultType)) {
          RavlError("Failed to load child component %s, at %s ",it->Name().data(),childContext.Path().data());
          throw RavlN::ExceptionBadConfigC("Failed to load component");
        }
        list.Append(value);
      }
      return true;
    }
    //: Load component list.
    // Returns true if child group exists, though it still may be empty.

    template<class DataT>
    bool UseComponentGroup(const StringC &group,SArray1dC<DataT> &list,const std::type_info &defaultType=typeid(void)) const {
      CollectionC<DataT> col;
      if(!UseComponentGroup(group,col,defaultType))
        return false;
      list = col.Array();
      return true;
    }
    //: Load component list.
    // Returns true if child group exists, though it still may be empty.

    template<typename ObjT,typename DataT,typename RetT>
    bool UseComponentGroup(const StringC &group,ObjT &obj,RetT (ObjT::*addMethod)(DataT),const std::type_info &defaultType=typeid(void)) const {
      XMLFactoryContextC childContext;
      if(!ChildContext(group,childContext))
        return false;
      HSetC<StringC> used;
      for(RavlN::DLIterC<XMLTreeC> it(childContext.Children());it;it++) {
        if(used.IsMember(it->Name())) {
          RavlError("Duplicate name '%s' in group '%s' ",it->Name().c_str(),Path().c_str());
          throw RavlN::ExceptionBadConfigC("Duplicate component");
        }
        used += it->Name();
        typename TraitsC<DataT>::BaseTypeT value;
        if(!childContext.UseChildComponent(it->Name(),value,false,defaultType)) {
          RavlError("Failed to load child component %s, at %s ",it->Name().data(),childContext.Path().data());
          throw RavlN::ExceptionBadConfigC("Failed to load component");
        }
        (obj.*addMethod)(value);
      }
      return true;
    }
    //: Load component list.
    // Returns true if child group exists, though it still may be empty.

    template<typename ObjT,typename DataT,typename RetT>
    bool UseComponentGroup(const StringC &group,ObjT &obj,RetT (ObjT::*addMethod)(const StringC &name,DataT),const std::type_info &defaultType=typeid(void)) const {
      XMLFactoryContextC childContext;
      if(!ChildContext(group,childContext))
        return false;
      HSetC<StringC> used;
      for(RavlN::DLIterC<XMLTreeC> it(childContext.Children());it;it++) {
        if(used.IsMember(it->Name())) {
          RavlError("Duplicate name '%s' in group '%s' ",it->Name().c_str(),Path().c_str());
          throw RavlN::ExceptionBadConfigC("Duplicate component");
        }
        used += it->Name();
        typename TraitsC<DataT>::BaseTypeT value;
        if(!childContext.UseChildComponent(it->Name(),value,false,defaultType)) {
          RavlError("Failed to load child component %s, at %s ",it->Name().data(),childContext.Path().data());
          throw RavlN::ExceptionBadConfigC("Failed to load component");
        }
        (obj.*addMethod)(it->Name(),value);
      }
      return true;
    }
    //: Load named component list.
    // Returns true if child group exists, though it still may be empty.

  protected:
    mutable XMLFactoryNodeC::RefT m_iNode;
    RCAbstractC m_factory;
  };

  //! userlevel=Advanced
  //: XML factory class.
  // This class holds the config parse tree, and maintains information on objects
  // That have already been constructed.

  class XMLFactoryC
    : public RCBodyVC
  {
  public:
    XMLFactoryC();
    //: Default constructor

    explicit XMLFactoryC(const XMLFactoryContextC &context);
    //: Recursive constructor.

    explicit XMLFactoryC(const StringC &configFile,XMLTreeLoadC *loader = 0);
    //: Open a config file.

    XMLFactoryC(const StringC &configFileName,const XMLTreeC &configTree);
    //: Use an existing tree.

    bool Read(const StringC &filename,XMLTreeLoadC *loader = 0);
    //: Read config file.

    typedef RCWrapAbstractC (*TypeFactoryT)(const XMLFactoryContextC &node);
    //: Factory function type

    const XMLTreeC &ConfigTree() const
    { return m_configTree; }
    //: Access config tree

    const StringC &MasterConfigFilename() const
    { return m_masterConfigFilename; }
    //: Access name of config file.

    template<class DataT>
    bool SetComponent(const StringC &name,const DataT &data)
    { return m_iRoot->SetComponent(name,data); }
    //: Set named component.

    template<class DataT>
    bool GetComponent(const StringC &name,DataT &data) {
      RCWrapC<DataT> handle;
      if(!m_iRoot->GetComponentInternal(name,typeid(DataT),handle))
        return false;
      data = handle.Data();
      return true;
    }
    //: Get named component, returns false if the component doesn't exist.
    //: This does NOT create the object if its not found.

    template<class DataT>
    bool UseComponent(const XMLFactoryContextC& currentContext,
                      const StringC &name,
                      DataT &data,
                      bool suppressErrors = false,
                      const std::type_info &defaultType = typeid(void),
                      XMLFactorySearchScopeT searchScope = XMLFACTORY_SEARCH_PARENT_NODES
                      ) const
    {
      RCWrapC<DataT> handle;
      if(!const_cast<XMLFactoryNodeC &>(currentContext.INode()).UseComponentInternal(const_cast<XMLFactoryC &>(*this),
                                                                                     name,
                                                                                     typeid(DataT),
                                                                                     handle,
                                                                                     suppressErrors,
                                                                                     defaultType,
                                                                                     searchScope
                                                                                     )) {
        if(!suppressErrors)
          throw RavlN::ExceptionBadConfigC("Failed to use component");
        return false;
      }
      RavlAssert(handle.IsValid());
      data = handle.Data();
      return true;
    }
    //: Get named component, or create it if not found.

    bool UseComponent(const XMLFactoryContextC& currentContext,
                      const StringC &name,
                      RCWrapAbstractC &data,
                      bool suppressErrors = false,
                      const std::type_info &defaultType = typeid(void),
                      XMLFactorySearchScopeT searchScope = XMLFACTORY_SEARCH_PARENT_NODES
                      ) const
    {
      if(!const_cast<XMLFactoryNodeC &>(currentContext.INode()).UseComponentInternal(const_cast<XMLFactoryC &>(*this),
                                                                                     name,
                                                                                     typeid(void),
                                                                                     data,
                                                                                     suppressErrors,
                                                                                     defaultType,
                                                                                     searchScope
                                                                                     )) {
        if(!suppressErrors)
          throw RavlN::ExceptionBadConfigC("Failed to use component");
        return false;
      }
      return true;
    }
    //: Get named component, or create it if not found.

    template<class DataT>
    bool CreateComponent(const XMLFactoryContextC &node,DataT &data)
    {
      RCWrapC<DataT> handle;
      if(!CreateComponentInternal(node,typeid(DataT),handle))
        return false;
      data = handle.Data();
      return true;
    }
    //: Create a component


    bool FindComponentToCreate(const XMLFactoryContextC& currentContext,const StringC &name,StringC &redirect,XMLFactoryContextC &newNode,bool suppressErrors);

    template<class DataT>
    bool CreateComponent(const XMLFactoryContextC& currentContext,
                         const StringC &name,
                         DataT &data,
                         bool suppressErrors = false,
                         XMLFactoryCreateModeT createMode = XMLFACTORY_CREATE_DEFAULT
                         )
    {
      XMLFactoryContextC newNode;
      StringC redirect;

      if(!FindComponentToCreate(currentContext,name,redirect,newNode,suppressErrors))
        return false;

      if(!CreateComponent(newNode,data)) {
        if(!suppressErrors)
          throw RavlN::ExceptionBadConfigC("Failed to create component");
        return false;
      }

      // Store ready for reuse.
      if(createMode != XMLFACTORY_CREATE_NO_REGISTERATION)
        const_cast<XMLFactoryNodeC &>(currentContext.INode()).AddChild(redirect,newNode.INode());
      return true;
    }
    //: Create named component, even if it exists already.

    static void RegisterTypeFactory(const std::type_info &typeInfo,TypeFactoryT typeFactoryFunc);
    //: Register a factory function.
    //: Note: This is NOT thread safe.

    static bool RegisterTypeAlias(const char *originalName,const char *newName);
    //: Register an alias for a type. This must be done after the type is registered.
    //: Note: This is NOT thread safe.

    static void ListKnownTypes(CollectionC<StringC> &types);
    //: Generate a list of known types.

    static void DumpKnownTypes(std::ostream &strm = std::cout);
    //: Write known types to a stream.

    template<class DataT>
    static RCWrapAbstractC DefaultFactoryFunc(const XMLFactoryContextC &node)
    { return RCWrapC<typename DataT::RefT>(typename DataT::RefT(*new DataT(node))); }
    //: Default factory function.

    bool SetupClean()
    { return m_setupClean; }
    //: Check if we've been setup cleanly.

    XMLFactoryContextC RootContext()
    { return XMLFactoryContextC(*this); }
    //: Access the root context.

    XMLFactoryNodeC &IRoot()
    { return *m_iRoot; }
    //: Root

    const XMLFactoryNodeC &IRoot() const
    { return *m_iRoot; }
    //: Root

    bool FollowPath(const StringC &path,XMLFactoryNodeC::RefT &node);
    //: Follow path to node.

    bool IsPostReadConfigDone() const
    { return !m_donePostSetup; }
    //: Check if post read of config has been done.

    bool PostReadConfig();
    //: Check config thats just been loaded.

    bool VerboseMode() const
    { return m_verbose; }
    //: Are we in verbose mode ?

    bool CheckConfig()
    { return m_checkConfig; }
    //: Check xml files for consistency as they're read.
    // At the moment this just looks for unused 
    // attributes.
    
    bool Clear();
    //: Clear factory contents.
    // This is can be used at shutdown to ensure all memory is free'd

    typedef SmartPtrC<XMLFactoryC> RefT;
    //: Reference

  protected:
    bool Child(const StringC &key,XMLFactoryContextC &child) const
    {
      XMLFactoryNodeC::RefT node;
      if(!m_iRoot->FollowPath(key,node))
        return false;
      child = XMLFactoryContextC(*this,*node);
      return true;
    }
    //: lookup child in tree.
    // Returns true and updates parameter 'child' if child is found.


    virtual bool CreateComponentInternal(const XMLFactoryContextC &node,const std::type_info &to,RCWrapAbstractC &handle);
    //: Create a component

    virtual bool CreateComponentInternal(const XMLFactoryNodeC &node,RCWrapAbstractC &rawHandle);
    //: Create a component

    void ReportUseComponentFailed(const XMLFactoryContextC &currentNode,const StringC &name,const std::type_info &type) const;
    //: Report failure in UseComponent

    StringC m_masterConfigFilename;
    XMLTreeC m_configRoot;
    XMLTreeC m_configTree;
    XMLFactoryNodeC::RefT m_iRoot;
    //HashC<StringC,RCWrapAbstractC> m_components;
    bool m_setupClean;
    bool m_donePostSetup;
    bool m_verbose;
    bool m_checkConfig;
    
    static HashC<StringC,TypeFactoryT> &Type2Factory();
    friend class XMLFactoryNodeC;

  private:
    XMLFactoryC(const XMLFactoryC &);
    //: Copy constructor, not supported!
  };

  //! userlevel=Normal
  //: Handle class for XMLFactoryC

  class XMLFactoryHC
    : public RCHandleC<XMLFactoryC>
  {
  public:
    XMLFactoryHC()
    {}
    //: Default constructor.
    // Creates an invalid handle.

    XMLFactoryHC(const StringC &configFile)
      : RCHandleC<XMLFactoryC>(new XMLFactoryC(configFile))
    { Body().PostReadConfig(); }
    //: Open a config file.

    XMLFactoryHC(const StringC &configFileName,const XMLTreeC &configTree)
      : RCHandleC<XMLFactoryC>(new XMLFactoryC(configFileName,configTree))
    { Body().PostReadConfig(); }
    //: Use an existing tree.

    const XMLTreeC &ConfigTree() const
    { return Body().ConfigTree(); }
    //: Access config tree

    const StringC &MasterConfigFilename() const
    { return Body().MasterConfigFilename(); }
    //: Access name of config file.

    XMLFactoryNodeC &IRoot()
    { return Body().IRoot(); }
    //: Root

    XMLFactoryContextC RootContext()
    { return Body().RootContext(); }
    //: Access the root context.

    const XMLFactoryNodeC &IRoot() const
    { return Body().IRoot(); }
    //: Root

    template<class DataT>
    bool SetComponent(const StringC &name,const DataT &data)
    { return Body().SetComponent(name,data); }
    //: Set named component.

    template<class DataT>
    bool GetComponent(const StringC &name,DataT &data)
    { return Body().GetComponent(name,data); }
    //: Get named component.

    template<class DataT>
    bool UseComponent(const StringC &name,
                      DataT &data,
                      bool suppressErrorMessages = false,
                      const std::type_info &defaultType=typeid(void)
                      ) const
    {
      XMLFactoryContextC context(Body());
      return Body().UseComponent(context,name,data,suppressErrorMessages,defaultType);
    }
    //: Get named component, or create it if not found.

    template<class DataT>
    bool CreateComponent(const XMLFactoryContextC &node,DataT &data)
    { return Body().CreateComponent(node,data); }
    //: Create a component

    template<class DataT>
    bool CreateComponent(const StringC &name,
                         DataT &data,
                         bool suppressErrorMessages = false
                         )
    {
      XMLFactoryContextC context(Body());
      return Body().CreateComponent(context,name,data,suppressErrorMessages);
    }
    //: Create named component. Event if it exists already.

    using RCHandleC<XMLFactoryC>::BodyPtr;
  };


  template<class DataT>
  inline bool XMLFactoryContextC::UseComponent(const StringC &name,
                                        DataT &data,
                                        bool suppressErrorMessages,
                                        const std::type_info &defaultType) const
  { return Factory().UseComponent(*this,name,data,suppressErrorMessages,defaultType,XMLFACTORY_SEARCH_PARENT_NODES); }
  //: Get named component, or create it if not found.

  template<class DataT>
  inline bool XMLFactoryContextC::UseChildComponent(const StringC &name,
                                        DataT &data,
                                        bool suppressErrorMessages,
                                        const std::type_info &defaultType) const
  { return Factory().UseComponent(*this,name,data,suppressErrorMessages,defaultType,XMLFACTORY_SEARCH_LOCAL_ONLY); }
  //: Get named component, or create it if not found.

  inline bool XMLFactoryContextC::UseComponent(const StringC &name,
                      RCWrapAbstractC &data,
                      bool suppressErrorMessages,
                      const std::type_info &defaultType) const
  { return Factory().UseComponent(*this,name,data,suppressErrorMessages,defaultType,XMLFACTORY_SEARCH_PARENT_NODES); }
  //: Get named component, or create it if not found.
  // This will search parent context's succesively up the tree until a
  // component with a matching name is found.

  inline bool XMLFactoryContextC::UseChildComponent(const StringC &name,
                           RCWrapAbstractC &data,
                           bool suppressErrorMessages,
                           const std::type_info &defaultType) const
  { return Factory().UseComponent(*this,name,data,suppressErrorMessages,defaultType,XMLFACTORY_SEARCH_LOCAL_ONLY); }
  //: Get named component, or create it if not found.
  // This will only check the local context for the component, creating
  // a new instance if its not found.


  template<class DataT>
  bool XMLFactoryContextC::CreateComponent(const StringC &name,DataT &data,
                                           bool suppressErrorMessages,
                                           XMLFactoryCreateModeT createMode) const
  { return Factory().CreateComponent(*this,name,data,suppressErrorMessages,createMode); }
  //: Create named component.

  template<class DataT>
  bool XMLFactoryContextC::SetComponent(const StringC &name,const DataT &data)
  { return Factory().SetComponent(Path() + ':' + name,data); }
  //: Get named component.

  template<class DataT>
  bool XMLFactoryContextC::GetComponent(const StringC &name,DataT &data,bool suppressErrorMessages) const
  { return Factory().GetComponent(Path() + ':' + name,data); }
  //: Get instance of the named component. Fail if its not found.

}

#endif
