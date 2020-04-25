// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2010, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! file = "Ravl/Contrib/OpenSceneGraph/HUD.hh"
//! lib=RavlGUIOpenSceneGraph
//! author = "Warren Moore"
//! docentry = "Ravl.API.Graphics.OpenSceneGraph"
//! example = "exOpenSceneGraphWidget.cc"

#ifndef RAVLGUI_OPENSCENEGRAPHHUD_HEADER
#define RAVLGUI_OPENSCENEGRAPHHUD_HEADER 1

#include "Ravl/OpenSceneGraph/Group.hh"
#include "Ravl/RealRange2d.hh"
#include <osg/MatrixTransform>

namespace RavlOSGN
{

  //! userlevel=Normal
  //: Heads Up Display object.
  // Adds a 2D orthographic project and model view matrix into the scene graph.

  class HUDC
  : public GroupC
  {
  public:
    HUDC(const RavlN::RealRange2dC &coords = RavlN::RealRange2dC(0, 0, 1, 1));
    //: Ctor.
    //!param: coords - The 2D coordinate space of the display in the format (min row, min col, max row, max col).

    HUDC(const XMLFactoryContextC &factory);
    //: Factory constructor.
    
    virtual ~HUDC();
    //: Dtor.

    virtual bool AddChildNode(const NodeC &node);
    //: Add a node object to the display.

    virtual bool RemoveChildNode(const NodeC &node);
    //: Remove a node object from the display.

    typedef RavlN::SmartOwnerPtrC<HUDC> RefT;
    typedef RavlN::SmartCallbackPtrC<HUDC> CBRefT;
    
  protected:
    bool Setup(const XMLFactoryContextC &factory);
    //: Do setup from factory

    bool BuildNode(const RavlN::RealRange2dC &coords);
    //: Build node.

    virtual void ZeroOwners();
    //: Zero owners reached.
    
    osg::ref_ptr<osg::MatrixTransform> m_modelViewMatrix;
  };

}

#endif
