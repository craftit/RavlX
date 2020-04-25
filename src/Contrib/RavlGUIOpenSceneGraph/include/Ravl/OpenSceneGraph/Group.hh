// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2010, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! file = "Ravl/Contrib/OpenSceneGraph/Group.hh"
//! lib=RavlGUIOpenSceneGraph
//! author = "Warren Moore"
//! docentry = "Ravl.API.Graphics.OpenSceneGraph"
//! example = "exOpenSceneGraphWidget.cc"

#ifndef RAVLGUI_OPENSCENEGRAPHGROUP_HEADER
#define RAVLGUI_OPENSCENEGRAPHGROUP_HEADER 1

#include "Ravl/OpenSceneGraph/Node.hh"

namespace RavlOSGN
{

  //! userlevel=Normal
  //: Group object.

  class GroupC
   : public NodeC
  {
  public:
    GroupC(bool create = true);
    //: Ctor.
    //!param: create - If true, a new group object will be allocated.

    GroupC(const XMLFactoryContextC &factory);
    //: XML Factory constructor

    virtual ~GroupC();
    //: Dtor.

    bool AddChild(const NodeC::RefT &node);
    //: Add a node object to the group.

    bool RemoveChild(const NodeC::RefT &node);
    //: Remove a node object from the group.

    virtual bool AddChildNode(const NodeC &node);
    //: Add a node object to the group.

    virtual bool RemoveChildNode(const NodeC &node);
    //: Remove a node object from the group.

    bool RemoveChildren();
    //: Remove all node objects from the group.

    typedef RavlN::SmartOwnerPtrC<GroupC> RefT;
    typedef RavlN::SmartCallbackPtrC<GroupC> CBRefT;

  protected:
    virtual void DoCallback();
    //: Process a callback.

    bool Setup(const XMLFactoryContextC &factory);
    //: Do setup from factory
    
    virtual void ZeroOwners();
    //: Called when owner handles drop to zero.
    
  };

}

#endif
