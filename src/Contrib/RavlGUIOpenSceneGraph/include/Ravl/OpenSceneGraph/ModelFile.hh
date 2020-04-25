// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2010, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! file = "Ravl/Contrib/OpenSceneGraph/ModelFile.hh"
//! lib=RavlGUIOpenSceneGraph
//! author = "Warren Moore"
//! docentry = "Ravl.API.Graphics.OpenSceneGraph"
//! example = "exOpenSceneGraphWidget.cc"

#ifndef RAVLGUI_OPENSCENEGRAPHMODELFILE_HEADER
#define RAVLGUI_OPENSCENEGRAPHMODELFILE_HEADER 1

#include "Ravl/OpenSceneGraph/Node.hh"
#include <string>

namespace RavlOSGN
{

  //! userlevel=Normal
  //: Node object to load a model file.

  class ModelFileC
  : public NodeC
  {
  public:
    ModelFileC(const std::string &filename);
    //: Ctor.
    //!param: filename - Filename of the model to load.
    
    ModelFileC(const XMLFactoryContextC &factory);
    //: XML Factory constructor
    
    virtual ~ModelFileC();
    //: Dtor.

    typedef RavlN::SmartOwnerPtrC<ModelFileC> RefT;
    typedef RavlN::SmartCallbackPtrC<ModelFileC> CBRefT;

  protected:
    virtual void ZeroOwners();
    //: Zero owners reached.

    bool BuildNode(const std::string &filename);
    //: Build the node
  };

}

#endif
