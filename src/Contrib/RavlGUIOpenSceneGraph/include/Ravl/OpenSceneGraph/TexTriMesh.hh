// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2010, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! file = "Ravl/Contrib/OpenSceneGraph/TexTriMesh.hh"
//! lib=RavlGUIOpenSceneGraph
//! author = "Warren Moore"
//! docentry = "Ravl.API.Graphics.OpenSceneGraph"
//! example = "exOpenSceneGraphWidget.cc"

#ifndef RAVLGUI_OPENSCENEGRAPHTEXTRIMESH_HEADER
#define RAVLGUI_OPENSCENEGRAPHTEXTRIMESH_HEADER 1

#include "Ravl/OpenSceneGraph/TriMesh.hh"
#include "Ravl/3D/TexTriMesh.hh"

namespace RavlOSGN
{

  //! userlevel=Normal
  //: Node object containing a TexTriMesh mesh.

  class TexTriMeshC
  : public TriMeshC
  {
  public:
    TexTriMeshC();
    //: Ctor.

    TexTriMeshC(const Ravl3DN::TexTriMeshC &texTriMesh);
    //: Ctor.

    TexTriMeshC(const XMLFactoryContextC &factory);
    //: XML Factory constructor

    virtual ~TexTriMeshC();
    //: Dtor.

    bool SetMesh(const Ravl3DN::TexTriMeshC &texTriMesh);
    // Set the node to display the mesh.

    typedef RavlN::SmartOwnerPtrC<TexTriMeshC> RefT;
    typedef RavlN::SmartCallbackPtrC<TexTriMeshC> CBRefT;
    
  protected:
    bool Setup(const XMLFactoryContextC &factory);
    //: Do setup from factory
    
    virtual void ZeroOwners();
    //: Called when owner handles drop to zero.
  };

}

#endif
