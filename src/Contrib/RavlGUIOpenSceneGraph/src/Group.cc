// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2010, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! file = "Ravl/Contrib/OpenSceneGraph/Group.cc"
//! lib=RavlGUIOpenSceneGraph
//! author = "Warren Moore"

#include "Ravl/OpenSceneGraph/Group.hh"
#include "Ravl/XMLFactoryRegister.hh"

#include <osg/Group>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlOSGN
{

  using namespace osg;

  GroupC::GroupC(bool create)
  {
    if (create)
      m_node = new Group();
  }

  //: XML Factory constructor
  GroupC::GroupC(const XMLFactoryContextC &factory)
   : NodeC(factory)
  {
    m_node = new Group();
    Setup(factory);
  }
  
  GroupC::~GroupC()
  {}
  
  bool GroupC::AddChild(const NodeC::RefT &node)
  {
    if(!node.IsValid())
      return false;
    
    return AddChildNode(*node);
  }

  bool GroupC::RemoveChild(const NodeC::RefT &node)
  {
    if(!node.IsValid())
      return false;
    
    return RemoveChildNode(*node);
  }

  bool GroupC::AddChildNode(const NodeC &node) {
    ref_ptr<Group> groupRef = m_node->asGroup();
    if (!groupRef) {
      RavlError("Not a group!");
      return false;
    }
    groupRef->addChild(const_cast<NodeC &>(node).Node());
    return true;
  }
  //: Add a node object to the group.

  bool GroupC::RemoveChildNode(const NodeC &node) {
    ref_ptr<Group> groupRef = m_node->asGroup();
    if (!groupRef) {
      RavlError("Not a group!");
      return false;
    }
    groupRef->removeChild(const_cast<NodeC &>(node).Node());
    return true;
  }
  //: Remove a node object from the group.

  bool GroupC::RemoveChildren()
  {
    if (!m_node)
      return false;

    ref_ptr<Group> groupRef = m_node->asGroup();
    if (!groupRef)
      return false;

    const int groupSize = groupRef->getNumChildren();
    if (groupSize > 0)
      groupRef->removeChildren(0, groupSize);

    return true;
  }
  
  //: Process a callback.
  
  void GroupC::DoCallback()
  {
    NodeC::DoCallback();
  }
  
  //: Do setup from factory
  
  bool GroupC::Setup(const XMLFactoryContextC &factory)
  {
    NodeC::Setup(factory);
    
    // Short hand if we just want a single child node.
    NodeC::RefT node;
    if(factory.UseComponent("Node",node,true)) {
      AddChild(node);
    }
    
    // Full list of child nodes.
    std::vector<NodeC::RefT> children;
    factory.UseComponentGroup("Nodes",*this,&GroupC::AddChild);
    return true;
  }

  //: Called when owner handles drop to zero.
  void GroupC::ZeroOwners()
  {
    NodeC::ZeroOwners();
  }

  void LinkGroup()
  {}
  
  static RavlN::XMLFactoryRegisterConvertC<GroupC,NodeC> g_registerXMLFactoryGroup("RavlOSGN::GroupC");


}
