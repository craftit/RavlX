// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2010, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! file = "Ravl/Contrib/OpenSceneGraph/Image.hh"
//! lib=RavlGUIOpenSceneGraph
//! author = "Warren Moore"
//! docentry = "Ravl.API.Graphics.OpenSceneGraph"
//! example = "exOpenSceneGraphWidget.cc"

#ifndef RAVLGUI_OPENSCENEGRAPHIMAGE_HEADER
#define RAVLGUI_OPENSCENEGRAPHIMAGE_HEADER 1

#include "Ravl/OpenSceneGraph/Node.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/RealRange2d.hh"
#include <osg/Geometry>

namespace RavlOSGN
{

  //! userlevel=Normal
  //: Node object containing a ByteRGBAValueC image.

  class ImageC
  : public NodeC
  {
  public:
    ImageC(const XMLFactoryContextC &factory);
    //: Factory constructor.
    
    virtual ~ImageC();
    //: Dtor.

    bool SetSize(const RavlN::RealRange2dC &coords);
    //: Set the object size.
    //!param: coords - The 2D position of the image in the X/Y plane.

    bool AlphaImageEnable(bool alphaImageEnable);
    //: Enable transparency for the alpha channel in the image (off by default for performance reasons).

    bool SetAlpha(float alpha);
    //: Set an alpha value for the entire image.

    typedef RavlN::SmartOwnerPtrC<ImageC> RefT;
    //: Reference type.
    
    typedef RavlN::SmartCallbackPtrC<ImageC> CBRefT;
    //: Reference type.

  protected:
    
    ImageC(const RavlN::RealRange2dC &coords = RavlN::RealRange2dC(1, 1));
    //: Ctor.
    //!param: coords - The 2D position of the image in the X/Y plane.

    bool BuildNode(const RavlN::RealRange2dC &coords);
    //: Build the node
    
    bool UpdateAlphaRenderState(osg::ref_ptr<osg::StateSet> &stateSetRef);
    //: Set rendering state.
    
    virtual void ZeroOwners();
    //: Zero owners reached.

    osg::ref_ptr<osg::Geometry> m_geometry;
    bool m_alphaImageEnabled;
    float m_alpha;
  };

}

#endif
