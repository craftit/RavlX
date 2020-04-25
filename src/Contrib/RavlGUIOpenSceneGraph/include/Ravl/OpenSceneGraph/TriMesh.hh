// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2010, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! file = "Ravl/Contrib/OpenSceneGraph/TriMesh.hh"
//! lib=RavlGUIOpenSceneGraph
//! author = "Warren Moore"
//! docentry = "Ravl.API.Graphics.OpenSceneGraph"
//! example = "exOpenSceneGraphWidget.cc"

#ifndef RAVLGUI_OPENSCENEGRAPHTRIMESH_HEADER
#define RAVLGUI_OPENSCENEGRAPHTRIMESH_HEADER 1

#include "Ravl/OpenSceneGraph/Node.hh"
#include "Ravl/3D/TriMesh.hh"
#include <osg/Geometry>

namespace RavlOSGN
{

  //! userlevel=Normal
  //: Node object containing a TriMesh mesh.

  class TriMeshC
  : public NodeC
  {
  public:
    TriMeshC();
    //: Ctor.

    TriMeshC(const Ravl3DN::TriMeshC &triMesh);
    //: Ctor.
    
    TriMeshC(const XMLFactoryContextC &factory);
    //: XML Factory constructor


    virtual ~TriMeshC();
    //: Dtor.

    bool SetMesh(const Ravl3DN::TriMeshC &triMesh);
    // Set the node to display the mesh.

    typedef RavlN::SmartOwnerPtrC<TriMeshC> RefT;
    typedef RavlN::SmartCallbackPtrC<TriMeshC> CBRefT;
    
  protected:
    bool Setup(const XMLFactoryContextC &factory);
    //: Do setup from factory
    
    virtual void ZeroOwners();
    //: Called when owner handles drop to zero.

    osg::ref_ptr<osg::Geometry> m_geometry;
    Ravl3DN::TriMeshC m_triMesh;
  };

}

#endif
