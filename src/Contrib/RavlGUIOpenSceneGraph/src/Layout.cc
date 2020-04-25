// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2011, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlGUIOpenSceneGraph

#include "Ravl/OpenSceneGraph/Layout.hh"
#include "Ravl/XMLFactoryRegister.hh"
#include "Ravl/RLog.hh"
#include "Node.hh"
#include <osg/ComputeBoundsVisitor>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlOSGN {
  
  //: Constructor
  
  LayoutEntryC::LayoutEntryC(const NodeC &element)
   : m_element(&element)
  {
    AddChildNode(element);
    
    //SetPosition(Vector3dC(0,0,0));
    //SetScale(Vector3dC(1.0,1.0,1.0));
    
    Node()->setDataVariance(osg::Object::DYNAMIC);
    
    ComputeBounds();
    
  }
  
  //: Destructor
  
  LayoutEntryC::~LayoutEntryC()
  {
    m_connections.DisconnectAll();
  }

  
  bool LayoutEntryC::ComputeBounds()
  {
    osg::ComputeBoundsVisitor boundsVisitor; 
    m_element->Node()->accept(boundsVisitor);
    m_bounds = boundsVisitor.getBoundingBox(); 
    ONDEBUG(rDebug("Bounds %f %f %f : %f %f %f",
           m_bounds._min[0],m_bounds._min[1],m_bounds._min[2],
           m_bounds._max[0],m_bounds._max[1],m_bounds._max[2]
           ));
    return true;
  }
  
  void LayoutEntryC::SetTargetPosition(const Vector3dC &position)
  {
    ONDEBUG(rDebug("Target position %s ",RavlN::StringOf(position).data()));
    m_targetPosition = position;
    SetPosition(position);
  }

  //---------------------------------------------------------------

  //: Default constructor
  LayoutC::LayoutC(bool create)
   : GroupC(create)
  {}
  
  //: XML factory constructor
  LayoutC::LayoutC(const XMLFactoryContextC &factory)
   : GroupC(true)
  {
    Setup(factory);
  }
  
  //: Add a node object to the group.
  bool LayoutC::AddChildNode(const NodeC &node)
  {
    
    LayoutEntryC::RefT le = new LayoutEntryC(node);
    le->ConnectionSet() += RavlN::ConnectPtr(node.SigResize(),CBRefT(this),&LayoutC::HandleChildResize);
    
    m_nodes.push_back(le);
    GroupC::AddChildNode(*le);
    UpdateLayout();
    if(m_sigResize.IsValid())
      m_sigResize();
    return true;
  }

  //: Remove a node object from the group.
  bool LayoutC::RemoveChildNode(const NodeC &node)
  {
    for(std::vector<LayoutEntryC::RefT>::iterator i= m_nodes.begin();i != m_nodes.end();++i) {
      if((*i)->ElementRef().BodyPtr() == &node) {
        GroupC::RemoveChildNode(**i);
        m_nodes.erase(i);
        UpdateLayout();
        if(m_sigResize.IsValid())
          m_sigResize();
        return true;
      }
    }
    rError("Failed to find node %p ",(void *)&node);
    return false;
  }

  //: Do setup from factory
  bool LayoutC::Setup(const XMLFactoryContextC &factory)
  {
    GroupC::Setup(factory);
    return true;
  }
   
  //: Update the layout
  bool LayoutC::UpdateLayout()
  {
    for(unsigned i =0;i < m_nodes.size();i++) {
      m_nodes[i]->SetPosition(Vector3dC(i*1.0,0.0,0.0));
    }
    return true;
  }

  //: Called when owner handles drop to zero.
  void LayoutC::ZeroOwners()
  {
    m_nodes.clear();
    GroupC::ZeroOwners();
  }

  //: Process a callback.
  void LayoutC::DoCallback() {    
    GroupC::DoCallback();
  }
  
  //: Handle resize of child object.
  
  bool LayoutC::HandleChildResize() {
    UpdateLayout();
    if(m_sigResize.IsValid())
      m_sigResize();
    return true;
  }

  
  void LinkLayout()
  {}
  
  static RavlN::XMLFactoryRegisterConvertC<LayoutC,GroupC> g_registerXMLFactoryLayout("RavlOSGN::LayoutC");

}
