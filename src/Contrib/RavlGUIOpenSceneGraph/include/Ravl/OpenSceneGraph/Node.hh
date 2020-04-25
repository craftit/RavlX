// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2010, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! file = "Ravl/Contrib/OpenSceneGraph/Node.hh"
//! lib=RavlGUIOpenSceneGraph
//! author = "Warren Moore"
//! docentry = "Ravl.API.Graphics.OpenSceneGraph"
//! example = "exOpenSceneGraphWidget.cc"

#ifndef RAVLGUI_OPENSCENEGRAPHNODE_HEADER
#define RAVLGUI_OPENSCENEGRAPHNODE_HEADER 1

#include "Ravl/SmartPtr.hh"
#include <osg/ref_ptr>
#include <osg/Node>
#include <osg/NodeCallback>
#include "Ravl/Threads/Signal.hh"
#include "Ravl/XMLFactory.hh"

namespace RavlOSGN
{
  class NodeC;
  class NodeCallbackC;
  class NodeVisitorC;
  using RavlN::XMLFactoryContextC;
  
  // Class for holding a reference back an arbitrary object.

  class NodeRefC
    : public osg::Referenced
  {
  public:
    //! Default constructor
    NodeRefC()
     : m_node(0)
    {}
    
    //! Construct from data
    NodeRefC(NodeC *node)
    : m_node(node)
    {}
    
    //! Access node
    NodeC *Node()
    { return m_node; }

    //! Access node
    const NodeC *Node() const
    { return m_node; }
  protected:
    NodeC *m_node;
  };
  
  template<class DataT>
  class NodeDataC
   : public NodeRefC
  {
  public:
    //! Default constructor
    NodeDataC()
    {}
    
    //! Construct from data
    NodeDataC(NodeC *node,const DataT &data)
    : NodeRefC(node),
      m_data(data)
    {}
    
    //! Access data
    DataT &Data()
    { return m_data; }
    
    //! Access data
    const DataT &Data() const
    { return m_data; }

    //! Access data
    DataT &operator*()
    { return m_data; }
    
    //! Access data
    const DataT &operator*() const
    { return m_data; }

  protected:
    DataT m_data;
  };
  
  //! userlevel=Normal
  //: Node base object.

  class NodeC
  : public RavlN::RCLayerBodyC
  {
  public:
    NodeC();
    //: Ctor.
    
    NodeC(const XMLFactoryContextC &factory);
    //: XML Factory constructor

    virtual ~NodeC();
    //: Dtor.

    bool EnableCallback(const bool enable = true);
    //: Enabled the callback function.

    RavlN::Signal0C &SigCallback()
    { return m_sigCallback; }
    //: Access the callback signal.
    
    bool BringToFront();
    //: Make sure the node is rendered after all other objects and disable depth testing.

    osg::ref_ptr<osg::Node> &Node()
    { return m_node; }
    //: Access the object smart pointer.
        
    typedef RavlN::SmartOwnerPtrC<NodeC> RefT;
    typedef RavlN::SmartCallbackPtrC<NodeC> CBRefT;
    
    static bool GetNode(osg::Node *,NodeC::RefT &node);
    //: Get node handle if it exists.

    void SetName(const std::string &name);
    //: Set the name of the node.

    template<class DataT>
    void SetUserData(const DataT &data)
    { m_node->setUserData(new NodeDataC<DataT>(this,data)); }
    //: Store user data in node.
    
    template<class DataT>
    bool GetUserData(DataT &data) {
      NodeDataC<DataT> *nodeData = dynamic_cast<NodeDataC<DataT> *>(m_node->getUserData());
      if(nodeData == 0)
        return false;
      data = nodeData->Data();
      return true;
    }
    //: Retrieve data from node.
    // Returns true if data of matching type has been found.

    template<class DataT>
    static bool GetUserData(osg::Node *node,DataT &data) {
      NodeDataC<DataT> *nodeData = dynamic_cast<NodeDataC<DataT> *>(node->getUserData());
      if(nodeData == 0)
        return false;
      data = nodeData->Data();
      return true;
    }
    //: Retrieve data from node.
    // Returns true if data of matching type has been found.

    const std::string &Name() const
    { return m_node->getName(); }
    //: Access name of node.
    
    virtual RavlN::Signal0C &SigResize() const;
    //: Access resize signal

    void Accept(NodeVisitorC &visitor);
    //: Visit all the child nodes derived from NodeC of this one

    void SetNodeRef();
    //: Setup node ref so we can get back from the scene graph node to this one.
  protected:
    virtual void DoCallback();
    //: Process a callback.
    
    bool Setup(const XMLFactoryContextC &factory);
    //: Do setup from factory
    
    virtual void ZeroOwners();
    //: Called when owner handles drop to zero.

    osg::ref_ptr<osg::Node> m_node;
    osg::ref_ptr<osg::NodeCallback> m_callback;
    RavlN::Signal0C m_sigCallback;
    mutable RavlN::Signal0C m_sigResize;
    
    friend class NodeCallbackC;
  };


}

#endif
