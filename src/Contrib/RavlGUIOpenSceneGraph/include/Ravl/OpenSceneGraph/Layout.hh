// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2011, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/* 
 * File:   Layout.hh
 * Author: charlesgalambos
 *
 * Created on April 25, 2011, 9:54 AM
 */
//! lib=RavlGUIOpenSceneGraph

#ifndef RAVLOSG_LAYOUT_HH
#define	RAVLOSG_LAYOUT_HH

#include "Ravl/OpenSceneGraph/Group.hh"
#include "Ravl/OpenSceneGraph/Transform.hh"
#include "Ravl/Threads/SignalConnectionSet.hh"
#include <osg/BoundingBox>

namespace RavlOSGN {
  using RavlN::Vector3dC;
  
  //: Layout entry.
  
  class LayoutEntryC
   : public TransformPositionAttitudeC
  {
  public:
    LayoutEntryC(const NodeC &element);
    //: Constructor
    
    ~LayoutEntryC();
    //: Destructor
    
    bool ComputeBounds();
    //: Compute bounds of entry.
    
    NodeC::RefT &ElementRef()
    { return m_element; }
    //: Access element we're managing.
    
    const Vector3dC &TargetPosition() const
    { return m_targetPosition; }
    
    void SetTargetPosition(const Vector3dC &position);
    
    const osg::BoundingBox &Bounds() const
    { return m_bounds; }
    
    RavlN::SignalConnectionSetC &ConnectionSet()
    { return m_connections; }

    typedef RavlN::SmartOwnerPtrC<LayoutEntryC> RefT;
    //: Owner handle.
    
    typedef RavlN::SmartCallbackPtrC<LayoutEntryC> CBRefT;
    //: Callback handle
    
  protected:
    Vector3dC m_targetPosition;
    
    NodeC::RefT m_element;
    
    osg::BoundingBox m_bounds;
    
    RavlN::SignalConnectionSetC m_connections;
    
  };
  
  //: Base class for automatic layouts
  
  class LayoutC
   : public GroupC
  {
  public:
    LayoutC(bool create = true);
    //: Constructor
    
    LayoutC(const XMLFactoryContextC &factory);
    //: XML factory constructor
    
    virtual bool AddChildNode(const NodeC &node);
    //: Add a node object to the group.

    virtual bool RemoveChildNode(const NodeC &node);
    //: Remove a node object from the group.

    typedef RavlN::SmartOwnerPtrC<LayoutC> RefT;
    //: Owner handle.
    
    typedef RavlN::SmartCallbackPtrC<LayoutC> CBRefT;
    //: Callback handle

  protected:
    virtual bool UpdateLayout();
    //: Update the layout
    
    virtual void DoCallback();
    //: Process a callback.
    
    bool HandleChildResize();
    //: Handle resize of child object.
    
    bool Setup(const XMLFactoryContextC &factory);
    //: Do setup from factory
    
    virtual void ZeroOwners();
    //: Called when owner handles drop to zero.
    
    std::vector<LayoutEntryC::RefT> m_nodes;
    
  };
}


#endif	/* LAYOUT_HH */

