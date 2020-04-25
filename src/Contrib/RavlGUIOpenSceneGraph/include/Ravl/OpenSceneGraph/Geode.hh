// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2010, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! file = "Ravl/Contrib/OpenSceneGraph/Geode.hh"
//! lib=RavlGUIOpenSceneGraph
//! author = "Warren Moore"
//! docentry = "Ravl.API.Graphics.OpenSceneGraph"
//! example = "exOpenSceneGraphWidget.cc"

#ifndef RAVLGUI_OPENSCENEGRAPHGEODE_HEADER
#define RAVLGUI_OPENSCENEGRAPHGEODE_HEADER 1

#include "Ravl/OpenSceneGraph/Node.hh"
#include "Ravl/OpenSceneGraph/Drawable.hh"

namespace RavlOSGN
{

  //! userlevel=Normal
  //: Geode object.

  class GeodeC
  : public NodeC
  {
  public:
    GeodeC();
    //: Ctor.

    GeodeC(const DrawableC &drawable);
    //: Ctor.

    GeodeC(const XMLFactoryContextC &factory);
    //: XML Factory constructor

    virtual ~GeodeC();
    //: Dtor.

    bool AddDrawable(const DrawableC::RefT &drawable);
    //: Add a drawble object to the geode.

    typedef RavlN::SmartOwnerPtrC<GeodeC> RefT;
    typedef RavlN::SmartCallbackPtrC<GeodeC> CBRefT;
    
  protected:
    virtual void ZeroOwners();
    //: Called when owner handles drop to zero.

  };

}

#endif
